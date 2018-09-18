// =====================================================================================
//
//       Filename:  mmcFrameReceiver.cpp
//
//    Description:  单数据流帧接收
//
//        Version:  1.0
//        Created:  2018/2/24 14:43:08
//       Revision:  none
//       Compiler:  Diab
//
//         Author:  Pi Zucheng (), 
//   Organization:  
//
// =====================================================================================
#include "mmcFrameFinder.h"
#include "mmc.h"
#include "equipFrame.h"

namespace MMC
{
    FrameFinder::FrameFinder(const vector<FrameTrait *> &paramTraits)
        : isBufOverflow(false)
    {
        FrameTrait *trait = NULL;
        traits = new vector<FrameTrait *>();
        states = new TDequePair<FrameTrait *, ReceiverState>();
        buf = new vector<unsigned char>();
        ReceiverState initState;
        initState.HeaderPos = 0;
        initState.VirtualCutPos = 0;
        initState.NextWork = Finding;
        initState.FrameLength = 0;
        for(vector<FrameTrait *>::const_iterator it = paramTraits.begin();
                it != paramTraits.end(); ++it)
        {
            trait = (*it)->Clone();
            maxHeaderLength = std::max<unsigned int>(trait->GetHeader().size(), maxHeaderLength);
            traits->push_back(trait);
            initState.BestPartialSize = trait->BestPartialSize();
            states->AddElement(trait, initState);
        }
    }
//    FrameFinder::FrameFinder(const InPortFrameDescription &paramDesc)
//        : isBufOverflow(false)
//    {
//        traits = paramDesc.CloneTraits();       // need to be deleted
//        states = new TDequePair<FrameTrait *, ReceiverState>();
//        buf = new vector<unsigned char>();
//        ReceiverState initState;
//        initState.HeaderPos = 0;
//        initState.VirtualCutPos = 0;
//        initState.NextWork = Finding;
//        initState.FrameLength = 0;
//        for(vector<FrameTrait *>::const_iterator it = traits->begin();
//                it != traits->end(); ++it)
//        {
//            maxHeaderLength = std::max<unsigned int>((*it)->GetHeader().size(), maxHeaderLength);
//            initState.BestPartialSize = (*it)->BestPartialSize();
//            states->AddElement(*it, initState);
//        }
//    }
    FrameFinder::~FrameFinder()
    {
        for(vector<FrameTrait *>::const_iterator it = traits->begin();
                it != traits->end(); ++it)
        {
            if(NULL != (*it))
            {
//                printf("before delete FrameFinder trait\n");
                delete *it;
//                printf("after delete FrameFinder trait\n");
            }
        }
//        printf("before delete FrameFinder trait vec\n");
        delete traits;
//        printf("after delete FrameFinder trait vec\n");
        delete states;
//        printf("after delete FrameFinder states vec\n");
        delete buf;
//        printf("after delete FrameFinder buf vec\n");
    }
    bool FrameFinder::findHeader(const FrameTrait *t, unsigned int offset, unsigned int* pos)
    {
        vector<unsigned char> header = t->GetHeader();
        if(buf->size() >= offset + header.size())
        {
            vector<unsigned char>::iterator res = search(buf->begin() + offset, buf->end(),header.begin(), header.end());
            if(res != buf->end()) 
            {
                *pos = distance(buf->begin() + offset, res);
                return true;
            }
        }
        return false;
    }
    bool FrameFinder::IsBufOverflowed() const
    {
        return isBufOverflow;
    }
    unsigned int FrameFinder::GetFrames(vector<EquipFrame> *frames)
    {
        vector<unsigned char> tempArray;
        EquipFrame tempFrame;
        unsigned int frameCounter = 0;
        unsigned int minBufSize = 0;
        unsigned int tempUint = 0;
        bool finishedWork = false;
//        printf("Test in FrameFinder GetFrames traits size:%d\n", traits->size());
        for(vector<FrameTrait *>::iterator it = traits->begin();
                it != traits->end(); ++it)
        {
            finishedWork = false;
            ReceiverState *st = &(*states)[*it];
//            if((*it)->GetId() == ID_FCC_O_FrA)
//            {
//                printf("Test in FrameFinder state:%d, bufsize:%d\n", st->NextWork, buf->size());
//            }
            while(!finishedWork)
            {
                switch(st->NextWork)
                {
                    case Finding:               // 头查找
                        minBufSize = st->VirtualCutPos + (*it)->GetHeader().size();
//                        printf("Test in FrameFinder minBufSize:%d\n", minBufSize);
                        if(buf->size() >= minBufSize)
                        {
                            if(findHeader(*it, st->VirtualCutPos, &tempUint)) // HeaderPos = 0优化
                            {
                                st->HeaderPos = tempUint;
                                st->NextWork = TraitTest;
                                minBufSize += st->HeaderPos;
                            }
                            else                // 未找到帧头
                            {
                                // 留下小于帧头1个字节长度的内容
                                st->VirtualCutPos = buf->size() - (*it)->GetHeader().size() + 1;
                                finishedWork = true; // 等待数据继续Finding
                            }
                        }
                        else                    // 长度不足够
                        {
                            finishedWork = true; // 等待数据继续Finding
                        }
                        break;
                    case TraitTest:             // 特征匹配查询
                        minBufSize = st->VirtualCutPos + st->HeaderPos + st->BestPartialSize;
                        if(buf->size() >= minBufSize)
                        {
                            tempArray.clear();
                            copy(buf->begin() + st->VirtualCutPos + st->HeaderPos,
                                    buf->begin() + st->VirtualCutPos + st->HeaderPos + st->BestPartialSize,
                                    back_inserter(tempArray)); // 获取足够数据进行特征判断
                            switch((*it)->IsMe(&tempArray))
                            {
                                case FrameTrait::Success:
                                    st->NextWork = WaittingData;
                                    st->FrameLength = (*it)->Length(&tempArray);
                                    if(st->FrameLength < (*it)->GetHeader().size()) // 对帧长进行合法性检测
                                    {
                                        st->VirtualCutPos = st->VirtualCutPos + st->HeaderPos + 1; // 去掉帧头的第一个字节，继续查找
                                        st->NextWork = Finding;
                                    }
                                    break;
                                case FrameTrait::Unknown:
                                case FrameTrait::Failed:
                                    st->VirtualCutPos = st->VirtualCutPos + st->HeaderPos + 1; // 去掉帧头的第一个字节，继续查找
                                    st->NextWork = Finding;
                                    break;
                                default:        // 异常情况则跳过
                                    st->VirtualCutPos = buf->size();
                                    finishedWork = true;
                            }
                        }
                        else                    // 不够检测
                        {
//                            st->VirtualCutPos = st->VirtualCutPos + st->HeaderPos; // 移动数据，优化比特流存储
//                            st->HeaderPos = 0;
                            finishedWork = true; // 等待数据继续进行TraitTest
                        }
                        break;
                    case WaittingData:          // 等待数据
                        minBufSize = st->VirtualCutPos + st->HeaderPos + st->FrameLength;
                        if(buf->size() >= minBufSize) 
                        {
                            tempArray.clear();
                            copy(buf->begin() + st->VirtualCutPos + st->HeaderPos,
                                    buf->begin() + st->VirtualCutPos + st->HeaderPos + st->FrameLength,
                                    back_inserter(tempArray));
                            tempFrame.SetFrameID((*it)->GetId());
                            tempFrame.SetFrameContent(tempArray);
                            frames->push_back(tempFrame);
                            frameCounter++;
                            st->VirtualCutPos = st->VirtualCutPos + st->HeaderPos + st->FrameLength;
                            st->NextWork = Finding;
                        }
                        else
                        {
                            finishedWork = true; // 等待数据继续提取
                        }
                        break;
                    default:;
                }
            }
        }
        moveVirtualCutPos();
        return frameCounter;
    }
    void FrameFinder::moveVirtualCutPos()
    {
        unsigned int minCutPos = buf->size();   // 默认删除全部数据
        for(vector<FrameTrait *>::iterator it = traits->begin();
                it != traits->end(); ++it)
        {
            minCutPos = std::min<unsigned int>((*states)[*it].VirtualCutPos, minCutPos);
        }
        vector<unsigned char> *remainVec = new vector<unsigned char>();
        copy(buf->begin() + minCutPos, buf->end(), back_inserter(*remainVec));
        buf->swap(*remainVec);
//        printf("min cut pos:%d, bufsize:%d\n",minCutPos, buf->size());
        delete remainVec;

        for(vector<FrameTrait *>::iterator it = traits->begin();
                it != traits->end(); ++it)
        {
            (*states)[*it].VirtualCutPos -= minCutPos;
        }
    }
    unsigned int FrameFinder::InBuffer(const vector<unsigned char> &newData)
    {
        copy(newData.begin(), newData.end(), back_inserter(*buf));
//        printf("Test in FrameFinder InBuffer new data size:%d\n", buf->size());
        if(buf->size() > maxBufLength)
        {
            buf->clear();                       // 采用清空进行溢出处理
            printf("FrameFinder buf isBufOverflowed\n");
            isBufOverflow = true;
        }
        return newData.size();
    }
    FrameFinder:: FrameFinder(const  FrameFinder &org){
        if(this == &org){}
    } // hide
    FrameFinder& FrameFinder::operator=(const  FrameFinder &org){
        if(this == &org){}
        return *this;
    } // hide
}


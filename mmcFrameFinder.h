// =====================================================================================
//
//       Filename:  mmcFrameReceiver.h
//
//    Description:  单数据流帧接收
//
//        Version:  1.0
//        Created:  2018/2/24 14:44:03
//       Revision:  none
//       Compiler:  Diab
//
//         Author:  Pi Zucheng (),
//   Organization:
//
// =====================================================================================
#ifndef mmcFrameFinder_INC
#define mmcFrameFinder_INC

#include "equipframe.h"
#include "mmcFrameStructure.h"
#include "mmcTemplateClass.h"
#include <iostream>
#include <string>
#include <vector>

#ifdef _cplusplus
extern "C" {
#endif
namespace MMC {
    class FrameFinder {
    public:
        typedef enum {
            Finding, // 搜寻工作
            TraitTest, // 特征检查
            WaittingData // 等待数据
        } TraitWorkingState;
        typedef struct
        {
            TraitWorkingState NextWork; // 当前工作状态
            unsigned int VirtualCutPos; // 虚切帧位置
            unsigned int HeaderPos; // 相对虚切点的帧头位置
            unsigned int BestPartialSize; // 判断帧的最佳长度
            unsigned int FrameLength; // 帧长度
        } ReceiverState;
        FrameFinder(const vector<FrameTrait*>& paramTraits);
        //                FrameFinder(const InPortFrameDescription &paramDesc);
        unsigned int GetFrames(vector<EquipFrame>* frames); // 返回收到的帧数量
        unsigned int InBuffer(const vector<unsigned char>& newData); // 写字节流入buffer
        bool IsBufOverflowed() const;
        ~FrameFinder();

    private:
        bool findHeader(const FrameTrait* t, unsigned int offset, unsigned int* pos);
        void moveVirtualCutPos();
        FrameFinder(const FrameFinder& org);
        FrameFinder& operator=(const FrameFinder& org);
        vector<FrameTrait*>* traits;
        vector<unsigned char>* buf;
        TDequePair<FrameTrait*, ReceiverState>* states;
        unsigned int maxHeaderLength; // 用于保留数据的最小长度
        static const unsigned int maxBufLength = 102400; // 100kB
        bool isBufOverflow;
    };
}
#ifdef _cplusplus
}
#endif
#endif

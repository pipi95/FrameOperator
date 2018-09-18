//#include "vxWorks.h"
#include "mmcFrameStructure.h"
#include "dgpsframetrait.h"
#include "seatassignframetrait.h"
#include <algorithm>
#include <string>
#include <vector>

namespace MMC {
void FrameIDLib::insertTrait(
    FrameStructureIDEnum id,
    unsigned int length,
    const string& arrayHeader)
{
    vector<unsigned char> vHeader;
    copy(arrayHeader.begin(), arrayHeader.end(), back_inserter(vHeader));
    lib->push_back(new FrameTrait(id, length, vHeader));
}
void FrameIDLib::insertTrait(
    FrameStructureIDEnum id,
    unsigned int length,
    const string& arrayHeader,
    const string& arrayTrait)
{
    vector<unsigned char> vHeader, vTrait;
    copy(arrayHeader.begin(), arrayHeader.end(), back_inserter(vHeader));
    copy(arrayTrait.begin(), arrayTrait.end(), back_inserter(vTrait));
    lib->push_back(new FrameTrait(id, length, vHeader, vTrait));
}
void FrameIDLib::insertDGPSTrait(
    FrameStructureIDEnum id,
    const string& arrayHeader)
{
    vector<unsigned char> vHeader;
    copy(arrayHeader.begin(), arrayHeader.end(), back_inserter(vHeader));
    lib->push_back(new DGPSFrameTrait(id, vHeader));
}
void FrameIDLib::insertSeatAssignTrait(FrameStructureIDEnum id, const string& arrayHeader)
{
    vector<unsigned char> vHeader;
    copy(arrayHeader.begin(), arrayHeader.end(), back_inserter(vHeader));
    lib->push_back(new SeatAssignFrameTrait(id, vHeader));
}
FrameTrait* FrameIDLib::CloneTrait(FrameStructureIDEnum id) const
{
    for (vector<FrameTrait*>::const_iterator it = lib->begin();
         it != lib->end(); ++it) {
        if ((*it)->GetId() == id) {
            return (*it)->Clone();
        }
    }
    return NULL;
}

FrameTrait::~FrameTrait() {}

FrameTrait* FrameTrait::Clone() const
{
    return this->myClone();
}
FrameTrait* FrameTrait::myClone() const
{
    //        printf("before new FrameTrait in FrameTrait's clone\n");
    FrameTrait* p = new FrameTrait(*this);
    //        printf("after new FrameTrait in FrameTrait's clone\n");
    return p;
}

bool FrameIDLib::Contains(FrameStructureIDEnum id) const
{
    for (vector<FrameTrait*>::const_iterator it = lib->begin();
         it != lib->end(); ++it) {
        if (id == (*it)->GetId()) {
            return true;
        }
    }
    return false;
}

FrameIDLib::~FrameIDLib()
{
    if (NULL != lib) {
        for (vector<FrameTrait*>::iterator i = lib->begin();
             i != lib->end(); ++i) {
            if (*i != NULL) {
                delete *i;
            }
        }
        delete lib;
    }
}

FrameIDLib::FrameIDLib(const FrameIDLib& org)
{
    lib = new vector<FrameTrait*>();
    FrameTrait* tempFt;
    vector<FrameStructureIDEnum> ids = org.GetAllId();
    for (vector<FrameStructureIDEnum>::iterator it = ids.begin();
         it != ids.end(); ++it) {
        tempFt = org.CloneTrait(*it);
        if (NULL != tempFt) {
            lib->push_back(tempFt);
        }
    }
}

FrameIDLib& FrameIDLib::operator=(const FrameIDLib& org)
{
    if (this == &org) // self assign
    {
        return *this;
    }
    if (lib != NULL) {
        for (vector<FrameTrait*>::iterator it = lib->begin();
             it != lib->end(); ++it) {
            if ((*it) != NULL) {
                delete *it;
            }
        }
        delete lib;
    }
    lib = new vector<FrameTrait*>();
    FrameTrait* tempFt;
    vector<FrameStructureIDEnum> ids = org.GetAllId();
    for (vector<FrameStructureIDEnum>::iterator it = ids.begin();
         it != ids.end(); ++it) {
        tempFt = org.CloneTrait(*it);
        if (NULL != tempFt) {
            lib->push_back(tempFt);
        }
    }
    return *this;
}

vector<FrameStructureIDEnum> FrameIDLib::GetAllId() const
{
    vector<FrameStructureIDEnum> result;
    for (vector<FrameTrait*>::const_iterator it = lib->begin();
         it != lib->end(); ++it) {
        result.push_back((*it)->GetId());
    }
    return result;
}
FrameIDLib::FrameIDLib()
{
    lib = new vector<FrameTrait*>();
    string s;
    //        s = "\xEB\x90";                       /* 示例代码：在需要添加0到字符串中的情况 */
    //        s += '\0';
    //        s += "\x01\x02";
    // 关于帧特征序列的说明：当帧特征相同，但帧长度不同时（如ID_EO_MMC_LINK和ID_EO_MMC_MFCC_Orig）
    // 在对端口进行配置时不能将这样的帧放到同一个端口中，否则只能识别其中一种，另一种将被忽略
    // 在本例中光电载荷提供了两个串口分别传输这两种帧，所以不会有问题。
    s = "\xAA\x55\x21"; /* EO遥测 */

    //        printf("before insertTrait in ct of FrameIDLib\n");
    insertTrait(ID_EO_O_Telemetry, 33, s);
    //        printf("after insertTrait in ct of FrameIDLib\n");

    s = "\xAA\x55"; /* EO原给火控，去掉0x21以兼容标清 */
    insertTrait(ID_EO_O_MFCC, 26, s);

    s = "\xAA\x55"; /* EO 变帧长读取*/
    insertTrait(ID_EO_O_Dynamic, 0, s);

    s = "\xEB\x90\x40";
    s += '1';
    insertTrait(ID_FCC_O_Fr1, 64, s);

    s = "\xEB\x90\x40";
    s += '2';
    insertTrait(ID_FCC_O_Fr2, 64, s);

    s = "\xEB\x90\x40";
    s += '3';
    insertTrait(ID_FCC_O_Fr3, 64, s);

    s = "\xEB\x90\x40";
    s += '4';
    insertTrait(ID_FCC_O_Fr4, 64, s);

    s = "\xEB\x90\x40"; /* 飞控数据 */
    s += 'A';
    insertTrait(ID_FCC_O_FrA, 64, s);

    s = "\xEB\x90\x40";
    s += 'B';
    insertTrait(ID_FCC_O_FrB, 64, s);

    s = "\xEB\x90\x40";
    s += 'C';
    insertTrait(ID_FCC_O_FrC, 64, s);

    s = "\xEB\x90\x40";
    s += 'D';
    insertTrait(ID_FCC_O_FrD, 64, s);

    s = "\xEB\x90\x40";
    s += 'E';
    insertTrait(ID_FCC_O_FrE, 64, s);

    s = "\xEB\x90\x40";
    s += 'F';
    insertTrait(ID_FCC_O_FrF, 64, s);

    s = "\xEB\x90\x40";
    s += 'G';
    insertTrait(ID_FCC_O_FrG, 64, s);

    s = "\xEB\x90\x40";
    s += 'H';
    insertTrait(ID_FCC_O_FrH, 64, s);

    s = "\xEB\x90\x40";
    s += 'O';
    insertTrait(ID_FCC_O_FrO, 64, s);

    s = "\xEB\x90\x40"; /* 飞控I帧 */
    s += 'I';
    insertTrait(ID_FCC_O_FrI, 64, s);

    s = "\xEB\x90\x40"; /* 飞控J帧 */
    s += 'J';
    insertTrait(ID_FCC_O_FrJ, 64, s);

    s = "\xEB\x90"; /* 飞控动态帧 */
    insertTrait(ID_FCC_O_Dynamic, 0, s);

    s = "\x59\x7E\x2D\xAA\x51\xC1"; /* 火控C1帧 */
    insertTrait(ID_MFCC_O_FrC1, 0x2D, s);

    s = "\x59\x7E\x0B\xAA\x51\xB0";
    insertTrait(ID_MFCC_O_FrB0, 0x0B, s);

    s = "\x59\x7E\x0F\xAA\x51\xB1";
    insertTrait(ID_MFCC_O_FrB1, 0x0F, s);

    s = "\x59\x7E\x37\xAA\x51\xB5";
    insertTrait(ID_MFCC_O_FrB5, 0x37, s);

    s = "\x59\x7E\x0D\xAA\x51\xBB";
    insertTrait(ID_MFCC_O_FrBB, 0x0D, s);

    s = "\x59\x7E\x15\xAA\x51\xA1";
    insertTrait(ID_MFCC_O_FrA1, 0x15, s);

    s = "\x59\x7E\x12\xAA\x51\xA3";
    insertTrait(ID_MFCC_O_FrA3, 0x12, s);

    s = "\x59\x7E\x3A\xAA\x51\xAF";
    insertTrait(ID_MFCC_O_FrAF, 0x3A, s);

    s = "\x59\x7E\x27\xAA\x51\x24";
    insertTrait(ID_MFCC_O_Fr24, 0x27, s);

    s = "\x59\x7E\x1D\xAA\x51\x51"; /* 火控0x51帧 */
    insertTrait(ID_MFCC_O_Fr51, 0x1D, s);

    s = "\xEB\x90";
    insertTrait(ID_Link_O_Remote_Control, 64, s); /* 来自链路的遥控 */

    s = "\x7E\xE7\x09";
    insertTrait(ID_ATC_O_Telemetry, 11, s); /* 应答机的遥测 */

    s = "\x64";
    s += '\0';
    insertTrait(ID_Radar_O_Telemetry, 0, s); // 雷达的遥测

    s = "\x66\x41";
    insertTrait(ID_AIS_O_Telemetry, 0, s); // AIS
    //        printf("constructed size of lib:%d\n", lib->size());
    s = "\x4D\x43";
    insertTrait(ID_MMC_O_Telemetry, 0, s); // MMC

    s = "\xEB\x90";
    insertTrait(ID_FCC_I_TC, 32, s);

    s = "\x64";
    s += '\0';
    insertTrait(ID_EO_I_TC, 23, s);

    s = "\xAA\x51";
    insertTrait(ID_MFCU_I_TC, 25, s);

    s = "\x59\x7E";
    insertTrait(ID_DGPS1_I_TC, 0, s);

    s = "\x66\x41";
    insertTrait(ID_DGPS2_I_TC, 0, s);

    s = "\xEB\x90";
    insertTrait(ID_FCC_I_TM, 0, s);

    s = "\xAA\x55";
    insertTrait(ID_EO_I_TM, 0, s);

    s = "\x59\x7E";
    insertTrait(ID_MFCU_I_TM, 0, s);

    s = "\x64";
    s += '\0';
    insertTrait(ID_SAR_I_TM, 0, s);

    s = "\x41";
    insertTrait(ID_FCC_O_TC, 31, s);

    s = "\x51";
    insertTrait(ID_EO_O_TC, 31, s);

    s = "\x52";
    insertTrait(ID_MFCU_O_TC, 31, s);

    s = "\x53";
    insertTrait(ID_SAR_O_TC, 31, s);

    s = "\x71";
    insertTrait(ID_DGPS2_O_TC, 31, s);

    s = "\x70";
    insertTrait(ID_DGPS2_O_TC, 31, s);

    s = "\xEB\x90";
    insertTrait(ID_UPLINK_O_TC, 64, s);

    s = "\x59\x7E";
    insertDGPSTrait(ID_DGPS_I_PositionA, s);

    s = "\x66\x41";
    insertDGPSTrait(ID_DGPS_I_PositionB, s);

    s = "\x65\x0F";
    insertSeatAssignTrait(ID_SEAT_ASSIGN, s);

    s = "\xC5\x2F";
    insertTrait(ID_UNITY_I_TC, 0, s);
}

FrameTrait::FrameTrait(FrameStructureIDEnum paramId, unsigned int paramLength, const vector<unsigned char>& paramHeader, const vector<unsigned char>& paramTrait)
    : length(paramLength)
    , header(paramHeader)
    , trait(paramTrait)
    , id(paramId)
{
    bestPartialSize = std::max<unsigned int>((unsigned int)(header.size() + trait.size()), 3);
}

FrameTrait::FrameTrait(FrameStructureIDEnum paramId, unsigned int paramLength, const vector<unsigned char>& paramHeader)
    : length(paramLength)
    , header(paramHeader)
    , trait()
    , id(paramId)
{
    bestPartialSize = std::max<unsigned int>(paramHeader.size(), 3);
}

FrameStructureIDEnum FrameTrait::GetId() const
{
    return id;
}
FrameTrait::FrameTrait(FrameStructureIDEnum paramId, unsigned int paramLength, unsigned int paramBestPartialSize, const vector<unsigned char>& paramHeader, const vector<unsigned char>& paramTrait)
    : length(paramLength)
    , header(paramHeader)
    , trait(paramTrait)
    , id(paramId)
{
    bestPartialSize = std::max<unsigned int>((unsigned int)(header.size() + trait.size()), std::max<unsigned int>(3, paramBestPartialSize));
}

FrameTrait::FrameTrait()
    : length(0)
    , bestPartialSize(3)
    , header()
    , trait()
    , id(ID_Fr_Invalid)
{
}

FrameTrait::FrameTrait(const FrameTrait& org) /* left for sample */
    : length(org.Length())
    , bestPartialSize(org.BestPartialSize())
    , header(org.GetHeader())
    , trait(org.GetTrait())
    , id(org.GetId())
{
}

unsigned int FrameTrait::Length(const vector<unsigned char>* partialFrame) const
{
    unsigned int tempLength = 0;
    if (length == 0 && NULL != partialFrame) {
        tempLength = getVarLength(partialFrame);
    } else {
        tempLength = length;
    }

    if (tempLength > 1024) {
        tempLength = this->GetHeader().size();
    }

    return tempLength;
}

unsigned int FrameTrait::BestPartialSize() const
{
    return bestPartialSize;
}

unsigned int FrameTrait::getVarLength(const vector<unsigned char>* partialFrame) const
{
    if (partialFrame->size() >= 3) {
        return (unsigned int)partialFrame->at(2); /* 默认第3个字节是帧长字节 */
    } else {
        return length;
    }
}

FrameTrait& FrameTrait::operator=(const FrameTrait& org) /* left for sample */
{
    if (this == &org) {
        return *this;
    }
    length = org.Length();
    bestPartialSize = org.BestPartialSize();
    header = org.GetHeader();
    trait = org.GetTrait();
    id = org.GetId();
    return *this;
}

vector<unsigned char> FrameTrait::GetTrait() const
{
    return trait;
}

vector<unsigned char> FrameTrait::GetHeader() const
{
    return header;
}

bool FrameTrait::isHeaderOk(const vector<unsigned char>* partialFrame) const
{
    return equal<vector<unsigned char>::const_iterator, vector<unsigned char>::const_iterator>(
        header.begin(),
        header.end(),
        partialFrame->begin());
}

FrameTrait::ThreeState FrameTrait::isTraitOk(const vector<unsigned char>* partialFrame) const
{
    unsigned int idPos = header.size(); // 默认实现帧长字节（变帧长），头部（定帧长）后一个字节为帧ID
    if (length > 0) // fix length frame, behind of header is id
    {
        idPos++;
    } else { // variable length, behind of length byte is id
        idPos = idPos + 2;
    }
    if (partialFrame->size() >= idPos) {
        if (trait.at(0) == partialFrame->at(idPos - 1)) // ID is match
        {
            return Success;
        } else {
            return Failed;
        }
    } else {
        return Unknown;
    }
}

FrameTrait::ThreeState FrameTrait::IsMe(const vector<unsigned char>* partialFrame) const // 由传入的字节来判断是否属于本帧数据
{
    //        printf("partialFrame size:%d, trait size:%d\n", partialFrame.size(), trait.size());
    if (partialFrame->size() >= bestPartialSize) {
        if (header.size() > 0) //  must has header
        {
            if (isHeaderOk(partialFrame)) {
                if (trait.size() > 0) {
                    if (isTraitOk(partialFrame)) {
                        return Success;
                    } else {
                        return Failed;
                    }
                } else {
                    return Success;
                }
            } else {
                return Failed;
            }
        } else {
            return Failed;
        }
    } else {
        return Unknown;
    }
}
}

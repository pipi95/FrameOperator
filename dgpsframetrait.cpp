#include "dgpsframetrait.h"

using namespace MMC;
DGPSFrameTrait::DGPSFrameTrait(
    FrameStructureIDEnum id,
    const vector<unsigned char>& paramHeader)
    : FrameTrait(id, 0, paramHeader)
{
    this->bestPartialSize = defaultPartialSize;
}

DGPSFrameTrait::~DGPSFrameTrait() {}

DGPSFrameTrait::DGPSFrameTrait(const DGPSFrameTrait& org)
    : FrameTrait(id, 0,org.GetHeader())
{
    trait = org.GetTrait();
    bestPartialSize = defaultPartialSize;
}

DGPSFrameTrait& DGPSFrameTrait::operator=(const DGPSFrameTrait& org)
{
    if (this == &org) {
        return *this;
    }
    length = 0;
    bestPartialSize = defaultPartialSize;
    header = org.GetHeader();
    trait = org.GetTrait();
    id = org.GetId();
    return *this;
}

FrameTrait* DGPSFrameTrait::myClone() const
{
    FrameTrait* p = new DGPSFrameTrait(*this);
    return p;
}

FrameTrait::ThreeState DGPSFrameTrait::isTraitOk(const vector<unsigned char>* partialFrame) const
{
    if (partialFrame->size() >= bestPartialSize)
        return Success;
    else
        return Unknown;
}
unsigned int DGPSFrameTrait::getVarLength(const vector<unsigned char>* partialFrame) const
{
    unsigned int tempLength = 0;
    if (partialFrame->size() >= bestPartialSize) {
        tempLength = caculation(partialFrame->at(4), partialFrame->at(7), partialFrame->at(8));
        //todo: fix this problem!
    }
    else {
        tempLength = this->GetHeader().size();
    }
    return tempLength;
}
unsigned char DGPSFrameTrait::caculation(unsigned char ch4, unsigned char ch7, unsigned char ch8) const
{
    //d8 ~ d1		ch0 ~ ch9
    unsigned char d30, ret;
    //滚动后字码2的d17 ~d21 滚动前 d5 d6 d1 d2 d3
    ret = 0;
    if ((ch7 & 0x10) > 0)
        ret += 0x10; //d17	滚动前ch7.d5
    if ((ch7 & 0x20) > 0)
        ret += 0x08; //d18	滚动前ch7.d6
    if ((ch8 & 0x01) > 0)
        ret += 0x04; //d19	滚动前ch8.d1
    if ((ch8 & 0x02) > 0)
        ret += 0x02; //d20	滚动前ch8.d2
    if ((ch8 & 0x04) > 0)
        ret += 0x01; //d21	滚动前ch8.d3
    //滚动后字码1的d30 滚动前 ch4.d6
    d30 = ch4 & 0x20;
    if (d30 > 0)
        ret = 31 - ret;
    ret = (ret + 2) * 5;
    return ret;
}

#include "seatassignframetrait.h"

using namespace MMC;

SeatAssignFrameTrait::SeatAssignFrameTrait(
    FrameStructureIDEnum id,
    const vector<unsigned char>& paramHeader)
    : FrameTrait(id, 0, paramHeader)
{
    this->bestPartialSize = defaultPartialSize;
}

SeatAssignFrameTrait::~SeatAssignFrameTrait() {}

SeatAssignFrameTrait::SeatAssignFrameTrait(const SeatAssignFrameTrait& org)
    : FrameTrait(org.GetId(), 0, org.GetHeader())
{
    trait = org.GetTrait();
    bestPartialSize = defaultPartialSize;
}

SeatAssignFrameTrait& SeatAssignFrameTrait::operator=(const SeatAssignFrameTrait& org)
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

FrameTrait* SeatAssignFrameTrait::myClone() const
{
    FrameTrait* p = new SeatAssignFrameTrait(*this);
    return p;
}

FrameTrait::ThreeState SeatAssignFrameTrait::isTraitOk(const vector<unsigned char>* partialFrame) const
{
    if (partialFrame->size() >= bestPartialSize)
        return Success;
    else
        return Unknown;
}
unsigned int SeatAssignFrameTrait::getVarLength(const vector<unsigned char>* partialFrame) const
{
    unsigned int tempLength = 0;
    if (partialFrame->size() >= bestPartialSize) {
        tempLength = caculation(partialFrame->at(18), partialFrame->at(19));
    } else {
        tempLength = this->GetHeader().size();
    }
    return tempLength;
}
unsigned int SeatAssignFrameTrait::caculation(unsigned char ch19, unsigned char ch20) const
{
    unsigned int ret = ch19;
    ret = ret << 8;
    ret += ch20;
    return ret;
}

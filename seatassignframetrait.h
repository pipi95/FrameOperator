#ifndef SEATASSIGNFRAMETRAIT_H
#define SEATASSIGNFRAMETRAIT_H

#include "frameoperator_global.h"
#include "mmcFrameStructure.h"
namespace MMC {

class FRAMEOPERATORSHARED_EXPORT SeatAssignFrameTrait : public FrameTrait {
public:
    SeatAssignFrameTrait(FrameStructureIDEnum id,
        const vector<unsigned char>& paramHeader);
    SeatAssignFrameTrait(const SeatAssignFrameTrait& org); // left for sample
    SeatAssignFrameTrait& operator=(const SeatAssignFrameTrait& org); // left for sample
    virtual ~SeatAssignFrameTrait();

protected:
    virtual ThreeState isTraitOk(const vector<unsigned char>* partialFrame) const override;
    virtual unsigned int getVarLength(const vector<unsigned char>* partialFrame) const override;
    virtual FrameTrait* myClone() const override;

private:
    static const unsigned int defaultPartialSize = 20;
    unsigned int caculation(unsigned char ch19, unsigned char ch20) const;
};
}
#endif // SEATASSIGNFRAMETRAIT_H

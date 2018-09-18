#ifndef DGPSFRAMETRAIT_H
#define DGPSFRAMETRAIT_H

#include "frameoperator_global.h"
#include "mmcFrameStructure.h"

namespace MMC {
class FRAMEOPERATORSHARED_EXPORT DGPSFrameTrait : public FrameTrait {
public:
    DGPSFrameTrait(FrameStructureIDEnum id,
        const vector<unsigned char>& paramHeader);
    DGPSFrameTrait(const DGPSFrameTrait& org); // left for sample
    DGPSFrameTrait& operator=(const DGPSFrameTrait& org); // left for sample
    virtual ~DGPSFrameTrait();

protected:
    virtual ThreeState isTraitOk(const vector<unsigned char>* partialFrame) const override;
    virtual unsigned int getVarLength(const vector<unsigned char>* partialFrame) const override;
    virtual FrameTrait* myClone() const override;

private:
    static const unsigned int defaultPartialSize = 8;
    unsigned char caculation(unsigned char ch4, unsigned char ch7, unsigned char ch8) const;
};
}
#endif // DGPSFRAMETRAIT_H

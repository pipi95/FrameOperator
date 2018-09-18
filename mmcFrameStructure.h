#ifndef mmcFramestructure_INC
#define mmcFramestructure_INC

#include "equipframe.h"
#include "frameoperator_global.h"
#include "mmcTemplateClass.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif // -----  not __CPLUSPLUS  -----

namespace MMC {
    using namespace FrameOperatorSpace;
    class FRAMEOPERATORSHARED_EXPORT FrameTrait {
    public:
        typedef enum {
            Success,
            Failed,
            Unknown // 判断条件不足
        } ThreeState;

        FrameTrait(
            FrameStructureIDEnum id,
            unsigned int paramLength,
            const vector<unsigned char>& paramHeader);
        FrameTrait(
            FrameStructureIDEnum id,
            unsigned int paramLength,
            const vector<unsigned char>& paramHeader,
            const vector<unsigned char>& paramTriat);
        FrameTrait(
            FrameStructureIDEnum id,
            unsigned int paramLength,
            unsigned int paramBestPartialSize,
            const vector<unsigned char>& paramHeader,
            const vector<unsigned char>& paramTriat);
        FrameTrait();
        FrameTrait(const FrameTrait& org); // left for sample
        FrameTrait& operator=(const FrameTrait& org); // left for sample
        unsigned int Length(const vector<unsigned char>* partialFrame = NULL) const;
        ThreeState IsMe(const vector<unsigned char>* partialFrame) const;
        unsigned int BestPartialSize() const; // best partial size is big enough to check header and trait
        vector<unsigned char> GetTrait() const;
        vector<unsigned char> GetHeader() const;
        FrameStructureIDEnum GetId() const;
        virtual ~FrameTrait();
        FrameTrait* Clone() const; // cloned pointer must be delete by customer
    protected:
        bool isHeaderOk(const vector<unsigned char>* partialFrame) const;
        virtual ThreeState isTraitOk(const vector<unsigned char>* partialFrame) const; // 由传入的字节来判断是否属于本帧数据
        virtual unsigned int getVarLength(const vector<unsigned char>* partialFrame) const; // 不足以判断帧长时也返回-1
        virtual FrameTrait* myClone() const;
        unsigned int length; // 0时为变帧长，默认实现第3个字节为帧长字节
        unsigned int bestPartialSize;
        vector<unsigned char> header; // header size = 0 代表为无效trait
        vector<unsigned char> trait;
        FrameStructureIDEnum id;
    };

    class FRAMEOPERATORSHARED_EXPORT FrameIDLib // 兼做frametrait的工厂
    {
    public:
        FrameIDLib();
        FrameIDLib(const FrameIDLib& org);
        FrameIDLib& operator=(const FrameIDLib& org);
        ~FrameIDLib();
        bool Contains(FrameStructureIDEnum id) const;
        FrameTrait* CloneTrait(FrameStructureIDEnum id) const; // 获取的新对象，需要客户手动析构该对象
        vector<FrameStructureIDEnum> GetAllId() const;

    private:
        vector<FrameTrait*>* lib;
        void insertTrait(
            FrameStructureIDEnum id,
            unsigned int length,
            const string& arrayHeader); // 请注意传入带'\0'的字符串时，采用+=方式为字符串添加‘'\0'字符！
        void insertTrait(
            FrameStructureIDEnum id,
            unsigned int length,
            const string& arrayHeader,
            const string& arrayTrait);
        void insertDGPSTrait(
            FrameStructureIDEnum id,
            const string& arrayHeader);
        void insertSeatAssignTrait(
            FrameStructureIDEnum id,
            const string& arrayHeader);
    };
}
#ifdef __cplusplus
}
#endif // -----  not __CPLUSPLUS  -----
#endif // ----- #ifndef mmcFramestructure_INC  -----

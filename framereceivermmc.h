#ifndef FRAMERECEIVERMMC_H
#define FRAMERECEIVERMMC_H

#include "frameoperator_global.h"
#include "mmcFrameFinder.h"
#include <QIODevice>
#include <QObject>
#include <QTimer>
#include <vector>

namespace FrameOperatorSpace {

using namespace MMC;
class FRAMEOPERATORSHARED_EXPORT FrameReceiverMMC : public QObject {
    Q_OBJECT
public:
    enum SourceType : quint8 {
        SerialportType,
        MulticastType,
        FileType,
        TcpType
    };
    FrameReceiverMMC(
        SourceType type,
        QIODevice* device,
        const vector<FrameTrait*>& frTraits,
        QObject* parent = nullptr);
    ~FrameReceiverMMC();
    bool IsSourceValid() { return isSourceValid; }
signals:
    void FrameIsFound(const QList<EquipFrame>& frames);
public slots:
    void Start();
    void Stop();

private:
    FrameFinder* framefinder;
    bool isSourceValid;
    bool isRunning;
    QIODevice* sourceDevice;
    SourceType sourceType;
    void readNewData();
    void appendToByteDataBuffer(const QByteArray& data);
    QTimer timer;
    void findFrames();
};
}
#endif // FRAMERECEIVERMMC_H

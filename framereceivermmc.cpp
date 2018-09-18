#include "framereceivermmc.h"
#include <QtNetwork>
#include <algorithm>

namespace FrameOperatorSpace {

FrameReceiverMMC::FrameReceiverMMC(
    SourceType type,
    QIODevice* device,
    const vector<FrameTrait*>& frTraits,
    QObject* parent)
    : QObject(parent)
    , isSourceValid(false)
    , isRunning(false)
    , sourceDevice(device)
    , sourceType(type)
{
    if (device != nullptr) {
        device->setParent(this);
        switch (type) {
        case MulticastType:
            isSourceValid = static_cast<QUdpSocket*>(device)->isValid();
            break;
        default:
            isSourceValid = sourceDevice->isReadable();
            break;
        }
    } else {
        isSourceValid = false;
    }
    framefinder = new FrameFinder(frTraits);
    connect(this, &FrameReceiverMMC::destroyed, this, &FrameReceiverMMC::Stop);
}
FrameReceiverMMC::~FrameReceiverMMC()
{
    delete framefinder;
}
void FrameReceiverMMC::Start()
{
    if (!isRunning) {
        isRunning = true;
        switch (sourceType) {
        case MulticastType:
        case SerialportType:
            connect(sourceDevice, &QIODevice::readyRead, this, &FrameReceiverMMC::readNewData);
            break;
        case FileType:
            if (isSourceValid) {
                sourceDevice->seek(0);
                timer.setInterval(100);
                connect(&timer, &QTimer::timeout, this, &FrameReceiverMMC::readNewData);
                timer.start();
            }
            break;
        default:
            break;
        }
    }
}
void FrameReceiverMMC::Stop()
{
    if (isRunning) {
        isRunning = false;
 //       sourceDevice->disconnect();
        if (sourceType == FileType) {
            sourceDevice->disconnect();
            timer.stop();
            timer.disconnect();
        }
    }
}
void FrameReceiverMMC::readNewData()
{
    if (isSourceValid && sourceDevice->bytesAvailable() > 0) {
        switch (sourceType) {
        case FileType:
            if (sourceDevice->isOpen()) {
                appendToByteDataBuffer(sourceDevice->read(2048));
                if (dynamic_cast<QFile*>(sourceDevice)->atEnd()) {
                    Stop();
                }
            }
            break;
        case MulticastType:
            appendToByteDataBuffer(dynamic_cast<QUdpSocket*>(sourceDevice)->receiveDatagram().data());
            break;
        case SerialportType:
            appendToByteDataBuffer(sourceDevice->readAll());
            break;
        default:
            break;
        }
        findFrames();
    }
}
void FrameReceiverMMC::findFrames()
{
    vector<EquipFrame> frames;
    framefinder->GetFrames(&frames);
    if(frames.size() > 0)
    {
        QList<EquipFrame> qlframes = QList<EquipFrame>::fromVector(QVector<EquipFrame>::fromStdVector(frames));
        emit FrameIsFound(qlframes);
    }
}
void FrameReceiverMMC::appendToByteDataBuffer(const QByteArray& data)
{
    vector<unsigned char> vData;
    copy(data.begin(), data.end(), back_inserter(vData));
    framefinder->InBuffer(vData);
}
}

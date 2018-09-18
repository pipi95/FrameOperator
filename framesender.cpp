#include "framesender.h"
#include <QUdpSocket>
#include <QDebug>

namespace FrameOperatorSpace {

FrameSender::FrameSender(SourceType type, QIODevice* device, QObject* parent)
    : QObject(parent)
    , sourceType(type)
{
    source = device;
    isSourceValid = device->isWritable();
}

bool FrameSender::IsSourceValid()
{
    return isSourceValid;
}
void FrameSender::Send(const EquipFrame& fr)
{
    Send(fr.GetContent());
}

void FrameSender::Send(const QByteArray& content)
{
    switch(sourceType)
    {
    case SerialportType:
    case TcpType:
        if (isSourceValid) {
            source->write(content);
        }
        break;
    case MulticastType:
 //       quint8 len = source->write(content);
       qDebug() <<dynamic_cast<QUdpSocket*>(source)->writeDatagram( content,QHostAddress("227.0.81.22"),37101);
        break;
    default:
        break;

    }
    if (isSourceValid) {
        source->write(content);
    }
}
FrameSender::~FrameSender()
{
}
}

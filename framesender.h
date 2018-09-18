#ifndef FRAMESENDER_H
#define FRAMESENDER_H

#include "equipframe.h"
#include "frameoperator_global.h"
#include <QHostAddress>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtSerialPort/QSerialPort>

namespace FrameOperatorSpace {

class FRAMEOPERATORSHARED_EXPORT FrameSender : public QObject {
    Q_OBJECT
public:
    enum SourceType {
        SerialportType,
        TcpType,
        MulticastType
    };
    FrameSender(SourceType type, QIODevice* device, QObject* parent = nullptr);
    bool IsSourceValid();
    ~FrameSender();
signals:

public slots:
    void Send(const EquipFrame& fr);
    void Send(const QByteArray& content);
private:
    bool isSourceValid;
    SourceType sourceType;
    QIODevice* source;
};
}
#endif // FRAMESENDER_H

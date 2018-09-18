#include "equipframe.h"
#include <QMetaEnum>
#include <QtDebug>

namespace FrameOperatorSpace {

EquipFrame::EquipFrame()
    : id(ID_Fr_Invalid)
{
}

EquipFrame::EquipFrame(const QByteArray& content, FrameStructureIDEnum id)
    : content(content)
    , id(id)
{
}

EquipFrame::EquipFrame(const EquipFrame& fr)
    : EquipFrame(fr.GetContent(), fr.GetID())
{
}

void EquipFrame::Print() const
{
    qDebug() << ToString();
}

EquipFrame& EquipFrame::operator=(const EquipFrame& fr)
{
    this->content = fr.GetContent();
    this->id = fr.GetID();
    return *this;
}

QString EquipFrame::ToString() const
{
    QMetaEnum me = QMetaEnum::fromType<FrameStructureIDEnum>();
    return QString("%1:%2")
        .arg(me.valueToKey(id))
        .arg(content.toHex(' ').data());
}

QByteArray EquipFrame::GetContent() const
{
    return content;
}
FrameStructureIDEnum EquipFrame::GetID() const
{
    return id;
}

void EquipFrame::SetContent(const QByteArray& content)
{
    this->content = content;
}
void EquipFrame::SetID(FrameStructureIDEnum id)
{
    this->id = id;
}
}

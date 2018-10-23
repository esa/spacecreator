/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QGraphicsObject>

class QGraphicsLineItem;
class QGraphicsPolygonItem;
class QGraphicsTextItem;

namespace msc {
class InstanceItem;
class MscMessage;

class MessageItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit MessageItem(MscMessage *message, QGraphicsItem *parent = nullptr);

    void setSourceInstanceItem(InstanceItem *sourceInstance);
    void setTargetInstanceItem(InstanceItem *targetInstance);

    QString name() const;

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public Q_SLOTS:
    void updateLayout();
    void setName(const QString &name);

private Q_SLOTS:
    void buildLayout();

private:
    void setWidth(double width);
    void centerName();

    msc::MscMessage *m_message = nullptr;
    QGraphicsLineItem *m_line = nullptr;
    QGraphicsPolygonItem *m_leftArrow = nullptr;
    QGraphicsPolygonItem *m_rightArrow = nullptr;
    QGraphicsTextItem *m_nameItem = nullptr;
    InstanceItem *m_sourceInstance = nullptr;
    InstanceItem *m_targetInstance = nullptr;
    bool m_layoutDirty = false;
};

} // namespace mas

#endif // MESSAGEITEM_H

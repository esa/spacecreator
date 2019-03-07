/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "baseitems/interactiveobject.h"

#include <QGraphicsItem>
#include <QObject>
#include <QPointF>
#include <QPointer>

namespace msc {

class ObjectAnchor : public QObject
{
    Q_OBJECT
public:
    enum class Snap
    {
        SnapTo,
        NoSnap
    };
    ObjectAnchor(QObject *parent = nullptr);
    ObjectAnchor(InteractiveObject *anchorObject, const QPointF &scenePoint, QObject *parent = nullptr);
    ObjectAnchor(const ObjectAnchor &other);
    ~ObjectAnchor();

    InteractiveObject *object() const;
    bool setObject(InteractiveObject *object);

    QPointF point() const;
    bool setPoint(const QPointF &scenePoint, ObjectAnchor::Snap snap);

    bool replace(InteractiveObject *object, const QPointF &scenePoint, ObjectAnchor::Snap snap);

    QGraphicsItem::GraphicsItemFlags originalFlags() const;

    bool operator==(const ObjectAnchor &other) const;

Q_SIGNALS:
    void anchorChanged(InteractiveObject *object, const QPointF &scenePoint) const;

private:
    ObjectAnchor &operator=(const ObjectAnchor &other) = delete;

    QPointer<InteractiveObject> m_anchorObject = nullptr;
    QPointF m_scenePoint;
    QGraphicsItem::GraphicsItemFlags m_storedFlags;

    void notifyChanged() const;
    void connectObject();
    void disconnectObject();
};

} // ns msc

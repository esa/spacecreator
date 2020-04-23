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
#include "objectanchor.h"

#include "baseitems/common/utils.h"
#include "baseitems/interactiveobject.h"

namespace msc {

/*!
  \class msc::ObjectAnchor
  \brief Handler for an object and its point (scene coordinates).

  \inmodule MscWidgets

*/

ObjectAnchor::ObjectAnchor(QObject *parent)
    : QObject(parent)
{
}

ObjectAnchor::ObjectAnchor(InteractiveObject *anchorObject, const QPointF &scenePoint, QObject *parent)
    : QObject(parent)
    , m_anchorObject(anchorObject)
    , m_scenePoint(scenePoint)
{
    connectObject();
}

ObjectAnchor::ObjectAnchor(const ObjectAnchor &other)
    : QObject(other.parent())
    , m_anchorObject(other.m_anchorObject)
    , m_scenePoint(other.m_scenePoint)
    , m_storedFlags(other.m_storedFlags)
{
    if (m_anchorObject) {
        m_anchorObject->highlightConnected();
    }
}

ObjectAnchor::~ObjectAnchor()
{
    if (m_anchorObject) {
        m_anchorObject->setFlags(m_storedFlags);
        m_anchorObject = nullptr;
    }
}

QGraphicsItem::GraphicsItemFlags ObjectAnchor::originalFlags() const
{
    return m_storedFlags;
}

bool ObjectAnchor::operator==(const ObjectAnchor &other) const
{
    return m_anchorObject == other.m_anchorObject && m_scenePoint == other.m_scenePoint
            && m_storedFlags == other.m_storedFlags;
}

InteractiveObject *ObjectAnchor::object() const
{
    return m_anchorObject;
}

bool ObjectAnchor::setObject(InteractiveObject *object)
{
    if (object == m_anchorObject)
        return false;

    disconnectObject();
    m_anchorObject = object;
    connectObject();

    notifyChanged();

    return true;
}

QPointF ObjectAnchor::point() const
{
    return m_scenePoint;
}

bool ObjectAnchor::setPoint(const QPointF &scenePoint, ObjectAnchor::Snap snap)
{
    if (scenePoint == m_scenePoint && snap == ObjectAnchor::Snap::NoSnap)
        return false;

    m_scenePoint = scenePoint;
    if (ObjectAnchor::Snap::SnapTo == snap && m_anchorObject) {
        const QRectF objectSceneRect = m_anchorObject->sceneBoundingRect();
        if (objectSceneRect.contains(m_scenePoint))
            m_scenePoint.setX(objectSceneRect.center().x());
    }
    notifyChanged();
    return true;
}

void ObjectAnchor::notifyChanged() const
{
    Q_EMIT anchorChanged(m_anchorObject, m_scenePoint);
}

bool ObjectAnchor::replace(InteractiveObject *object, const QPointF &scenePoint, ObjectAnchor::Snap snap)
{
    bool changed(false);

    {
        QSignalBlocker block(this);
        const bool objectChanged = setObject(object);
        const bool anchorChanged = setPoint(scenePoint, snap);
        changed = objectChanged || anchorChanged;
    }

    if (changed)
        notifyChanged();

    return changed;
}

void ObjectAnchor::connectObject()
{
    if (m_anchorObject) {
        m_storedFlags = m_anchorObject->flags();
        m_anchorObject->setFlags(m_storedFlags | QGraphicsItem::ItemSendsGeometryChanges
                                 | QGraphicsItem::ItemSendsScenePositionChanges);
        m_anchorObject->highlightConnected();
    }
}

void ObjectAnchor::disconnectObject()
{
    if (m_anchorObject) {
        m_anchorObject->highlightDisconnected();
        m_anchorObject->setFlags(m_storedFlags);
        m_anchorObject = nullptr;
    }
}

} // ns msc

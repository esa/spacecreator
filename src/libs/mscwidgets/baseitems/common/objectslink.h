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

#include "objectanchor.h"

#include <QGraphicsItem>
#include <QPointF>

namespace msc {

class InteractiveObject;
class ObjectsLink : public QObject
{
    Q_OBJECT
public:
    ObjectsLink(QObject *parent = nullptr);
    virtual ~ObjectsLink();

    QPointF makeLink(InteractiveObject *source, const QPointF &sourceAnchorPoint, InteractiveObject *target,
            const QPointF &targetAnchorPoint);
    bool replaceSource(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap);
    bool replaceTarget(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap);

    QPointF linkCenter() const;

    ObjectAnchor *source() const;
    ObjectAnchor *target() const;

Q_SIGNALS:
    void sourceAnchorChanged(const QPointF &scenePoint) const;
    void targetAnchorChanged(const QPointF &scenePoint) const;

protected:
    ObjectAnchor *m_anchorStart, *m_anchorEnd;

    bool replaceObj(ObjectAnchor *anchor, InteractiveObject *toObject, const QPointF &toPoint, ObjectAnchor::Snap snap);

    void notifyChanged(ObjectAnchor *anchor) const;
};

} // ns msc

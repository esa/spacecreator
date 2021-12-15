/*
 Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "itemeditor/ivfunctiongraphicsitem.h"
#include "miniviewrenderer.h"

namespace ive {

class End2EndFunctionGraphicsItem : public IVFunctionGraphicsItem
{
    Q_OBJECT
public:
    explicit End2EndFunctionGraphicsItem(ivm::IVFunction *entity, QGraphicsItem *parent = nullptr);
    void setEnd2EndConnections(const QList<shared::Id> &connections);
};

class End2EndMiniViewRenderer : public MiniViewRenderer
{
public:
    explicit End2EndMiniViewRenderer(const End2EndFunctionGraphicsItem *item);
    void setEnd2EndConnections(const QList<shared::Id> &connections);
    void render(QPainter *painter) override;
    QPolygonF getPolygon(const QPair<shared::Id, shared::Id> connections) const;

private:
    QList<shared::Id> m_connections;
};

} // namespace ive

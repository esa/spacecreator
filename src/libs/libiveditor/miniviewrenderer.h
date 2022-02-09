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

#include "common.h"

#include <QHash>
#include <QList>
#include <QPointF>
#include <QRectF>
#include <memory>

class QPainter;

namespace ive {
class IVFunctionGraphicsItem;
struct MiniViewRendererPrivate;

class MiniViewRenderer
{
public:
    explicit MiniViewRenderer(const IVFunctionGraphicsItem *item);
    ~MiniViewRenderer();

    virtual void render(QPainter *painter);

private:
    void clear();
    void updateData();
    void drawData(QPainter *painter);

protected:
    //    struct ConnectionData {
    //        QPointF outerMappedScenePos;
    //        QPointF innerScenePos;
    //        shared::Id innerFunctionId;
    //        shared::Id connectionId;
    //    };

    struct MiniViewRendererPrivate {
        explicit MiniViewRendererPrivate(const IVFunctionGraphicsItem *item)
            : item(item)
        {
        }

        const IVFunctionGraphicsItem *item { nullptr };

        QHash<shared::Id, QRectF> rects;
        QHash<shared::Id, QPolygonF> polygons;
    };
    const std::unique_ptr<MiniViewRendererPrivate> d;
};

} // namespace ive

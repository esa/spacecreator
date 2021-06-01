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

#include "colors/colormanager.h"
#include "dvconnection.h"
#include "ui/verectgraphicsitem.h"

class QGraphicsItem;

namespace dve {
class DVDeviceGraphicsItem;

class DVConnectionGraphicsItem : public shared::ui::VEInteractiveObject
{
    Q_OBJECT
public:
    explicit DVConnectionGraphicsItem(dvm::DVConnection *connection, DVDeviceGraphicsItem *startItem,
            DVDeviceGraphicsItem *endItem, QGraphicsItem *parent = nullptr);
    ~DVConnectionGraphicsItem() override;
    enum
    {
        Type = UserType + static_cast<int>(dvm::DVObject::Type::Connection)
    };

    dvm::DVConnection *entity() const override;
    void init() override;

    DVDeviceGraphicsItem *startItem() const;
    DVDeviceGraphicsItem *endItem() const;

    void setPoints(const QVector<QPointF> &points);

    int type() const override { return Type; }
    int itemLevel(bool isSelected) const override;
    void updateFromEntity() override;
    void layout();

protected Q_SLOTS:
    void applyColorScheme() override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    shared::ColorManager::HandledColors handledColorType() const override;

private:
    DVDeviceGraphicsItem *m_startItem { nullptr };
    DVDeviceGraphicsItem *m_endItem { nullptr };
    QPolygonF m_points;
};

} // namespace dve

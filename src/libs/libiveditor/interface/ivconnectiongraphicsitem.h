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

#include "ivconnection.h"
#include "ivobject.h"
#include "ui/veconnectiongraphicsitem.h"

#include <QPointer>

namespace ive {

class IVInterfaceGraphicsItem;
class IVFunctionGraphicsItem;

class IVConnectionGraphicsItem : public shared::ui::VEConnectionGraphicsItem
{
    Q_OBJECT
public:
    enum
    {
        Type = UserType + static_cast<int>(ivm::IVObject::Type::Connection)
    };

    int type() const override { return Type; }

    explicit IVConnectionGraphicsItem(ivm::IVConnection *connection, IVInterfaceGraphicsItem *ifaceStart,
            IVInterfaceGraphicsItem *ifaceEnd, QGraphicsItem *parent = nullptr);
    ~IVConnectionGraphicsItem() override;

    ivm::IVConnection *entity() const override;
    QPainterPath shape() const override;
    QRectF boundingRect() const override;

    QString prepareTooltip() const override;

    int itemLevel(bool isSelected) const override;

protected:
    shared::ColorManager::HandledColors handledColorType() const override;

protected Q_SLOTS:
    void applyColorScheme() override;
    void rebuildLayout() override;
};

}

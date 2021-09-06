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
#include "dvdevice.h"
#include "ui/veconnectionendpointgraphicsitem.h"

#include <QPointer>

class QGraphicsItem;

namespace shared {
namespace ui {
class TextItem;
}
}

namespace dve {
class DVNodeGraphicsItem;
class DVConnectionGraphicsItem;

class DVDeviceGraphicsItem : public shared::ui::VEConnectionEndPointGraphicsItem
{
    Q_OBJECT
public:
    explicit DVDeviceGraphicsItem(dvm::DVDevice *device, QGraphicsItem *parent = nullptr);
    ~DVDeviceGraphicsItem() override;
    enum
    {
        Type = UserType + static_cast<int>(dvm::DVObject::Type::Device)
    };

    dvm::DVDevice *entity() const override;

    int type() const override { return Type; }
    int itemLevel(bool isSelected) const override;

    QPointF connectionEndPoint(const bool nestedConnection) const override;
    QPointF connectionEndPoint(shared::ui::VEConnectionGraphicsItem *connection = nullptr) const override;

    DVNodeGraphicsItem *nodeItem() const;

protected Q_SLOTS:
    void applyColorScheme() override;

protected:
    void rebuildLayout() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    shared::ColorManager::HandledColors handledColorType() const override;
    void updateInternalItems(Qt::Alignment alignment) override;
    void updateTextPosition() override;

    QList<QPair<Qt::Alignment, QPainterPath>> sidePaths() const override;
    shared::graphicsviewutils::LookupDirection lookupType() const override;
    shared::ui::TextItem *initTextItem() override;
    void updateEntityTitle(const QString &text);
};

} // namespace dve

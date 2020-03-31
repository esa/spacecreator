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
  along with this program. If not, see
  <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "baseitems/interactiveobject.h"
#include "tab_aadl/aadlobject.h"

#include <QPointer>

namespace taste3 {
namespace aadl {
class AADLObjectIface;
class AADLConnectionGraphicsItem;
class AADLFunctionGraphicsItem;

class AADLInterfaceGraphicsItem : public InteractiveObject
{
    Q_OBJECT
public:
    explicit AADLInterfaceGraphicsItem(AADLObjectIface *entity, QGraphicsItem *parent = nullptr);
    enum
    {
        Type = UserType + static_cast<int>(AADLObject::Type::RequiredInterface) // provided?
    };

    AADLObjectIface *entity() const;

    int type() const override { return Type; }
    void addConnection(AADLConnectionGraphicsItem *item);
    void removeConnection(AADLConnectionGraphicsItem *item);
    QList<QPointer<AADLConnectionGraphicsItem>> connectionItems() const;

    AADLFunctionGraphicsItem *targetItem() const;
    void setTargetItem(QGraphicsItem *item, const QPointF &globalPos);

    void setInterfaceName(const QString &name);

    QPainterPath shape() const override;

    void updateFromEntity() override;
    QList<QVariantList> prepareChangeCoordinatesCommandParams() const override;

    QString prepareTooltip() const override;
    void layout();

protected:
    void rebuildLayout() override;
    void onSelectionChanged(bool isSelected) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(GripPoint *grip, const QPointF &from, const QPointF &to) override;

    void updateInternalItems(Qt::Alignment alignment);
    virtual ColorManager::HandledColors handledColorType() const override;
    void adjustItem();

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

protected Q_SLOTS:
    virtual void applyColorScheme() override;
    void updateLabel();
    void updateKind();
    void onAttrOrPropChanged(taste3::aadl::meta::Props::Token t);

private:
    QGraphicsPathItem *m_type = nullptr;
    QGraphicsPathItem *m_iface = nullptr;
    QGraphicsTextItem *m_text = nullptr;
    QList<QPointer<AADLConnectionGraphicsItem>> m_connections;
    QPainterPath m_shape;

    QString ifaceLabel() const;

    QPainterPath composeShape() const;
};

} // namespace aadl
} // namespace taste3

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

#include "aadlfunctiontypegraphicsitem.h"

class QSvgRenderer;

namespace aadl {
class AADLObjectFunction;
}

namespace aadlinterface {

class AADLFunctionGraphicsItem : public AADLFunctionTypeGraphicsItem
{
    Q_OBJECT
public:
    explicit AADLFunctionGraphicsItem(aadl::AADLObjectFunction *entity, QGraphicsItem *parent = nullptr);
    enum
    {
        Type = UserType + static_cast<int>(aadl::AADLObject::Type::Function)
    };

    aadl::AADLObjectFunction *entity() const;

    int type() const override { return Type; }

    QPainterPath shape() const override;

    bool isRootItem() const;

    QString prepareTooltip() const override;

protected:
    void rebuildLayout() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

    void onManualResizeProgress(GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualResizeFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt) override;
    void onManualMoveProgress(GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt) override;

    void prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const override;

    virtual ColorManager::HandledColors handledColorType() const override;

protected Q_SLOTS:
    void applyColorScheme() override;

private:
    Q_INVOKABLE void updateNestedIcon();

    enum ConnectionLayoutPolicy
    {
        IgnoreCollisions = 0,
        RebuildOnCollision
    };
    void layoutConnectionsOnMove(ConnectionLayoutPolicy layoutPolicy);
    void layoutConnectionsOnResize();

private:
    static QPointer<QSvgRenderer> m_svgRenderer;
    bool m_hasNestedItems = false;
};

}

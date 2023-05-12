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

#include "ivfunction.h"
#include "ivfunctiontypegraphicsitem.h"

class QSvgRenderer;

namespace shared {
namespace ui {
class TextItem;
}
}

namespace ive {
class IVInterfaceGraphicsItem;
class MiniViewRenderer;

class IVFunctionGraphicsItem : public IVFunctionTypeGraphicsItem
{
    Q_OBJECT
public:
    explicit IVFunctionGraphicsItem(ivm::IVFunction *entity, QGraphicsItem *parent = nullptr);
    ~IVFunctionGraphicsItem() override;

    enum
    {
        Type = UserType + static_cast<int>(ivm::IVObject::Type::Function)
    };

    void init() override;

    ivm::IVFunction *entity() const override;

    int type() const override { return Type; }

    QPainterPath shape() const override;

    /**
     * @brief resizedRect takes a GripPoint sitting on this VERectGraphicsItem and two
     * QPointFs representing the movement of the GripPoint and returns
     * a QRectF representing the new sceneBoundingRect of this VERectGraphicsItem based on
     * the allowed movement of that particular grip point.
     *
     * A corner GripPoint can move freely. A GripPoint on a horizontal line can only move
     * up or down. A GripPoint on a vertical line can only move left or right.
     *
     * If a transformation is illegal, a QRectF is calculated that performs as much as possible of the
     * transformation as is legal.
     *
     * Limits of movement: An IVFunctionGraphicsItem inherits its resizedRect from
     * IVFunctionTypeGraphicsItem which limits the resizing with respect to
     * interfaces (aka connection endpoints).
     *
     * FunctionTypeGraphicsItem adds respect to the Text displaying the FunctionName, so
     * that the FunctionName is always visible.
     *
     * FunctionGraphicsItem adds respect to the nested FunctionGraphicsItems it can contain.
     *
     * @param grip a GripPoint
     * @param from a QPointF the GripPoint was moved from
     * @param to a QPointF the GripPoint was moved to
     * @return a QRect representing the new sceneBoundingRect of this IVFunctionTypeGraphicsItem
     */
    virtual QRectF resizedRect(shared::ui::ResizeLimits resizeLimits) override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void onManualMoveProgress(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt) override;

    virtual shared::ColorManager::HandledColors handledColorType() const override;
    shared::ui::TextItem *initTextItem() override;

    explicit IVFunctionGraphicsItem(
            ivm::IVFunction *entity, std::unique_ptr<MiniViewRenderer> renderer, QGraphicsItem *parent = nullptr);

    std::unique_ptr<MiniViewRenderer> m_miniViewDrawer;

private:
    shared::ui::ResizeLimits resizedRectForNestedFunctions(shared::ui::ResizeLimits resizeLimits) const;
    QRectF boundingRectForNestedFunctions() const;

    static QPointer<QSvgRenderer> m_svgRenderer;
};
}

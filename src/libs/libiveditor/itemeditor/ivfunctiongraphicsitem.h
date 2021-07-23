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

#include "ivconnectiongraphicsitem.h"
#include "ivfunction.h"
#include "ivfunctiontypegraphicsitem.h"

class QSvgRenderer;

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

    ivm::IVFunction *entity() const override;

    int type() const override { return Type; }

    QPainterPath shape() const override;

    QString prepareTooltip() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void onManualMoveProgress(shared::ui::GripPoint *grip, const QPointF &from, const QPointF &to) override;
    void onManualMoveFinish(shared::ui::GripPoint *grip, const QPointF &pressedAt, const QPointF &releasedAt) override;

    void prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const override;

    virtual shared::ColorManager::HandledColors handledColorType() const override;

protected Q_SLOTS:
    void applyColorScheme() override;

private:
    static QPointer<QSvgRenderer> m_svgRenderer;
    std::unique_ptr<MiniViewRenderer> m_miniViewDrawer;
};
}

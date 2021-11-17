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

#include "ivfunctiontype.h"
#include "ivobject.h"
#include "ui/verectgraphicsitem.h"
#include "veobject.h"

namespace ive {

class IVFunctionNameGraphicsItem;

class IVFunctionTypeGraphicsItem : public shared::ui::VERectGraphicsItem
{
    Q_OBJECT
public:
    explicit IVFunctionTypeGraphicsItem(ivm::IVFunctionType *entity, QGraphicsItem *parentItem = nullptr);
    enum
    {
        Type = UserType + static_cast<int>(ivm::IVObject::Type::FunctionType)
    };
    int type() const override { return Type; }

    ivm::IVFunctionType *entity() const override;

    void enableEditMode() override;

    QSizeF minimalSize() const override;

    QString prepareTooltip() const override;

    bool isRootItem() const;

    int itemLevel(bool isSelected) const override;

    void init() override;

protected Q_SLOTS:
    void applyColorScheme() override;
    void updateNameFromUi(const QString &name);

protected:
    void rebuildLayout() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    shared::ColorManager::HandledColors handledColorType() const override;

    void updateTextPosition() override;

    virtual void prepareTextRect(QRectF &textRect, const QRectF &targetTextRect) const;

    shared::ui::TextItem *initTextItem() override;
};

}

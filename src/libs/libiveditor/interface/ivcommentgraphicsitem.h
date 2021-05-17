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

#include "ivcomment.h"
#include "ivobject.h"
#include "ui/verectgraphicsitem.h"

#include <QScopedPointer>

namespace ivm {
class IVComment;
}

namespace ive {

class IVCommentGraphicsItem : public shared::ui::VERectGraphicsItem
{
    Q_OBJECT

public:
    explicit IVCommentGraphicsItem(ivm::IVComment *comment, QGraphicsItem *parent = nullptr);
    enum
    {
        Type = UserType + static_cast<int>(ivm::IVObject::Type::Comment)
    };
    int type() const override { return Type; }

    void init() override;

    void setText(const QString &text);
    QString text() const;

    ivm::IVComment *entity() const override;
    QSizeF minimalSize() const override;
    void updateFromEntity() override;

    int itemLevel(bool isSelected) const override;

protected Q_SLOTS:
    virtual void applyColorScheme() override;

protected:
    void rebuildLayout() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual shared::ColorManager::HandledColors handledColorType() const override;

private:
    QString m_text;
};

}

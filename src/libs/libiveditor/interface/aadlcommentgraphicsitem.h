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
#include "aadlrectgraphicsitem.h"
#include "aadlobject.h"

#include <QScopedPointer>
#include <QTextLayout>

namespace aadl {
class AADLObjectComment;
}

namespace aadlinterface {

class AADLCommentGraphicsItem : public AADLRectGraphicsItem
{
    Q_OBJECT
public:
    explicit AADLCommentGraphicsItem(aadl::AADLObjectComment *comment, QGraphicsItem *parent = nullptr);
    enum
    {
        Type = UserType + static_cast<int>(aadl::AADLObject::Type::Comment)
    };
    int type() const override { return Type; }

    void setText(const QString &text);
    QString text() const;

    aadl::AADLObjectComment *entity() const;
    QSizeF minimalSize() const override;
    void updateFromEntity() override;

protected Q_SLOTS:
    virtual void applyColorScheme() override;

protected:
    void rebuildLayout() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual ColorManager::HandledColors handledColorType() const override;

private:
    void updateTextLayout(const QString &text);

private:
    QScopedPointer<QTextLayout> m_textLayout;
    QString m_text;
};

}

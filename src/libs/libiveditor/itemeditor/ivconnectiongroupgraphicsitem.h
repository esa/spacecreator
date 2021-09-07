/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

namespace ivm {
class IVConnectionGroup;
}

namespace ive {
class IVInterfaceGroupGraphicsItem;

class IVConnectionGroupGraphicsItem : public IVConnectionGraphicsItem
{
    Q_OBJECT
public:
    explicit IVConnectionGroupGraphicsItem(ivm::IVConnectionGroup *connection,
            IVInterfaceGroupGraphicsItem *ifaceStart, IVInterfaceGroupGraphicsItem *ifaceEnd,
            QGraphicsItem *parent = nullptr);

    shared::ColorManager::HandledColors handledColorType() const override;
    void init() override;

protected:
    void updateBoundingRect() override;

private Q_SLOTS:
    void updateLabel(const QString &text);

private:
    void updateTitleItemPosition();
};

} // namespace ive

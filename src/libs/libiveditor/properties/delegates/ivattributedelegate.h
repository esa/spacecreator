/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "interface/attributedelegate.h"

#include <QPointer>

namespace ivm {
class IVObject;
}

namespace ive {

/**
 * Delegate for Qt views to handle all kind of attributes of IV objects. Including the special needs for IV names.
 */
class IVAttributeDelegate : public shared::AttributeDelegate
{
public:
    explicit IVAttributeDelegate(ivm::IVObject *object, QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QPointer<ivm::IVObject> m_object;
};

} // namespace ive

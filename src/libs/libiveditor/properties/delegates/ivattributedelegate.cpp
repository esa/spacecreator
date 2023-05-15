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

#include "ivattributedelegate.h"
#include "propertieslistmodel.h"

#include "ivobject.h"
#include "properties/qivnamevalidator.h"
#include "propertiesmodelbase.h"

#include <QLineEdit>

namespace ive {

IVAttributeDelegate::IVAttributeDelegate(ivm::IVObject *object, QObject *parent)
    : shared::AttributeDelegate { parent }
    , m_object(object)
{
}

QWidget *IVAttributeDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QWidget *editor = shared::AttributeDelegate::createEditor(parent, option, index);
    if (auto lineEdit = qobject_cast<QLineEdit *>(editor)) {
        if (index.data(shared::PropertiesModelBase::TokenRole).value<ivm::meta::Props::Token>()
            == ivm::meta::Props::Token::name) {
            auto validator = new QIVNameValidator(m_object, editor);
            validator->setSecondValidator(lineEdit->validator());
            lineEdit->setValidator(validator);
        }
        // disable validator for name for connection groups
        /// @todo fix properly #issue 978
        if (m_object->type() == ivm::IVObject::Type::ConnectionGroup && index.row() == 0
            && index.column() == shared::PropertiesListModel::Column::Value) {
            lineEdit->setValidator(nullptr);
        }
    }
    return editor;
}

} // namespace ive

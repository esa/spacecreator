/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "propertytypedelegate.h"

#include "app/datatypes/datatypesstorage.h"
#include "tab_aadl/aadlparameter.h"

#include <QComboBox>
#include <QStringListModel>

namespace taste3 {
namespace aadl {

PropertyTypeDelegate::PropertyTypeDelegate(QObject *parent)
    : ComboBoxDelegate(parent)
    , m_model(new QStringListModel(this))
{
    const QMap<QString, datatypes::BasicDataType *> types = datatypes::DataTypesStorage::dataTypes();
    QStringList names;
    for (auto type : types)
        names.append(type->name());

    names.append(BasicParameter::typeName(BasicParameter::Type::Timer));
    names.append(BasicParameter::typeName(BasicParameter::Type::Directive));

    m_model->setStringList(names);
}

PropertyTypeDelegate::~PropertyTypeDelegate() {}

QAbstractItemModel *PropertyTypeDelegate::editorModel(const QModelIndex & /*id*/) const
{
    return m_model;
}

} // ns aadl
} // ns taste3

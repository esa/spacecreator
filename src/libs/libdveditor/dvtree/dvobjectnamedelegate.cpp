/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvobjectnamedelegate.h"

#include "abstractvisualizationmodel.h"
#include "common.h"
#include "dvmodel.h"
#include "dvobject.h"
#include "properties/qdvnamevalidator.h"
#include "ui/errorindicatinglineedit.h"

#include <QPointer>
#include <QValidator>

namespace dve {

DVObjectNameDelegate::DVObjectNameDelegate(dvm::DVModel *model, QObject *parent)
    : QStyledItemDelegate { parent }
    , m_model(model)
{
}

QWidget *DVObjectNameDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto lineEdit = new shared::ErrorIndicatingLineEdit(parent);
    const shared::Id id = index.data(shared::AbstractVisualizationModel::IdRole).toUuid();
    dvm::DVObject *object = m_model ? m_model->getObject(id)->as<dvm::DVObject *>() : nullptr;
    auto validator = new QDVNameValidator(object, lineEdit);
    lineEdit->setValidator(validator);
    return lineEdit;
}

} // namespace dve

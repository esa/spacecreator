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

#include "functionattrdelegate.h"

#include "tab_aadl/aadlcommonprops.h"
#include "tab_aadl/aadlobjectfunction.h"
#include "tab_aadl/aadlobjectsmodel.h"
#include "tab_interface/properties/propertieslistmodel.h"

#include <QComboBox>
#include <QDebug>

namespace taste3 {
namespace aadl {

FunctionAttrDelegate::FunctionAttrDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *FunctionAttrDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const
{
    if (index.column() == PropertiesListModel::ColumnValue)
        if (const PropertiesListModel *pModel = qobject_cast<const PropertiesListModel *>(index.model())) {
            if (pModel->isAttr(index)) {
                const QString &attrName =
                        pModel->data(pModel->index(PropertiesListModel::ColumnTitle, index.row())).toString();
                const meta::Props::Token t = meta::Props::token(attrName);

                const AADLObjectFunction *objFn = qobject_cast<const AADLObjectFunction *>(pModel->dataObject());
                Q_ASSERT(objFn);

                switch (t) {
                case meta::Props::Token::instance_of: {
                    const QStringList &availableFnTypes = (QStringList() << QString())
                            + objFn->objectsModel()->getAvailableFunctionTypes(objFn).keys();
                    QComboBox *cb = new QComboBox(parent);
                    cb->addItems(availableFnTypes);
                    return cb;
                }
                default:
                    break;
                }
            }
        }

    return QStyledItemDelegate::createEditor(parent, option, index);
}
} // ns aadl
} // ns taste3

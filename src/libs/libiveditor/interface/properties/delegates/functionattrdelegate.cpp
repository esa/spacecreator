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

#include "aadlcommonprops.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "interface/properties/propertieslistmodel.h"

#include <QComboBox>
#include <QDebug>

namespace aadlinterface {

FunctionAttrDelegate::FunctionAttrDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *FunctionAttrDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == PropertiesListModel::ColumnValue)
        if (const PropertiesListModel *pModel = qobject_cast<const PropertiesListModel *>(index.model())) {
            if (pModel->isAttr(index)) {
                const QString &attrName =
                        pModel->data(pModel->index(PropertiesListModel::ColumnTitle, index.row())).toString();
                const aadl::meta::Props::Token t = aadl::meta::Props::token(attrName);

                auto objFn = qobject_cast<const aadl::AADLObjectFunction *>(pModel->dataObject());
                Q_ASSERT(objFn);

                switch (t) {
                case aadl::meta::Props::Token::instance_of: {
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

}

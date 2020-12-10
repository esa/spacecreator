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

#include "interfaceattrdelegate.h"

#include "aadlcommonprops.h"
#include "aadlobjectiface.h"
#include "aadlobjectsmodel.h"
#include "interface/properties/propertieslistmodel.h"

#include <QComboBox>
#include <QDebug>

namespace aadlinterface {

InterfaceAttrDelegate::InterfaceAttrDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *InterfaceAttrDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == PropertiesListModel::ColumnValue)
        if (const PropertiesListModel *pModel = qobject_cast<const PropertiesListModel *>(index.model())) {
            if (pModel->isAttr(index)) {
                const QString &attrName =
                        pModel->data(pModel->index(PropertiesListModel::ColumnTitle, index.row())).toString();
                const aadl::meta::Props::Token t = aadl::meta::Props::token(attrName);

                auto iface = qobject_cast<const aadl::AADLObjectIface *>(pModel->dataObject());
                Q_ASSERT(iface);

                switch (t) {
                case aadl::meta::Props::Token::kind: {
                    auto names = iface->availableKindNames();
                    QComboBox *cb = new QComboBox(parent);
                    for (auto it = names.cbegin(); it != names.cend(); ++it) {
                        cb->addItem(it.value(), QVariant::fromValue(it.key()));
                    }
                    return cb;
                }
                default:
                    break;
                }
            }
        }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void InterfaceAttrDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == PropertiesListModel::ColumnValue)
        if (const PropertiesListModel *pModel = qobject_cast<const PropertiesListModel *>(index.model())) {
            if (pModel->isAttr(index)) {
                const QString &attrName =
                        pModel->data(pModel->index(PropertiesListModel::ColumnTitle, index.row())).toString();
                const aadl::meta::Props::Token t = aadl::meta::Props::token(attrName);

                auto iface = qobject_cast<const aadl::AADLObjectIface *>(pModel->dataObject());
                Q_ASSERT(iface);

                switch (t) {
                case aadl::meta::Props::Token::kind: {
                    if (QComboBox *cb = qobject_cast<QComboBox *>(editor))
                        cb->setCurrentIndex(static_cast<int>(iface->kind()));
                }
                default:
                    break;
                }
            }
        }

    return QStyledItemDelegate::setEditorData(editor, index);
}

}

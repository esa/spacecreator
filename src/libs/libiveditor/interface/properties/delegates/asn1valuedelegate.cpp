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

#include "asn1valuedelegate.h"

#include "asn1editor.h"
#include "datatypesstorage.h"

#include <QLabel>
#include <QVariant>

namespace aadlinterface {

const char *MODEL_INDEX_PROPERTY = "modelIndex";
const char *DIALOG_PROPERTY = "dialog";

Asn1ValueDelegate::Asn1ValueDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *Asn1ValueDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex &index) const
{
    auto dialog = new asn1::Asn1Editor(aadl::DataTypesStorage::instance()->asn1DataTypes(), parent);
    dialog->setProperty(MODEL_INDEX_PROPERTY, QVariant(index));
    dialog->setModal(true);
    QModelIndex typeIndex = index.siblingAtColumn(index.column() - 1);
    dialog->showAsn1Type(typeIndex.data().toString());

    connect(dialog, &asn1::Asn1Editor::accepted, this, &aadlinterface::Asn1ValueDelegate::onDialogAccepted);

    auto *proxy = new QLabel(parent);
    proxy->setProperty(DIALOG_PROPERTY, QVariant::fromValue(dialog));

    return proxy;
}

void Asn1ValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *proxy = qobject_cast<QLabel *>(editor);
    Q_ASSERT(proxy);
    proxy->setText(index.data().toString());

    auto dialog = proxy->property(DIALOG_PROPERTY).value<asn1::Asn1Editor *>();
    Q_ASSERT(dialog);
    dialog->setValue(index.data().toString());
    dialog->show();
}

void Asn1ValueDelegate::onDialogAccepted()
{
    auto dialog = qobject_cast<asn1::Asn1Editor *>(sender());
    if (dialog) {
        auto modelIndex = dialog->property(MODEL_INDEX_PROPERTY).toModelIndex();
        auto model = const_cast<QAbstractItemModel *>(modelIndex.model());
        const QString newValue = dialog->value();
        if (model && newValue != model->data(modelIndex).toString()) {
            model->setData(modelIndex, newValue);
        }
    }
}

}

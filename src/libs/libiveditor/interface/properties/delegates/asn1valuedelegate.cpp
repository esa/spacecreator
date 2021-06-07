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
#include "file.h"
#include "interface/properties/propertieslistmodel.h"

#include <QLabel>
#include <QVariant>

namespace ive {

static const char *MODEL_INDEX_PROPERTY = "modelIndex";
static const char *DIALOG_PROPERTY = "dialog";

Asn1ValueDelegate::Asn1ValueDelegate(const QSharedPointer<Asn1Acn::File> &asn1Types, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_asn1Types(asn1Types)
{
}

QWidget *Asn1ValueDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex &index) const
{
    auto dialog = new asn1::Asn1Editor(m_asn1Types, parent);
    dialog->setProperty(MODEL_INDEX_PROPERTY, QVariant(index));
    dialog->setModal(true);
    QModelIndex typeIndex = index.siblingAtColumn(index.column() - 1);
    QString typeName = typeIndex.data(PropertiesListModel::DataRole).toString();
    dialog->showAsn1Type(typeName);

    connect(dialog, &asn1::Asn1Editor::accepted, this, &ive::Asn1ValueDelegate::onDialogAccepted);

    auto *proxy = new QLabel(parent);
    proxy->setProperty(DIALOG_PROPERTY, QVariant::fromValue(dialog));

    return proxy;
}

void Asn1ValueDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const QString dataText = index.data().toString();
    auto *proxy = qobject_cast<QLabel *>(editor);
    Q_ASSERT(proxy);
    proxy->setText(dataText);

    auto dialog = proxy->property(DIALOG_PROPERTY).value<asn1::Asn1Editor *>();
    Q_ASSERT(dialog);
    dialog->setValue(dataText);
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
        dialog->deleteLater();
    }
}

}

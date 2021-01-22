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

#include "comboboxdelegate.h"

#include <QComboBox>
#include <QStringListModel>

namespace ive {

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

ComboBoxDelegate::~ComboBoxDelegate() { }

QWidget *ComboBoxDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem & /*option*/, const QModelIndex &index) const
{
    QComboBox *cb = new QComboBox(parent);
    cb->setModel(editorModel(index));
    return cb;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    const QString &currentText = index.data(Qt::EditRole).toString();
    const int cbIndex = cb->findText(currentText);
    if (cbIndex >= 0)
        cb->setCurrentIndex(cbIndex);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (QComboBox *cb = qobject_cast<QComboBox *>(editor))
        model->setData(index, cb->currentText(), Qt::EditRole);
}

StringListComboDelegate::StringListComboDelegate(const QStringList &data, QObject *parent)
    : ComboBoxDelegate(parent)
    , m_model(new QStringListModel(this))
{
    m_model->setStringList(data);
}
StringListComboDelegate::~StringListComboDelegate() { }

QAbstractItemModel *StringListComboDelegate::editorModel(const QModelIndex & /*id*/) const
{
    return m_model;
}

}

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

#include "comboboxdelegate.h"

#include <QComboBox>

namespace shared {

ComboBoxDelegate::ComboBoxDelegate(const QStringList &options, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_options(options)
{
}

QWidget *ComboBoxDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        auto comboBox = new QComboBox(parent);
        QObject::connect(comboBox, &QComboBox::currentTextChanged, this,
                [this, comboBox]() { const_cast<ComboBoxDelegate *>(this)->commitData(comboBox); });
        comboBox->addItems(m_options);
        comboBox->setFocusPolicy(Qt::StrongFocus);
        comboBox->setEnabled(index.flags().testFlag(Qt::ItemIsEnabled));
        return comboBox;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.isValid()) {
        if (auto comboBox = qobject_cast<QComboBox *>(editor)) {
            comboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
        }
        return;
    }

    return QStyledItemDelegate::setEditorData(editor, index);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (auto comboBox = qobject_cast<QComboBox *>(editor)) {
        model->setData(index, comboBox->currentText());
        return;
    }

    QStyledItemDelegate::setModelData(editor, model, index);
}

} // namespace shared

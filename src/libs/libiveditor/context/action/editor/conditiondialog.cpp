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

#include "conditiondialog.h"

#include "attributesmodel.h"
#include "ui_conditiondialog.h"

namespace ive {

/*!
 * \class ive::ConditionDialog
 * The UI for editing conditions of scriptable actions.
 */

ConditionDialog::ConditionDialog(const Condition &cond, const QStringList &itemTypes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConditionDialog)
    , m_condition(cond)
    , m_attrsModel(new AttributesModel(this))
{
    ui->setupUi(this);
    ui->lvAttrs->setModel(m_attrsModel);
    m_attrsModel->setAttributesList(&m_condition.m_attrs);

    ui->cbItemType->addItems(itemTypes);
    ui->cbItemType->setCurrentIndex(itemTypes.indexOf(cond.m_itemType));

    connect(ui->cbItemType, &QComboBox::currentTextChanged, this, &ConditionDialog::setItemType);
    connect(ui->btnAddAttr, &QPushButton::clicked, this, &ConditionDialog::addAttribute);
    connect(ui->btnRmAttr, &QPushButton::clicked, this, &ConditionDialog::removeAttribute);
}

ConditionDialog::~ConditionDialog()
{
    delete ui;
}

Condition ConditionDialog::condition() const
{
    return m_condition;
}

void ConditionDialog::setItemType(const QString &text)
{
    m_condition.m_itemType = text;
}

void ConditionDialog::addAttribute()
{
    m_attrsModel->insertRows(m_attrsModel->rowCount(), 1);
}

void ConditionDialog::removeAttribute()
{
    const QModelIndex &currId = ui->lvAttrs->selectionModel()->currentIndex();
    if (!currId.isValid()) {
        return;
    }
    m_attrsModel->removeRow(currId.row());
}

}

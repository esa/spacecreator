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

namespace taste3 {
namespace aadl {

ConditionDialog::ConditionDialog(const ctx::Condition &cond, const QStringList &itemTypes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConditionDialog)
    , m_condition(cond)
    , m_attrsModel(new AttributesModel(this))

{
    ui->setupUi(this);
    ui->lvAttrs->setModel(m_attrsModel);
    m_attrsModel->setAttributesList(&m_condition.m_attrs);

    ui->cbItemType->addItems(itemTypes); // triggers on_cbItemType_currentIndexChanged
    ui->cbItemType->setCurrentIndex(itemTypes.indexOf(cond.m_itemType));
}

ConditionDialog::~ConditionDialog()
{
    delete ui;
}

ctx::Condition ConditionDialog::condition() const
{
    return m_condition;
}

void ConditionDialog::on_cbItemType_currentIndexChanged(const QString &text)
{
    m_condition.m_itemType = text;
}

void ConditionDialog::on_btnAddAttr_clicked()
{
    m_attrsModel->insertRows(m_attrsModel->rowCount(), 1);
}

void ConditionDialog::on_btnRmAttr_clicked()
{
    const QModelIndex &currId = ui->lvAttrs->selectionModel()->currentIndex();
    if (!currId.isValid())
        return;
    m_attrsModel->removeRow(currId.row());
}

} // ns aadl
} // ns taste3

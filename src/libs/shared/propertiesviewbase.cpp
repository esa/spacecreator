/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "propertiesviewbase.h"

#include "propertieslistmodel.h"
#include "propertiesmodelbase.h"
#include "ui_propertiesviewbase.h"

#include <QHeaderView>
#include <QStandardItemModel>

namespace shared {

PropertiesViewBase::PropertiesViewBase(const QList<int> delegatesColumns, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PropertiesViewBase)
    , m_delegatesColumns(delegatesColumns)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(120);
}

PropertiesViewBase::~PropertiesViewBase()
{
    delete ui;
}

void PropertiesViewBase::setModel(PropertiesModelBase *model)
{
    if (model == m_model)
        return;

    if (tableView()->selectionModel()) {
        disconnect(tableView()->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
                &PropertiesViewBase::onCurrentRowChanged);
    }
    if (m_model) {
        disconnect(m_model, &QStandardItemModel::rowsInserted, this, &PropertiesViewBase::rowsInserted);
    }

    m_model = model;
    tableView()->setModel(m_model);
    connect(m_model, &QStandardItemModel::rowsInserted, this, &PropertiesViewBase::rowsInserted, Qt::QueuedConnection);

    if (tableView()->selectionModel()) {
        connect(tableView()->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
                &PropertiesViewBase::onCurrentRowChanged);
    }
    setButtonsDisabled();
    rowsInserted(QModelIndex(), 0, m_model->rowCount() - 1);

    if (m_model->rowCount()) {
        tableView()->resizeColumnsToContents();
    }
}

QTableView *PropertiesViewBase::tableView() const
{
    return ui->tableView;
}

void PropertiesViewBase::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &)
{
    if (m_model && !setButtonsDisabled()) {
        ui->btnDel->setEnabled(current.isValid());
    }
}

static inline QString generatePropertyName(QStandardItemModel *model)
{
    static const QString newNameTempl { QLatin1String("p%1") };
    int duplicateCounter(1);
    QString newName = newNameTempl.arg(duplicateCounter);
    while (!model->findItems(newName).isEmpty()) {
        newName = newNameTempl.arg(++duplicateCounter);
    }
    return newName;
}

void PropertiesViewBase::on_btnAdd_clicked()
{
    if (m_model) {
        if (m_model->createProperty(generatePropertyName(m_model))) {
            const QModelIndex &added = m_model->index(m_model->rowCount() - 1, 0);
            ui->tableView->scrollToBottom();
            // Delay the editing, after the value editor is created, so this name text is on top of the value editor
            QMetaObject::invokeMethod(ui->tableView, "edit", Qt::QueuedConnection, Q_ARG(const QModelIndex &, added));
        }
    }
}

void PropertiesViewBase::on_btnDel_clicked()
{
    if (m_model) {
        m_model->removeProperty(ui->tableView->currentIndex());
        ui->tableView->update();
    }
}

void PropertiesViewBase::setButtonsDisabled(bool state)
{
    ui->btnAdd->setDisabled(state);
    ui->btnDel->setDisabled(state);
}

void PropertiesViewBase::rowsInserted(const QModelIndex &parent, int first, int last)
{
    for (int idx = first; idx <= last; ++idx) {
        for (const int column : qAsConst(m_delegatesColumns)) {
            const QModelIndex index = m_model->index(idx, column, parent);
            if (index.isValid()) {
                tableView()->openPersistentEditor(index);
            }
        }
    }
}

AttributesView::AttributesView(QWidget *widget)
    : PropertiesViewBase({ PropertiesListModel::Column::Value }, widget)
{
    ui->btnAdd->hide();
    ui->btnDel->hide();
}

bool AttributesView::setButtonsDisabled()
{
    PropertiesViewBase::setButtonsDisabled(true);
    return true;
}

} // namespace shared

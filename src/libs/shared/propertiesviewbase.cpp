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
    , m_delegatesColumns(delegatesColumns)
    , ui(new Ui::PropertiesViewBase)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(120);

    connect(ui->btnUp, &QPushButton::clicked, this, &PropertiesViewBase::moveCurrentRowUp);
    connect(ui->btnDown, &QPushButton::clicked, this, &PropertiesViewBase::moveCurrentRowDown);
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
        disconnect(tableView()->selectionModel(), &QItemSelectionModel::selectionChanged, this,
                &PropertiesViewBase::onSelectionChanged);
    }
    if (m_model) {
        disconnect(m_model, &QStandardItemModel::rowsInserted, this, &PropertiesViewBase::rowsInserted);
    }

    m_model = model;
    tableView()->setModel(m_model);
    connect(m_model, &QStandardItemModel::rowsInserted, this, &PropertiesViewBase::rowsInserted, Qt::QueuedConnection);

    if (tableView()->selectionModel()) {
        connect(tableView()->selectionModel(), &QItemSelectionModel::selectionChanged, this,
                &PropertiesViewBase::onSelectionChanged);
    }
    updateButtons();
    rowsInserted(QModelIndex(), 0, m_model->rowCount() - 1);

    if (m_model->rowCount()) {
        tableView()->resizeColumnsToContents();
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
    }
}

QTableView *PropertiesViewBase::tableView() const
{
    return ui->tableView;
}

void PropertiesViewBase::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    updateButtons();
    ui->btnDel->setEnabled(!selected.isEmpty());

    if (selected.isEmpty()) {
        ui->btnUp->setEnabled(false);
        ui->btnDown->setEnabled(false);
    } else {
        const QModelIndexList indexes = selected.indexes();
        bool hasFirst { false }, hasLast { false };
        std::for_each(indexes.cbegin(), indexes.cend(), [&hasFirst, &hasLast](const QModelIndex &index) {
            hasFirst |= index.row() == 0;
            hasLast |= index.row() == index.model()->rowCount(index.parent()) - 1;
        });
        ui->btnUp->setEnabled(!hasFirst);
        ui->btnDown->setEnabled(!hasLast);
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
            if (m_model->rowCount() == 1) {
                ui->tableView->resizeColumnsToContents();
                ui->tableView->horizontalHeader()->setStretchLastSection(true);
            }
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

void PropertiesViewBase::moveCurrentRowUp()
{
    if (m_model) {
        QModelIndexList idxs = ui->tableView->selectionModel()->selectedRows();
        QList<int> rows;
        for (const QModelIndex &idx : qAsConst(idxs))
            rows.append(idx.row() - 1);

        std::sort(idxs.begin(), idxs.end(),
                [](const QModelIndex &idx1, const QModelIndex &idx2) { return idx1.row() < idx2.row(); });
        for (int idx = 0; idx < idxs.size(); ++idx) {
            m_model->moveRow(QModelIndex(), idxs.value(idx).row(), QModelIndex(), idxs.value(idx).row() - 1);
        }

        ui->tableView->selectionModel()->clearSelection();
        for (int row : qAsConst(rows)) {
            const QItemSelection selection(m_model->index(row, 0), m_model->index(row, m_model->columnCount() - 1));
            ui->tableView->selectionModel()->select(
                    selection, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
        }
    }
}

void PropertiesViewBase::moveCurrentRowDown()
{
    if (m_model) {
        QModelIndexList idxs = ui->tableView->selectionModel()->selectedRows();
        QList<int> rows;
        for (auto idx : idxs)
            rows.append(idx.row() + 1);

        std::sort(idxs.begin(), idxs.end(),
                [](const QModelIndex &idx1, const QModelIndex &idx2) { return idx1.row() > idx2.row(); });
        for (int idx = 0; idx < idxs.size(); ++idx) {
            m_model->moveRow(QModelIndex(), idxs.value(idx).row(), QModelIndex(), idxs.value(idx).row() + 1);
        }

        ui->tableView->selectionModel()->clearSelection();
        for (int row : rows) {
            const QItemSelection selection(m_model->index(row, 0), m_model->index(row, m_model->columnCount() - 1));
            ui->tableView->selectionModel()->select(
                    selection, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
        }
    }
}

void PropertiesViewBase::rowsInserted(const QModelIndex &parent, int first, int last)
{
    for (int idx = first; idx <= last; ++idx) {
        for (const int column : qAsConst(m_delegatesColumns)) {
            const QModelIndex index = m_model->index(idx, column, parent);
            if (index.isValid()) {
                tableView()->openPersistentEditor(index);
            }
            ui->tableView->resizeColumnToContents(column);
        }
    }
}

QPushButton *PropertiesViewBase::addButton() const
{
    return ui->btnAdd;
}

QPushButton *PropertiesViewBase::deleteButton() const
{
    return ui->btnDel;
}

QPushButton *PropertiesViewBase::upButton() const
{
    return ui->btnUp;
}

QPushButton *PropertiesViewBase::downButton() const
{
    return ui->btnDown;
}

AttributesView::AttributesView(QWidget *widget)
    : PropertiesViewBase({ PropertiesListModel::Column::Value }, widget)
{
}

void AttributesView::updateButtons()
{
    addButton()->hide();
    deleteButton()->hide();
    upButton()->hide();
    downButton()->hide();
}

} // namespace shared

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

#include "propertiesviewbase.h"

#include "aadlfunction.h"
#include "aadliface.h"
#include "contextparametersmodel.h"
#include "ifaceparametersmodel.h"
#include "propertieslistmodel.h"
#include "ui_propertiesviewbase.h"

#include <QDebug>
#include <QHeaderView>
#include <QSortFilterProxyModel>

namespace ive {

PropertiesViewBase::PropertiesViewBase(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PropertiesViewBase)
{
    ui->setupUi(this);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->horizontalHeader()->setMinimumSectionSize(120);

    setButtonsDisabled();
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

    if (m_model->rowCount()) {
        tableView()->resizeColumnsToContents();
    }

    if (tableView()->selectionModel()) {
        connect(tableView()->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
                &PropertiesViewBase::onCurrentRowChanged);
    }
    setButtonsDisabled();
    rowsInserted(QModelIndex(), 0, m_model->rowCount() - 1);
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
            ui->tableView->edit(added);
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

void PropertiesViewBase::rowsInserted(const QModelIndex &parent, int first, int last)
{
    for (int idx = first; idx <= last; ++idx) {
        for (const int column : m_delegatesColumns) {
            const QModelIndex index = m_model->index(idx, column, parent);
            if (index.isValid()) {
                tableView()->openPersistentEditor(index);
            }
        }
    }
}

bool PropertiesViewBase::setButtonsDisabled()
{
    if (!m_model)
        return false;

    bool disabled = false;

    if (auto dataObject = m_model->dataObject()) {
        switch (dataObject->aadlType()) {
        case ivm::AADLObject::Type::Function: {
            if (auto fn = dataObject->as<const ivm::AADLFunction *>())
                disabled = fn->inheritsFunctionType();
            break;
        }
        case ivm::AADLObject::Type::RequiredInterface:
        case ivm::AADLObject::Type::ProvidedInterface: {
            if (auto iface = dataObject->as<const ivm::AADLIface *>()) {
                disabled = iface->isClone();
                if (!disabled && iface->isRequired()) {
                    if (auto ri = iface->as<const ivm::AADLIfaceRequired *>())
                        disabled = ri->hasPrototypePi();
                }
            }

            break;
        }
        default:
            break;
        }
    }

    if (disabled) {
        ui->btnAdd->setDisabled(disabled);
        ui->btnDel->setDisabled(disabled);
    }

    return disabled;
}

ContextParametersView::ContextParametersView(QWidget *widget)
    : PropertiesViewBase(widget)
{
    m_delegatesColumns = { ContextParametersModel::Column::Type, ContextParametersModel::Column::Value };
}

IfaceParametersView::IfaceParametersView(QWidget *widget)
    : PropertiesViewBase(widget)
{
    m_delegatesColumns = { IfaceParametersModel::Column::Type, IfaceParametersModel::Column::Encoding,
        IfaceParametersModel::Column::Direction };
}

AttributesView::AttributesView(QWidget *widget)
    : PropertiesViewBase(widget)
{
    m_delegatesColumns = { PropertiesListModel::Column::Value };
}

} // namespace ive

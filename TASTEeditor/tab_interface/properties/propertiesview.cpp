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

#include "propertiesview.h"

#include "propertieslistmodel.h"
#include "propsfiltermodel.h"
#include "ui_propertiesview.h"

#include <QDebug>
#include <QSortFilterProxyModel>

namespace taste3 {
namespace aadl {

PropertiesView::PropertiesView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PropertiesView)
    , m_filter(new PropsFilterModel(this))
{
    ui->setupUi(this);
}

PropertiesView::~PropertiesView()
{
    delete ui;
}

void PropertiesView::setModel(PropertiesListModel *model)
{
    if (model == m_model)
        return;

    if (ui->tableView->selectionModel())
        disconnect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
                   &PropertiesView::onCurrentRowChanged);

    m_model = model;
    m_filter->setSourceModel(m_model);
    ui->tableView->setModel(m_filter);
    ui->tableView->resizeColumnsToContents();

    if (ui->tableView->selectionModel())
        connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this,
                &PropertiesView::onCurrentRowChanged);
}

QTableView *PropertiesView::tableView() const
{
    return ui->tableView;
}

void PropertiesView::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &)
{
    if (m_model)
        ui->btnDel->setEnabled(m_model->isProp(current));
}

void PropertiesView::on_btnAdd_clicked()
{
    if (m_model) {
        static const QString newNameTmp = tr("New property");
        QString newName(newNameTmp);
        int duplicateCounter(0);
        while (!m_model->findItems(newName).isEmpty())
            newName = QString("%1 #%2").arg(newNameTmp, QString::number(++duplicateCounter));

        if (m_model->createProperty(newName)) {
            const QModelIndex &added = m_model->index(m_model->rowCount() - 1, 0);
            //            ui->tableView->scrollTo(added, QAbstractItemView::EnsureVisible);
            ui->tableView->scrollToBottom();
            ui->tableView->edit(added);
        }
    }
}

void PropertiesView::on_btnDel_clicked()
{
    if (m_model)
        m_model->removeProperty(ui->tableView->currentIndex());
}

void PropertiesView::setPropType(PropertiesListModel::ItemType type)
{
    m_filter->setTargetType(type);

    const bool isProps = type == PropertiesListModel::Property;
    ui->btnAdd->setVisible(isProps);
    ui->btnDel->setVisible(isProps);
}

} // namespace aadl
} // namespace taste3

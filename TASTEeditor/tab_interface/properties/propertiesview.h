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

#pragma once

#include "propertieslistmodel.h"

#include <QWidget>

namespace Ui {
class PropertiesView;
}

class QTableView;

namespace taste3 {
namespace aadl {

class PropsFilterModel;
class PropertiesView : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesView(QWidget *parent = nullptr);
    ~PropertiesView();

    void setModel(PropertiesListModel *model);
    QTableView *tableView() const;

    void setPropType(PropertiesListModel::ItemType type);

private slots:
    void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_btnAdd_clicked();
    void on_btnDel_clicked();

private:
    Ui::PropertiesView *ui { nullptr };
    PropertiesListModel *m_model { nullptr };
    bool m_buttonsVisible { true };
    PropsFilterModel *m_filter { nullptr };
};

} // namespace aadl
} // namespace taste3

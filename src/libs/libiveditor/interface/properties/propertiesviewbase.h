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
class PropertiesViewBase;
}

class QTableView;
class QAbstractItemModel;

namespace ive {

class PropertiesModelBase;
class PropertiesViewBase : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesViewBase(QWidget *parent = nullptr);
    ~PropertiesViewBase();

    virtual void setModel(PropertiesModelBase *model);
    QTableView *tableView() const;

protected Q_SLOTS:
    virtual void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    virtual void on_btnAdd_clicked();
    virtual void on_btnDel_clicked();

protected:
    PropertiesModelBase *m_model { nullptr };
    QList<int> m_delegatesColumns;
    bool m_buttonsVisible { true };

protected:
    bool setButtonsDisabled();
    void rowsInserted(const QModelIndex &parent, int first, int last);

private:
    Ui::PropertiesViewBase *ui { nullptr };
};

class AttributesView : public PropertiesViewBase
{
    Q_OBJECT

public:
    explicit AttributesView(QWidget *widget = nullptr);
    ~AttributesView() override = default;
};

class ContextParametersView : public PropertiesViewBase
{
    Q_OBJECT

public:
    explicit ContextParametersView(QWidget *widget = nullptr);
    ~ContextParametersView() override = default;
};

class IfaceParametersView : public PropertiesViewBase
{
    Q_OBJECT

public:
    explicit IfaceParametersView(QWidget *widget = nullptr);
    ~IfaceParametersView() override = default;
};

}

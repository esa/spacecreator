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

#pragma once

#include <QWidget>

class QTableView;
class QItemSelection;
class QPushButton;

namespace Ui {
class PropertiesViewBase;
}

namespace shared {
class PropertiesModelBase;

class PropertiesViewBase : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesViewBase(const QList<int> delegatesColumns, QWidget *parent = nullptr);
    ~PropertiesViewBase();

    virtual void setModel(PropertiesModelBase *model);
    QTableView *tableView() const;

protected Q_SLOTS:
    void on_btnAdd_clicked();
    void on_btnDel_clicked();
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void moveCurrentRowUp();
    void moveCurrentRowDown();

protected:
    PropertiesModelBase *m_model { nullptr };
    const QList<int> m_delegatesColumns;

protected:
    virtual void updateButtons() = 0;
    void rowsInserted(const QModelIndex &parent, int first, int last);

    QPushButton *addButton() const;
    QPushButton *deleteButton() const;
    QPushButton *upButton() const;
    QPushButton *downButton() const;

private:
    Ui::PropertiesViewBase *ui { nullptr };
};

class AttributesView : public PropertiesViewBase
{
    Q_OBJECT

public:
    explicit AttributesView(QWidget *widget = nullptr);
    ~AttributesView() override = default;

    void updateButtons() override;
};

} // namespace shared

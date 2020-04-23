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

#pragma once

#include "app/context/action/action.h"

#include <QDialog>
#include <QVector>

namespace Ui {
class DynActionEditor;
}

class QItemSelection;

namespace taste3 {
namespace aadl {

class ActionsModel;
class ConditionsModel;
class DynActionEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DynActionEditor(QWidget *parent = nullptr);
    ~DynActionEditor() override;

    bool init();

public Q_SLOTS:
    void accept() override;

private Q_SLOTS:
    void onActionActivated(const QModelIndex &current, const QModelIndex &previous);
    void onConditionActivated(const QModelIndex &index);
    void on_cbActType_currentIndexChanged(int id);
    void on_btnCreateAction_clicked();
    void on_btnRemoveAction_clicked();
    void on_btnCreateCondition_clicked();
    void on_btnRemoveCondition_clicked();
    void on_btnSelectFile_clicked();
    void on_btnCreateFile_clicked();

private:
    Ui::DynActionEditor *ui;
    QVector<ctx::Action> m_actions;
    ctx::Action *m_action { nullptr };
    ActionsModel *m_actionsModel { nullptr };
    ConditionsModel *m_conditionsModel { nullptr };

    bool loadFile(const QString &filePath);
    void displayAction(ctx::Action *action);
    void commitCurrentAction();

    bool save();
};

} // ns aadl
} // ns taste3

/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "context/action/action.h"

#include <QVector>
#include <QWidget>

namespace Ui {
class DynActionEditorWidget;
}

namespace ive {

class ActionsModel;
class ConditionsModel;

class DynActionEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DynActionEditorWidget(QWidget *parent = nullptr);
    ~DynActionEditorWidget();

    bool init();
    bool loadFile(const QString &filePath);
    void displayAction(Action *action);
    void commitCurrentAction();

    bool save();

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
    void on_btnInfo_clicked();

private:
    QString listRegisteredActions() const;

    Ui::DynActionEditorWidget *ui;
    QVector<Action> m_actions;
    Action *m_action { nullptr };
    ActionsModel *m_actionsModel { nullptr };
    ConditionsModel *m_conditionsModel { nullptr };
};

}

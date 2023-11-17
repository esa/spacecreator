/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "checkedfilterproxymodel.h"

#include <QPointer>
#include <QSortFilterProxyModel>
#include <QWidget>

namespace Ui {
class RequirementsWidget;
}

namespace ive {
class InterfaceDocument;
}

class RequirementsManager;

class RequirementsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequirementsWidget(const QByteArray &requirementsUrl, RequirementsManager *manager,
            QAbstractItemModel *model, QWidget *parent = nullptr);
    ~RequirementsWidget();

    void setUrl(const QString &url);
    void setToken(const QString &token);

protected:
    void loadSavedCredentials();
    void onChangeOfCredentials();

protected Q_SLOTS:
    void onLoginUpdate();
    void openIssueLink(const QModelIndex &index);
    void openTokenSettingsPage();
    void toggleShowUsedRequirements();

Q_SIGNALS:
    void requirementSelected(QString RequirementID, bool checked);
    void requirementsUrlChanged(QByteArray requirementsUrl);

private:
    Ui::RequirementsWidget *ui;
    QPointer<RequirementsManager> mReqManager;
    QPointer<QAbstractItemModel> m_model;
    QSortFilterProxyModel m_filterModel;
    shared::CheckedFilterProxyModel m_checkedModel;
    QByteArray m_requirementsUrl;
};

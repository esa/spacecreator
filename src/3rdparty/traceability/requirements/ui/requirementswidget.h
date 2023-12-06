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
#include "tagfilterproxymodel.h"

#include <QPointer>
#include <QSortFilterProxyModel>
#include <QToolButton>
#include <QWidget>

namespace Ui {
class RequirementsWidget;
}

namespace ive {
class InterfaceDocument;
}

namespace requirement {
class RequirementsManager;
class RequirementsModelBase;

class RequirementsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RequirementsWidget(const QString &requirementsUrl, RequirementsManager *manager,
            requirement::RequirementsModelBase *model, QWidget *parent = nullptr);
    ~RequirementsWidget();

    void setUrl(const QString &url);
    QUrl url() const;
    void setToken(const QString &token);
    QString token() const;

protected:
    bool loadSavedCredentials();
    void onChangeOfCredentials();

protected Q_SLOTS:
    void setLoginData();
    void updateServerStatus();
    void openIssueLink(const QModelIndex &index);
    void openTokenSettingsPage();
    void toggleShowUsedRequirements();
    void requestRequirements();
    void showNewRequirementDialog() const;
    void removeRequirement();
    void modelSelectionChanged(const QItemSelection &selected, const QItemSelection &);
    void fillTagBar(const QStringList &tags);

Q_SIGNALS:
    void requirementSelected(QString RequirementID, bool checked);
    void requirementsUrlChanged(QString requirementsUrl);

private:
    QString tokenKey(const QString &base) const { return QString("%1__token").arg(base); }
    bool tagButtonExists(const QString &tag) const;

    Ui::RequirementsWidget *ui;
    QList<QToolButton *> m_tagButtons;
    QPointer<RequirementsManager> m_reqManager;
    QPointer<requirement::RequirementsModelBase> m_model;
    QSortFilterProxyModel m_textFilterModel;
    TagFilterProxyModel m_tagFilterModel;
    CheckedFilterProxyModel m_checkedModel;
    QString m_requirementsUrl;
};

}

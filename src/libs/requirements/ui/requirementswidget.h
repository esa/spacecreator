#pragma once

#include "requirementsmanager.h"
#include "requirementsmodel.h"

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
    explicit RequirementsWidget(QByteArray requirementsUrl, QStringList requirementsIDs, QWidget *parent = nullptr);
    ~RequirementsWidget();

    void setUrl(const QString &url);
    void setToken(const QString &token);

protected:
    void LoadSavedCredentials();
    void onChangeOfCredentials();

protected Q_SLOTS:
    void onLoginUpdate();
    void openIssueLink(const QModelIndex &index);

Q_SIGNALS:
    void requirementSelected(QString RequirementID, bool checked);
    void requirementsUrlChanged(QByteArray requirementsUrl);

private:
    Ui::RequirementsWidget *ui;
    RequirementsManager mReqManager;
    requirement::RequirementsModel m_model;
    QSortFilterProxyModel m_filterModel;
    QByteArray m_requirementsUrl;
};

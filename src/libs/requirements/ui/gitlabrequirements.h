#ifndef GITLABREQUIREMENTS_H
#define GITLABREQUIREMENTS_H

#include "requirementsmanager.h"
#include "requirementsmodel.h"

#include <QPointer>
#include <QSortFilterProxyModel>
#include <QWidget>

namespace Ui {
class GitLabRequirements;
}

namespace ive {
class InterfaceDocument;
}

class RequirementsManager;

class GitLabRequirements : public QWidget
{
    Q_OBJECT

public:
    explicit GitLabRequirements(QByteArray requirementsUrl,  QStringList requirementsIDs, QWidget *parent = nullptr);
    ~GitLabRequirements();

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
    Ui::GitLabRequirements *ui;
    RequirementsManager mReqManager;
    requirement::RequirementsModel m_model;
    QSortFilterProxyModel m_filterModel;
    QByteArray m_requirementsUrl;
};

#endif // GITLABREQUIREMENTS_H

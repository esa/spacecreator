#ifndef REQUIREMENTSMANAGER_H
#define REQUIREMENTSMANAGER_H

#include <QObject>
#include <issue.h>
#include <issuerequestoptions.h>
#include <qgitlabclient.h>

class RequirementsManager: public QObject
{
    Q_OBJECT
public:

    enum class REPO_TYPE
    {
        GITLAB
    };

    RequirementsManager(REPO_TYPE repoType);
    void setCredentials(const QString &url, const QString &token);
    void requestRequirements(const QString &projectID, const QString &assignee, const QString &author) const;
    void createRequirement(const QString &projectID, const gitlab::Issue &issue) const;
    void RequestListofLabels(const QString &projectID,
                             const QString &with_counts = "false",
                             const QString &search = QString());
    void RequestProjectID(const QString &projectName);
    void editIssue(const QString &projectID, const QString &issueID, const gitlab::Issue &newIssue);

Q_SIGNALS:
    void listOfIssues(QList<gitlab::Issue>);
    void listOfLabels(QList<gitlab::Label>);
    void RequestedProjectID(QString);

private:
    REPO_TYPE mRepoType;
    gitlab::QGitlabClient *gitlabClient;
};

#endif // REQUIREMENTSMANAGER_H

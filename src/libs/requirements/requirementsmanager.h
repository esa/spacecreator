#ifndef REQUIREMENTSMANAGER_H
#define REQUIREMENTSMANAGER_H

#include "qgitlabclient.h"
#include <QObject>
class RequirementsManager: public QObject
{
    Q_OBJECT
public:

    enum class REPO_TYPE
    {
        GITLAB
    };

    RequirementsManager(REPO_TYPE repoType, const QString &url, const QString &token);
    void requestRequirements(const QString &assignee,
                             const QString &author,
                             const QStringList &iids) const;
    void createRequirement(const QString &projectID, const Issue &issue) const;
    void RequestListofLabels(const QString &projectID,
                             const QString &with_counts = "false",
                             const QString &search = QString());
    void editIssue(const QString &projectID, const QString &issueID, const Issue& newIssue);
Q_SIGNALS:
    void listOfIssues(QList<Issue>);
    void listOfLabels(QList<Label>);

private:
    REPO_TYPE mRepoType;
    QGitlabClient * gitlabClient;
};

#endif // REQUIREMENTSMANAGER_H

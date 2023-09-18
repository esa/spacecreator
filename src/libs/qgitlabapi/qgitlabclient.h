#ifndef QGITLABCLIENT_H
#define QGITLABCLIENT_H

#include "QGitlabAPI_global.h"
#include <QString>
#include <QList>
#include "Issue.h"
#include "Label.h"
#include "GitLabTransport.h"

class QGITLABAPI_EXPORT QGitlabClient: public QObject {
    Q_OBJECT
public:
    QGitlabClient(const QString & url, const QString &token);
    void requestIssues(const QString & assignee = QString(), const QString & author = QString(), const QString &iids = QString());
    void requestIssue(const QString &iid);
    void editIssue(const QString &projectID, const QString &issueID, const Issue& newIssue);
    void createIssue(const QString & projectID, const Issue &issue);
    void RequestListofLabels(const QString &projectID,
                             const QString &with_counts = "false",
                             const QString &search = QString());
Q_SIGNALS:
    void listOfIssues(QList<Issue>);
    void listOfLabels(QList<Label>);
private:
    std::unique_ptr<GitLabTransport> mGitlab;
};

#endif // QGITLABCLIENT_H

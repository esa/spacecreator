#ifndef GITLABTRANSPORT_H
#define GITLABTRANSPORT_H

#include "Issue.h"
#include "Label.h"
#include "UrlComposer.h"

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QObject>

class GitLabTransport : public QObject
{
    Q_OBJECT

public:
    enum ReqType {
        GET = 0,
        POST = 1,
        PUT = 2,
    };
    GitLabTransport(const QString &url, const QString &token);
    void RequestListofIssues(const QString &assignee = QString(),
                             const QString &author = QString(),
                             const QStringList &iids = QStringList());
    void RequestIssue(const QString &issueID);
    void RequestListofLabels(const QString &projectID,
                             const QString &with_counts,
                             const QString &search);
    void CreateIssue(const QString &projectID, const Issue &issue);
    void EditIssue(const QString &projectID, const Issue &newIssue);

protected:
    QNetworkReply *SendRequest(ReqType reqType, const QUrl &url);

Q_SIGNALS:
    void ListOfIssues(QList<Issue> issues);
    void ListOfLabels(QList<Label> labels);
    void ConnectionError(QString errorString);

private:
    QString mUsername;
    UrlComposer mUrlComposer;
    QString mToken;
    QNetworkAccessManager mManager;
};

#endif

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
    GitLabTransport();
    void setCredentials(const QString &url, const QString &token);
    void RequestListofIssues(const QString &projectID, const QString &assignee = QString(),
            const QString &author = QString(), const QStringList &iids = QStringList(), const int page = 1);
    void RequestIssue(const QString &issueID);
    void RequestListofLabels(const QString &projectID,
                             const QString &with_counts,
                             const QString &search);
    void CreateIssue(const QString &projectID, const Issue &issue);
    void EditIssue(const QString &projectID, const Issue &newIssue);
    void requestProjectIdByName(const QString &projectName);

protected:
    QNetworkReply *SendRequest(ReqType reqType, const QUrl &url);
    int pageNumberFromHeader(QNetworkReply *reply) const;
    int totalPagesFromHeader(QNetworkReply *reply) const;
    int numberHeaderAttribute(QNetworkReply *reply, const QByteArray &headername) const;

Q_SIGNALS:
    void ListOfIssues(QList<Issue> issues);
    void issueFetchingDone();
    void ListOfLabels(QList<Label> labels);
    void ConnectionError(QString errorString);
    void RequestedProjectID(QString projectID);

private:
    QString mUsername;
    UrlComposer mUrlComposer;
    QString mToken;
    QNetworkAccessManager mManager;
};

#endif

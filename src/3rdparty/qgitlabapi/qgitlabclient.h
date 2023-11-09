#ifndef QGITLABCLIENT_H
#define QGITLABCLIENT_H

#include "Issue.h"
#include "Label.h"
#include "QGitlabAPI_global.h"
#include "UrlComposer.h"

#include <QList>
#include <QNetworkAccessManager>
#include <QString>

class QGITLABAPI_EXPORT QGitlabClient: public QObject {
    Q_OBJECT
public:
    enum ReqType
    {
        GET = 0,
        POST = 1,
        PUT = 2,
    };

    QGitlabClient();
    void setCredentials(const QString & url, const QString &token);
    void requestIssues(const QString &projectID, const QString &assignee = QString(), const QString &author = QString(),
            const QStringList &iids = QStringList(), const int page = 1);
    void requestIssue(const QString &issueId);
    void editIssue(const QString &projectID, const QString &issueID, const Issue& newIssue);
    void createIssue(const QString & projectID, const Issue &issue);
    void requestListofLabels(
            const QString &projectID, const QString &with_counts = "false", const QString &search = QString());
    void requestProjectIdByName(const QString &projectName);

Q_SIGNALS:
    /**
     * @brief listOfIssues provides a block/page of issues
     */
    void listOfIssues(QList<Issue>);
    /**
     * @brief issueFetchingDone is sent once all pages of issues are loaded
     */
    void issueFetchingDone();
    void listOfLabels(QList<Label>);
    void requestedProjectID(QString);
    void connectionError(QString errorString);

protected:
    QNetworkReply *SendRequest(ReqType reqType, const QUrl &url);
    int pageNumberFromHeader(QNetworkReply *reply) const;
    int totalPagesFromHeader(QNetworkReply *reply) const;
    int numberHeaderAttribute(QNetworkReply *reply, const QByteArray &headername) const;

private:
    QString mUsername;
    UrlComposer mUrlComposer;
    QString mToken;
    QNetworkAccessManager mManager;
};

#endif // QGITLABCLIENT_H

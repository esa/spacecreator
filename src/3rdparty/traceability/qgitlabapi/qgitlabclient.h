#pragma once

#include "QGitlabAPI_global.h"
#include "issue.h"
#include "label.h"
#include "urlcomposer.h"

#include <QList>
#include <QNetworkAccessManager>
#include <QString>

namespace gitlab {

class IssueRequestOptions;

/**
 * @brief The QGitlabClient class is the main class to start requests on the Gitlab server
 */
class QGITLABAPI_EXPORT QGitlabClient : public QObject
{
    Q_OBJECT
public:
    enum ReqType
    {
        GET = 0,
        POST = 1,
        PUT = 2,
    };

    QGitlabClient();
    void setCredentials(const QString &url, const QString &token);
    void requestIssues(const QString &projectID, const IssueRequestOptions &options);
    void editIssue(const QString &projectID, const QString &issueID, const Issue &newIssue);
    void createIssue(const QString &projectID, const QString &title, const QString &description);
    void requestListofLabels(
            const QString &projectID, const QString &with_counts = "false", const QString &search = QString());
    void requestProjectId(const QUrl &projectUrl);

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
    void issueCreated();

protected:
    QNetworkReply *sendRequest(ReqType reqType, const QUrl &url);
    int pageNumberFromHeader(QNetworkReply *reply) const;
    int totalPagesFromHeader(QNetworkReply *reply) const;
    int numberHeaderAttribute(QNetworkReply *reply, const QByteArray &headername) const;

private:
    QString mUsername;
    UrlComposer mUrlComposer;
    QString mToken;
    QNetworkAccessManager mManager;
};
}

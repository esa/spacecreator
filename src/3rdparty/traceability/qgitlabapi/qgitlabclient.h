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
class LabelsRequestOptions;
class RequestOptions;

/**
 * @brief The QGitlabClient class is the main class to start requests on the Gitlab server
 */
class QGITLABAPI_EXPORT QGitlabClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyStateChanged)

public:
    enum ReqType
    {
        GET = 0,
        POST = 1,
        PUT = 2,
    };

    QGitlabClient();
    void setCredentials(const QString &url, const QString &token);
    bool requestIssues(const IssueRequestOptions &options);
    bool editIssue(const int &projectID, const int &issueID, const Issue &newIssue);
    bool createIssue(const int &projectID, const QString &title, const QString &description, const QStringList &labels);
    bool closeIssue(const int &projectID, const int &issueID);
    bool requestListofLabels(const LabelsRequestOptions &options);
    bool requestProjectId(const QUrl &projectUrl);

    bool isBusy() const;

Q_SIGNALS:
    /*!
     * Provides a block/page of issues
     */
    void listOfIssues(QList<Issue>);
    /*!
     * Is send either when fetching data is started. Or when the fething of data ended.
     * The busy property is true, while the fetching is active
     */
    void busyStateChanged(bool);
    /*!
     * Sent after successfully fetching issues
     */
    void issueFetchingDone();
    /*!
     * Sent after successfully fetching labels
     */
    void labelsFetchingDone();

    void listOfLabels(QList<Label>);
    void requestedProjectID(int);
    void connectionError(QString errorString);
    void issueCreated(const Issue &issue);
    void issueClosed();

protected:
    QNetworkReply *sendRequest(ReqType reqType, const QUrl &url);
    /*!
     * \brief requestNextPage makes a request for next page (if any ) for issues or labels
     * \param reply
     * \param options which can be any derived class from RequestOptions
     * \return true if there was another requestable page
     */
    bool requestNextPage(QNetworkReply *reply, const RequestOptions &options);
    int pageNumberFromHeader(QNetworkReply *reply) const;
    int totalPagesFromHeader(QNetworkReply *reply) const;
    int numberHeaderAttribute(QNetworkReply *reply, const QString &headername) const;
    void setBusy(bool busy);
    bool isIssueRequest(QNetworkReply *reply) const;

private:
    QString mUsername;
    UrlComposer mUrlComposer;
    QString mToken;
    QNetworkAccessManager mManager;
    bool m_busy = false;

    void notifyError(QNetworkReply *reply, const QString &text = QString());
};
}

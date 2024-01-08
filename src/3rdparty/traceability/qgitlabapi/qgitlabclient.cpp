#include "qgitlabclient.h"

#include "issuerequestoptions.h"
#include "labelsrequestoptions.h"

#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

namespace gitlab {

const QString kContentType = "application/x-www-form-urlencoded";

QGitlabClient::QGitlabClient() { }

void QGitlabClient::setCredentials(const QString &url, const QString &token)
{
    QUrl api_url(url);
    api_url.setPath("/api/v4");
    mUrlComposer.setBaseURL(api_url.toString());
    mToken = token;
}

bool QGitlabClient::requestIssues(const IssueRequestOptions &options)
{
    if (isBusy()) {
        return true;
    }
    setBusy(true);
    auto reply = sendRequest(QGitlabClient::GET, mUrlComposer.composeGetIssuesUrl(options.mProjectID, options));
    connect(reply, &QNetworkReply::finished, [reply, options, this]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QJsonParseError jsonError;
            auto replyContent = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (QJsonParseError::NoError != jsonError.error) {
                qWarning() << "ERROR: Parsing json data: " << jsonError.errorString();
                Q_EMIT connectionError(reply->errorString());
            } else {
                QList<Issue> issues;
                for (const QJsonValueRef &value : replyContent.array()) {
                    issues.push_back(value.toObject());
                }
                Q_EMIT listOfIssues(issues);
            }

            if (!requestNextPage(reply, options)) {
                setBusy(false);
                Q_EMIT issueFetchingDone();
            }
        } else {
            qDebug() << reply->error() << reply->errorString();
            setBusy(false);
            Q_EMIT connectionError(reply->errorString());
        }
    });
    return false;
}

bool QGitlabClient::editIssue(const int &projectID, const int &issueID, const Issue &newIssue)
{
    if (isBusy()) {
        return true;
    }
    setBusy(true);
    auto reply = sendRequest(QGitlabClient::PUT,
            mUrlComposer.composeEditIssueUrl(projectID, newIssue.mIssueIID, newIssue.mTitle, newIssue.mDescription,
                    newIssue.mAssignee, newIssue.mState_event, newIssue.mLabels));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        setBusy(false);
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
            qDebug() << reply->error() << reply->errorString();
            Q_EMIT connectionError(reply->errorString());
        }
    });
    return false;
}

bool QGitlabClient::createIssue(
        const int &projectID, const QString &title, const QString &description, const QStringList &labels)
{
    if (isBusy()) {
        return true;
    }
    setBusy(true);
    auto reply = sendRequest(
            QGitlabClient::POST, mUrlComposer.composeCreateIssueUrl(projectID, title, description, labels, ""));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        setBusy(false);
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->error() << reply->errorString();
            Q_EMIT connectionError(reply->errorString());
        } else {
            Q_EMIT issueCreated();
        }
    });
    return false;
}

bool QGitlabClient::closeIssue(const int &projectID, const int &issueID)
{
    if (isBusy()) {
        return true;
    }
    setBusy(true);
    static const QString &title = QString();
    static const QString &description = QString();
    static const QString &assignee = QString();
    const QString state_event = "close";
    const QStringList &labels = QStringList();
    auto reply = sendRequest(QGitlabClient::PUT,
            mUrlComposer.composeEditIssueUrl(projectID, issueID, title, description, assignee, state_event, labels));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        setBusy(false);
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
            qDebug() << reply->error() << reply->errorString();
            Q_EMIT connectionError(reply->errorString());
        } else {
            Q_EMIT issueClosed();
        }
    });
    return false;
}

bool QGitlabClient::requestListofLabels(const LabelsRequestOptions &options)
{
    if (isBusy()) {
        return true;
    }
    setBusy(true);
    auto reply = sendRequest(QGitlabClient::GET, mUrlComposer.composeProjectLabelsUrl(options));
    connect(reply, &QNetworkReply::finished, [reply, this, options]() {
        setBusy(false);
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QJsonParseError jsonError;
            auto replyContent = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (QJsonParseError::NoError != jsonError.error) {
                qWarning() << "ERROR: Parsing json data: " << jsonError.errorString();
                Q_EMIT connectionError(reply->errorString());
            } else {
                QList<Label> labels;
                for (const QJsonValueRef &label : replyContent.array()) {
                    labels.push_back(Label(label.toObject()));
                }
                Q_EMIT listOfLabels(labels);
            }
            if (!requestNextPage(reply, options)) {
                setBusy(false);
                Q_EMIT labelsFetchingDone();
            }
        } else {
            qDebug() << reply->error() << reply->errorString();
            Q_EMIT connectionError(reply->errorString());
        }
    });
    return false;
}

bool QGitlabClient::requestProjectId(const QUrl &projectUrl)
{
    if (isBusy()) {
        return true;
    }
    setBusy(true);
    auto projectName = QDir(QUrl(projectUrl).path()).dirName();
    auto reply = sendRequest(QGitlabClient::GET, mUrlComposer.composeProjectUrl(projectName));
    connect(reply, &QNetworkReply::finished, [reply, projectUrl, this]() {
        setBusy(false);
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QJsonParseError jsonError;
            auto replyContent = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (QJsonParseError::NoError != jsonError.error) {
                qWarning() << "ERROR: Parsing json data: " << jsonError.errorString();
                Q_EMIT connectionError(reply->errorString());
            } else {
                auto content = replyContent.array();
                if (!content.isEmpty()) {
                    static const auto ID = "id";
                    for (const auto item : content) {
                        const QString projectUrlStr(projectUrl.toString());
                        const auto &itemObj = item.toObject();
                        const bool objOk = !itemObj.isEmpty() && itemObj.contains("web_url") && itemObj.contains("id");
                        if (objOk && itemObj.value("web_url").toString() == projectUrlStr) {
                            int projectID = itemObj.value("id").toInteger();
                            Q_EMIT requestedProjectID(projectID);
                            break;
                        }
                    }
                }
            }
        } else {
            qDebug() << reply->error() << reply->errorString();
            Q_EMIT connectionError(reply->errorString());
        }
    });
    return false;
}

/*!
 * Returns true, if data is currently fetched from the server
 */
bool QGitlabClient::isBusy() const
{
    return m_busy;
}

QNetworkReply *QGitlabClient::sendRequest(QGitlabClient::ReqType reqType, const QUrl &uri)
{
    QNetworkRequest request(uri);
    request.setRawHeader("PRIVATE-TOKEN", mToken.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, kContentType);

    QNetworkReply *reply;
    switch (reqType) {
    case QGitlabClient::GET: {
        reply = mManager.get(request);
        break;
    }
    case QGitlabClient::POST: {
        reply = mManager.post(request, uri.query(QUrl::FullyEncoded).toUtf8());
        break;
    }
    case QGitlabClient::PUT: {
        reply = mManager.sendCustomRequest(request, "PUT");
        break;
    }
    default: {
        qDebug() << "Unknown request";
    }
    }

    return reply;
}

bool QGitlabClient::requestNextPage(QNetworkReply *reply, const RequestOptions &options)
{
    int page = pageNumberFromHeader(reply);
    const int totalPages = totalPagesFromHeader(reply);
    if (page >= 0 && totalPages >= 0) {
        if (page < totalPages) {
            if (isIssueRequest(reply)) {
                IssueRequestOptions nextPage = dynamic_cast<const IssueRequestOptions &>(options);
                nextPage.mPage = page + 1;
                m_busy = false; // workaround to enable request for the next page
                requestIssues(nextPage);
            } else {
                LabelsRequestOptions nextPage = dynamic_cast<const LabelsRequestOptions &>(options);
                nextPage.mPage = page + 1;
                m_busy = false; // workaround to enable request for the next page
                requestListofLabels(nextPage);
            }
            return true;
        }
    }
    return false;
}

int QGitlabClient::pageNumberFromHeader(QNetworkReply *reply) const
{
    return numberHeaderAttribute(reply, "x-page");
}

int QGitlabClient::totalPagesFromHeader(QNetworkReply *reply) const
{
    return numberHeaderAttribute(reply, "x-total-pages");
}

int QGitlabClient::numberHeaderAttribute(QNetworkReply *reply, const QString &headername) const
{
    if (!reply) {
        return -1;
    }

    const QString pageAttribute = reply->rawHeader(headername.toUtf8());
    if (pageAttribute.isEmpty()) {
        return -1;
    }

    return pageAttribute.toInt();
}

void QGitlabClient::setBusy(bool busy)
{
    if (busy == m_busy) {
        return;
    }

    m_busy = busy;
    Q_EMIT busyStateChanged(m_busy);
}
}

bool gitlab::QGitlabClient::isIssueRequest(QNetworkReply *reply) const
{
    return reply->request().url().query().contains("api/v4/issues");
}

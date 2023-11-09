#include "qgitlabclient.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

namespace {

const QString kContentType = "application/x-www-form-urlencoded";

} // namespace

QGitlabClient::QGitlabClient() { }

void QGitlabClient::setCredentials(const QString &url, const QString &token)
{
    QUrl api_url(url);
    api_url.setPath("/api/v4");
    mUrlComposer.setBaseURL(api_url.toString());
    mToken = token;
}

void QGitlabClient::requestIssues(const QString &projectID, const QString &assignee, const QString &author,
        const QStringList &iids, const int page)
{
    auto reply = SendRequest(QGitlabClient::GET,
            mUrlComposer.ComposeGetIssuesUrl(projectID, assignee, author, iids, "all", "opened", page));
    connect(reply, &QNetworkReply::finished, [reply, projectID, assignee, author, iids, this]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QJsonParseError jsonError;
            auto replyContent = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (QJsonParseError::NoError != jsonError.error) {
                qWarning() << "ERROR: Parsing json data: " << jsonError.errorString();
            } else {
                QList<Issue> issues;
                for (const QJsonValueRef &value : replyContent.array()) {
                    issues.push_back(value.toObject());
                }
                Q_EMIT listOfIssues(issues);
            }

            // Request next page if needed
            int page = pageNumberFromHeader(reply);
            const int totalPages = totalPagesFromHeader(reply);
            if (page >= 0 && totalPages >= 0) {
                if (page < totalPages) {
                    requestIssues(projectID, assignee, author, iids, ++page);
                } else {
                    if (page == totalPages) {
                        Q_EMIT issueFetchingDone();
                    }
                }
            }
        } else {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

void QGitlabClient::requestIssue(const QString &issueId)
{
    static const QString anyAssignee("");
    static const QString anyAuthor("");
    requestIssues(anyAssignee, anyAuthor, { issueId });
}

void QGitlabClient::editIssue(const QString &projectID, const QString &issueID, const Issue &newIssue)
{
    auto reply = SendRequest(QGitlabClient::PUT,
            mUrlComposer.ComposeEditIssueUrl(projectID.toInt(), newIssue.mIssueIID, newIssue.mTitle,
                    newIssue.mDescription, newIssue.mAssignee, newIssue.mState_event, newIssue.mLabels));
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

void QGitlabClient::createIssue(const QString &projectID, const Issue &issue)
{
    auto reply = SendRequest(QGitlabClient::POST,
            mUrlComposer.ComposeCreateIssueUrl(projectID, issue.mTitle, issue.mDescription, issue.mAssignee));
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

void QGitlabClient::requestListofLabels(const QString &projectID, const QString &with_counts, const QString &search)
{
    auto reply = SendRequest(QGitlabClient::GET, mUrlComposer.ComposeProjectLabelsUrl(projectID, with_counts, search));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
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
        } else {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

void QGitlabClient::requestProjectIdByName(const QString &projectName)
{
    auto reply = SendRequest(QGitlabClient::GET, mUrlComposer.ComposeProjectUrl(projectName));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QJsonParseError jsonError;
            auto replyContent = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (QJsonParseError::NoError != jsonError.error) {
                qWarning() << "ERROR: Parsing json data: " << jsonError.errorString();
                Q_EMIT connectionError(reply->errorString());
            } else {
                QString projectID;
                auto content = replyContent.array();
                if (!content.isEmpty()) {
                    static const auto ID = "id";
                    auto projectID = QString::number(content.at(0).toObject().value("id").toInteger());
                    Q_EMIT requestedProjectID(projectID);
                }
            }
        } else {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

QNetworkReply *QGitlabClient::SendRequest(QGitlabClient::ReqType reqType, const QUrl &uri)
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

int QGitlabClient::pageNumberFromHeader(QNetworkReply *reply) const
{
    return numberHeaderAttribute(reply, "x-page");
}

int QGitlabClient::totalPagesFromHeader(QNetworkReply *reply) const
{
    return numberHeaderAttribute(reply, "x-total-pages");
}

int QGitlabClient::numberHeaderAttribute(QNetworkReply *reply, const QByteArray &headername) const
{
    if (!reply) {
        return -1;
    }

    const QByteArray pageAttribute = reply->rawHeader(headername);
    if (pageAttribute.isEmpty()) {
        return -1;
    }

    return pageAttribute.toInt();
}

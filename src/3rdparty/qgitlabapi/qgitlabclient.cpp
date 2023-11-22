#include "qgitlabclient.h"

#include "issuerequestoptions.h"

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

void QGitlabClient::requestIssues(const QString &projectID, const IssueRequestOptions &options)
{
    auto reply = sendRequest(QGitlabClient::GET, mUrlComposer.composeGetIssuesUrl(projectID, options));
    connect(reply, &QNetworkReply::finished, [reply, projectID, options, this]() {
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

            // Request next page if needed
            int page = pageNumberFromHeader(reply);
            const int totalPages = totalPagesFromHeader(reply);
            if (page >= 0 && totalPages >= 0) {
                if (page < totalPages) {
                    IssueRequestOptions nextPage = options;
                    nextPage.mPage += 1;
                    requestIssues(projectID, nextPage);
                } else {
                    if (page == totalPages) {
                        Q_EMIT issueFetchingDone();
                    }
                }
            }
        } else {
            qDebug() << reply->error() << reply->errorString();
            Q_EMIT connectionError(reply->errorString());
        }
    });
}

void QGitlabClient::editIssue(const QString &projectID, const QString &issueID, const Issue &newIssue)
{
    auto reply = sendRequest(QGitlabClient::PUT,
            mUrlComposer.composeEditIssueUrl(projectID.toInt(), newIssue.mIssueIID, newIssue.mTitle,
                    newIssue.mDescription, newIssue.mAssignee, newIssue.mState_event, newIssue.mLabels));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
            qDebug() << reply->error() << reply->errorString();
            Q_EMIT connectionError(reply->errorString());
        }
    });
}

void QGitlabClient::createIssue(const QString &projectID, const QString &title, const QString &description)
{
    auto reply =
            sendRequest(QGitlabClient::POST, mUrlComposer.composeCreateIssueUrl(projectID, title, description, ""));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->error() << reply->errorString();
            Q_EMIT connectionError(reply->errorString());
        }
        else
        {
            Q_EMIT issueCreated();
        }
    });
}

void QGitlabClient::requestListofLabels(const QString &projectID, const QString &with_counts, const QString &search)
{
    auto reply = sendRequest(QGitlabClient::GET, mUrlComposer.composeProjectLabelsUrl(projectID, with_counts, search));
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
            Q_EMIT connectionError(reply->errorString());
        }
    });
}

void QGitlabClient::requestProjectId(const QUrl &projectUrl)
{
    auto projectName = QDir(QUrl(projectUrl).path()).dirName();
    auto reply = sendRequest(QGitlabClient::GET, mUrlComposer.composeProjectUrl(projectName));
    connect(reply, &QNetworkReply::finished, [reply,projectUrl, this]() {
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
                    QString projectID;
                    for (const auto item: content)
                    {
                        if (item.toObject().value("web_url").toString() == projectUrl.toString())
                        {
                            projectID = QString::number(item.toObject().value("id").toInteger());
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

}

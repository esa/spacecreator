#include "GitLabTransport.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>

namespace {

const QString kContentType = "application/x-www-form-urlencoded";

} // namespace

GitLabTransport::GitLabTransport(const QString &url, const QString &token)
    : mUrlComposer(url)
    , mToken(token)

{}

void GitLabTransport::RequestListofIssues(const QString &assignee,
                                     const QString &author,
                                     const QString &iids)
{
    auto reply = SendRequest(GitLabTransport::GET, mUrlComposer.ComposeGetIssuesUrl(assignee, author, iids));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QJsonParseError jsonError;
            auto replyContent = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (QJsonParseError::NoError != jsonError.error) {
                qWarning() << "ERROR: Parsing json data: " << jsonError.errorString();
            } else {
                QList<Issue> issues;
                foreach (auto value, replyContent.array()) {
                    issues.push_back(value.toObject());
                }
                emit ListOfIssues(issues);
            }
        } else {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

void GitLabTransport::RequestIssue(const QString &issueID)
{
    static const QString anyAssignee("");
    static const QString anyAuthor("");

    RequestListofIssues(anyAssignee, anyAuthor, issueID);
}

void GitLabTransport::RequestListofLabels(const QString &projectID, const QString &with_counts, const QString &search)
{
    auto reply = SendRequest(GitLabTransport::GET,
                             mUrlComposer.ComposeProjectLabelsUrl(projectID, with_counts, search));
    connect(reply, &QNetworkReply::finished, [reply, this]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
            QJsonParseError jsonError;
            auto replyContent = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (QJsonParseError::NoError != jsonError.error) {
                qWarning() << "ERROR: Parsing json data: " << jsonError.errorString();
                emit ConnectionError(reply->errorString());
            } else {
                QList<Label> labels;
                foreach (auto label, replyContent.array()) {
                    labels.push_back(Label(label.toObject()));
                }
                emit ListOfLabels(labels);
            }
        } else {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

void GitLabTransport::CreateIssue(const QString &projectID, const Issue &issue)
{
    auto reply = SendRequest(GitLabTransport::POST,
                             mUrlComposer.ComposeCreateIssueUrl(projectID,
                                                                issue.mTitle,
                                                                issue.mDescription,
                                                                issue.mAssignee));
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

void GitLabTransport::EditIssue(const QString &projectID, const Issue &newIssue)
{
    auto reply = SendRequest(GitLabTransport::PUT,
                             mUrlComposer.ComposeEditIssueUrl(projectID,
                                                              newIssue.mIssueID,
                                                              newIssue.mTitle,
                                                              newIssue.mDescription,
                                                              newIssue.mAssignee,
                                                              newIssue.mState_event,
                                                              newIssue.mLabels));
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200) {
            qDebug() << reply->error() << reply->errorString();
        }
    });
}

QNetworkReply *GitLabTransport::SendRequest(GitLabTransport::ReqType reqType, const QUrl &uri)
{
    QNetworkRequest request(uri);
    request.setRawHeader("PRIVATE-TOKEN", mToken.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, kContentType);

    QNetworkReply *reply;
    switch(reqType)
    {
    case GitLabTransport::GET:
    {
        reply = mManager.get(request);
        break;
    }
    case GitLabTransport::POST:
    {
        reply = mManager.post(request, uri.query(QUrl::FullyEncoded).toUtf8());
        break;
    }
    case GitLabTransport::PUT:
    {
        reply = mManager.sendCustomRequest(request, "PUT");
        break;
    }
    default:
    {
        qDebug() << "Unknown request";
    }
    }

    return reply;
}

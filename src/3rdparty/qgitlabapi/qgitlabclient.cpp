#include "qgitlabclient.h"
#include <QDebug>

#include <QUrl>

QGitlabClient::QGitlabClient()
{
    mGitlab = std::make_unique<GitLabTransport>();
    connect(mGitlab.get(), &GitLabTransport::ListOfIssues, this, &QGitlabClient::listOfIssues);
    connect(mGitlab.get(), &GitLabTransport::ListOfLabels, this, &QGitlabClient::listOfLabels);
    connect(mGitlab.get(), &GitLabTransport::RequestedProjectID, this, &QGitlabClient::RequestedProjectID);
}

void QGitlabClient::setCredentials(const QString &url, const QString &token)
{
    if (mGitlab)
    {
        QUrl api_url(url);
        api_url.setPath("/api/v4");
        mGitlab->setCredentials(api_url.toString(), token);
    }
}

void QGitlabClient::requestIssues(const QString &projectID,
                                  const QString &assignee,
                                  const QString &author,
                                  const QStringList &iids)
{
    mGitlab->RequestListofIssues(projectID, assignee, author, iids);
}

void QGitlabClient::requestIssue(const QString &iid)
{
    mGitlab->RequestIssue(iid);
}

void QGitlabClient::editIssue(const QString &projectID,
                              const QString &issueID,
                              const Issue &newIssue)
{
    mGitlab->EditIssue(projectID, newIssue);
}

void QGitlabClient::createIssue(const QString &projectID, const Issue &issue)
{
    mGitlab->CreateIssue(projectID, issue);
}

void QGitlabClient::RequestListofLabels(const QString &projectID,
                                        const QString &with_counts,
                                        const QString &search)
{
    mGitlab->RequestListofLabels(projectID, with_counts, search);
}

void QGitlabClient::requestProjectIdByName(const QString &projectName)
{
   mGitlab->requestProjectIdByName(projectName);
}

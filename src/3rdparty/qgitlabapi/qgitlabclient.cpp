#include "qgitlabclient.h"
#include <QDebug>

QGitlabClient::QGitlabClient()
{
    mGitlab = std::make_unique<GitLabTransport>();
    connect(mGitlab.get(), &GitLabTransport::ListOfIssues, this, &QGitlabClient::listOfIssues);
    connect(mGitlab.get(), &GitLabTransport::ListOfLabels, this, &QGitlabClient::listOfLabels);
}

void QGitlabClient::setCredentials(const QString &url, const QString &token)
{
    if (mGitlab)
    {
        mGitlab->setCredentials(url, token);
    }
}

void QGitlabClient::requestIssues(const QString &assignee,
                                  const QString &author,
                                  const QStringList &iids)
{
    mGitlab->RequestListofIssues(assignee, author, iids);
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

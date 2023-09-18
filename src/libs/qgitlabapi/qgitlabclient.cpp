#include "qgitlabclient.h"
#include <QDebug>

QGitlabClient::QGitlabClient(const QString &url, const QString &token)
{
    mGitlab = std::make_unique<GitLabTransport>(url, token);
    connect(mGitlab.get(), &GitLabTransport::ListOfIssues, this, &QGitlabClient::listOfIssues);
    connect(mGitlab.get(), &GitLabTransport::ListOfLabels, this, &QGitlabClient::listOfLabels);
}

void QGitlabClient::requestIssues(const QString &assignee,
                                  const QString &author,
                                  const QString &iids)
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

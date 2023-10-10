#include "requirementsmanager.h"

RequirementsManager::RequirementsManager(REPO_TYPE repoType, const QString &url, const QString &token):
    mRepoType(repoType)
{
    switch(repoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        gitlabClient = new QGitlabClient(url, token);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::requestRequirements(const QString &assignee, const QString &author, const QStringList &iids) const
{
    switch(mRepoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        gitlabClient->requestIssues(assignee, author, iids);
        connect(gitlabClient, &QGitlabClient::listOfIssues, this, &RequirementsManager::listOfIssues, Qt::UniqueConnection);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::createRequirement(const QString &projectID, const Issue &issue) const
{
    switch(mRepoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        gitlabClient->createIssue(projectID, issue);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::RequestListofLabels(const QString &projectID, const QString &with_counts, const QString &search)
{
    switch(mRepoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        gitlabClient->RequestListofLabels(projectID, with_counts, search);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::editIssue(const QString &projectID, const QString &issueID, const Issue &newIssue)
{
    switch(mRepoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        gitlabClient->editIssue(projectID, issueID, newIssue);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

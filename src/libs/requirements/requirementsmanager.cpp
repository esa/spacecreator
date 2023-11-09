#include "requirementsmanager.h"

#include <issuerequestoptions.h>

RequirementsManager::RequirementsManager(REPO_TYPE repoType):
    mRepoType(repoType)
{
    switch(repoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        gitlabClient = new QGitlabClient();
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::setCredentials(const QString &url, const QString &token)
{
    auto _url = QUrl(url);
    gitlabClient->setCredentials(_url.scheme() + "://" + _url.host(), token);
}

void RequirementsManager::requestRequirements(
        const QString &projectID, const QString &assignee, const QString &author) const
{
    switch(mRepoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        IssueRequestOptions options;
        options.mAssignee = assignee.toUtf8();
        options.mAuthor = author.toUtf8();
        gitlabClient->requestIssues(projectID, options);
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
        gitlabClient->requestListofLabels(projectID, with_counts, search);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::RequestProjectID(const QString &projectName)
{
    switch(mRepoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        gitlabClient->requestProjectIdByName(projectName);
        connect(gitlabClient, &QGitlabClient::requestedProjectID, this, &RequirementsManager::RequestedProjectID, Qt::UniqueConnection);
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

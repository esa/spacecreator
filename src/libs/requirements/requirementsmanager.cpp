/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "requirementsmanager.h"

#include "gitlab/gitlabrequirements.h"

#include <issue.h>
#include <issuerequestoptions.h>
#include <qgitlabclient.h>

struct RequirementsManager::RequirementsManagerPrivate {
    RequirementsManagerPrivate(RequirementsManager::REPO_TYPE repoType)
        : mRepoType(repoType)
    {
    }

    REPO_TYPE mRepoType;
    std::unique_ptr<gitlab::QGitlabClient> gitlabClient;
    std::unique_ptr<requirement::GitLabRequirements> gitlabRequirements;
};

RequirementsManager::RequirementsManager(REPO_TYPE repoType)
    : d(std::make_unique<RequirementsManagerPrivate>(repoType))
{
    switch(repoType)
    {
    case(REPO_TYPE::GITLAB):
    {
        d->gitlabClient = std::make_unique<gitlab::QGitlabClient>();
        d->gitlabRequirements = std::make_unique<requirement::GitLabRequirements>();
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::listOfIssues, d->gitlabRequirements.get(),
                &requirement::GitLabRequirements::listOfIssues);
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::connectionError, this,
                &RequirementsManager::connectionError);
        connect(d->gitlabRequirements.get(), &requirement::GitLabRequirements::listOfRequirements, this,
                &RequirementsManager::listOfRequirements);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

RequirementsManager::~RequirementsManager() { }

void RequirementsManager::setCredentials(const QString &url, const QString &token)
{
    auto _url = QUrl(url);
    d->gitlabClient->setCredentials(_url.scheme() + "://" + _url.host(), token);
}

void RequirementsManager::requestRequirements(
        const QString &projectID, const QString &assignee, const QString &author) const
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        gitlab::IssueRequestOptions options;
        options.mAssignee = assignee.toUtf8();
        options.mAuthor = author.toUtf8();
        options.mLabels = { "requirement" };
        d->gitlabClient->requestIssues(projectID, options);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::createRequirement(
        const QString &projectID, const QString &title, const QString &description) const
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        d->gitlabClient->createIssue(projectID, title, description);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::requestProjectID(const QString &projectName)
{
    switch (d->mRepoType) {
    case(REPO_TYPE::GITLAB):
    {
        d->gitlabClient->requestProjectIdByName(projectName);
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::requestedProjectID, this,
                &RequirementsManager::requestedProjectID, Qt::UniqueConnection);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

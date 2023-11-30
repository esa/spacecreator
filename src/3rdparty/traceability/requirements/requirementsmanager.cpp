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

#include <QDir>
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

RequirementsManager::RequirementsManager(REPO_TYPE repoType, QObject *parent)
    : QObject(parent)
    , d(std::make_unique<RequirementsManagerPrivate>(repoType))
{
    switch (repoType) {
    case (REPO_TYPE::GITLAB): {
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
    if (m_projectUrl == url && token == m_token) {
        Q_EMIT connectionReady();
        return;
    }

    m_projectUrl = url;
    m_token = token;
    m_projectID.clear();

    QUrl _url;
    _url.setScheme("https");
    _url.setHost(QUrl(url).host());
    _url.setPath("/api/v4/");

    d->gitlabClient->setCredentials(_url.scheme() + "://" + _url.host(), token);
    requestProjectID(url);
}

void RequirementsManager::requestRequirements(const QString &assignee, const QString &author)
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        gitlab::IssueRequestOptions options;
        options.mAssignee = assignee.toUtf8();
        options.mAuthor = author.toUtf8();
        options.mLabels = { "requirement" };
        Q_EMIT startfetchingRequirements();
        d->gitlabClient->requestIssues(m_projectID, options);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::createRequirement(
        const QString &title, const QString &reqIfId, const QString &description) const
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        const QString descr = QString("#reqid %1\n\n%2").arg(reqIfId, description);
        d->gitlabClient->createIssue(m_projectID, title, descr);
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::issueCreated, this, &RequirementsManager::requirementCreated,
                Qt::UniqueConnection);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

void RequirementsManager::requestProjectID(const QUrl &url)
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        d->gitlabClient->requestProjectId(url);
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::requestedProjectID, this,
                &RequirementsManager::setProjectID, Qt::UniqueConnection);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

QString RequirementsManager::projectID() const
{
    return m_projectID;
}

void RequirementsManager::setProjectID(const QString &newProjectID)
{
    if (m_projectID == newProjectID)
        return;
    m_projectID = newProjectID;
    Q_EMIT connectionReady();
}

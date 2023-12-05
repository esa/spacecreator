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

namespace requirement {

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
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::busyStateChanged, this,
                &RequirementsManager::busyChanged);
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::requestedProjectID, this,
                &RequirementsManager::setProjectID);
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::issueCreated, this,
                &RequirementsManager::requirementCreated);
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::issueClosed, this,
                &RequirementsManager::requirementClosed);
        connect(d->gitlabRequirements.get(), &requirement::GitLabRequirements::listOfRequirements, this,
                &RequirementsManager::listOfRequirements);
        break;
    }
    default:
        qDebug() << "unknown repository type";
    }
}

RequirementsManager::~RequirementsManager() { }

/*!
 * Sets the url and authentification credentials to be able to load data from the server.
 * It does trigger a re-load of all requirements after the data is validated by the server (project ID set).
 * \param url The URL of the project to fetch the requirements
 * \param token The authentikation token for the server
 * \return Returns true when everything went well.
 */
bool RequirementsManager::setCredentials(const QString &url, const QString &token)
{
    if (m_projectUrl == url && token == m_token) {
        return true;
    }

    m_projectUrl = url;
    m_token = token;
    m_projectID = -1;

    QUrl _url;
    _url.setScheme("https");
    _url.setHost(QUrl(url).host());
    _url.setPath("/api/v4/");

    d->gitlabClient->setCredentials(_url.scheme() + "://" + _url.host(), token);
    return requestProjectID(url);
}

/*!
 * URL of the project to load the requirements data from
 */
QString RequirementsManager::projectUrl() const
{
    return m_projectUrl.toString();
}

/*!
 * The token used to authenticate on the server
 */
const QString &RequirementsManager::token() const
{
    return m_token;
}

/*!
 * Returns true when there is currently a request to load data running
 */
bool RequirementsManager::isBusy() const
{
    switch (d->mRepoType) {

    case (REPO_TYPE::GITLAB):
        return d->gitlabClient->isBusy();
    }
    return false;
}

/*!
 * Returns the internal ID of the project
 */
const int &RequirementsManager::projectID() const
{
    return m_projectID;
}

/*!
 * Starts a request to load all requirements from the server. The requirements will be delivered by the
 * listOfRequirements signal.
 * \return Returns true when everything went well.
 */
bool RequirementsManager::requestAllRequirements()
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        gitlab::IssueRequestOptions options;
        options.mLabels = { "requirement" };

        const bool wasBusy = d->gitlabClient->requestIssues(m_projectID, options);
        if (wasBusy) {
            return false;
        }
        Q_EMIT startFetchingRequirements();
        return true;
    }
    default:
        qDebug() << "unknown repository type";
    }
    return false;
}

/*!
 * Creates a new requirement on the server
 * \param title The title of the requirement
 * \param reqIfId The ID of that requirement
 * \param description The content of the requirements
 * \return Returns true when everything went well.
 */
bool RequirementsManager::createRequirement(
        const QString &title, const QString &reqIfId, const QString &description) const
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        const QString descr = QString("#reqid %1\n\n%2").arg(reqIfId, description);
        const bool wasBusy = d->gitlabClient->createIssue(m_projectID, title, descr);
        return !wasBusy;
    }
    default:
        qDebug() << "unknown repository type";
    }
    return false;
}

bool RequirementsManager::removeRequirement(const Requirement &requirement) const
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        const bool wasBusy = d->gitlabClient->closeIssue(m_projectID, requirement.m_issueID);
        return !wasBusy;
    }
    default:
        qDebug() << "unknown repository type";
    }
    return false;
}

void RequirementsManager::setProjectID(const int &newProjectID)
{
    if (m_projectID == newProjectID) {
        return;
    }
    m_projectID = newProjectID;
    Q_EMIT projectIDChanged();
}

bool RequirementsManager::requestProjectID(const QUrl &url)
{
    switch (d->mRepoType) {
    case (REPO_TYPE::GITLAB): {
        const bool wasBusy = d->gitlabClient->requestProjectId(url);
        return !wasBusy;
    }
    default:
        qDebug() << "unknown repository type";
    }
    return false;
}

}

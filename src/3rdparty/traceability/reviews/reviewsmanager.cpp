/*
   Copyright (C) 2024 European Space Agency - <maxime.perrotin@esa.int>

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

#include "reviewsmanager.h"

#include "gitlab/gitlabreviews.h"
#include "issuerequestoptions.h"
#include "issuesmanagerprivate.h"

namespace reviews {

struct ReviewsManager::ReviewsManagerPrivate : public tracecommon::IssuesManagerPrivate {
    ReviewsManagerPrivate(tracecommon::IssuesManager::REPO_TYPE rType)
        : IssuesManagerPrivate(rType)
    {
        switch (repoType) {
        case REPO_TYPE::GITLAB:
            gitlabReviews = std::make_unique<reviews::GitLabReviews>();
        }
    }

    std::unique_ptr<reviews::GitLabReviews> gitlabReviews;
};

ReviewsManager::ReviewsManager(REPO_TYPE repoType, QObject *parent)
    : tracecommon::IssuesManager(parent)
    , d(std::make_unique<ReviewsManagerPrivate>(repoType))
{
    init(d.get());

    switch (d->repoType) {
    case (REPO_TYPE::GITLAB): {
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::listOfIssues, this,
                [this](const QList<gitlab::Issue> &issues) { Q_EMIT d->gitlabReviews->convertIssues(issues); });
        connect(d->gitlabClient.get(), &gitlab::QGitlabClient::issueFetchingDone, this,
                &ReviewsManager::fetchingReviewsEnded);
        connect(d->gitlabReviews.get(), &reviews::GitLabReviews::listOfReviews, this, &ReviewsManager::listOfReviews);
    }
    }
}

ReviewsManager::~ReviewsManager() { }

bool ReviewsManager::requestAllReviews()
{
    switch (d->repoType) {
    case (REPO_TYPE::GITLAB): {
        gitlab::IssueRequestOptions options;
        options.mProjectID = m_projectID;
        options.mLabels = { k_reviewsTypeLabel };
        const bool wasBusy = d->gitlabClient->requestIssues(options);
        if (wasBusy) {
            return false;
        }
        Q_EMIT startingFetchingReviews();
        return true;
    }
    default:
        qDebug() << "unknown repository type";
    }
    return false;
}

} // namespace reviews

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

#pragma once

#include "issuesmanager.h"
#include "review.h"

#include <memory>

namespace reviews {

/*!
 * The ReviewsManager is responsible to communicate to the server/storage that handles the reviews
 */
class ReviewsManager : public tracecommon::IssuesManager
{
    Q_OBJECT
public:
    explicit ReviewsManager(
            REPO_TYPE repoType = tracecommon::IssuesManager::REPO_TYPE::GITLAB, QObject *parent = nullptr);
    ~ReviewsManager();

    bool requestAllReviews();

Q_SIGNALS:
    void startingFetchingReviews();
    void fetchingReviewsEnded();
    void listOfReviews(const QList<reviews::Review> &);

private:
    class ReviewsManagerPrivate;
    std::unique_ptr<ReviewsManagerPrivate> d;
};

} // namespace reviews

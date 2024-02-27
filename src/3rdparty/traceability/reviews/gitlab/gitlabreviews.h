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

#include "issue.h"
#include "review.h"

#include <QObject>

namespace reviews {

const static QString k_reviewsTypeLabel = "review";

/*!
 * \brief The GitLabReviews class
 */
class GitLabReviews : public QObject
{
    Q_OBJECT
public:
    QList<Review> convertIssues(const QList<gitlab::Issue> &issues);

    static Review reviewFromIssue(const gitlab::Issue &issue);
    static QString parseRevIfId(const gitlab::Issue &issue);

Q_SIGNALS:
    void listOfReviews(const QList<Review> &);
};

}

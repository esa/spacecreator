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

#pragma once

#include "reviewsmodelbase.h"

#include <QPointer>

namespace scs {
class SpaceCreatorProject;
}

namespace spctr {

/*!
 * Show all reviews that do have a reference in that model.
 * Others are not shown, as one review-URL might be used for seferal models.
 */
class AllReviewsModel : public reviews::ReviewsModelBase
{
    Q_OBJECT
public:
    explicit AllReviewsModel(
            scs::SpaceCreatorProject *project, reviews::ReviewsManager *manager, QObject *parent = nullptr);

    void setReviews(const QList<reviews::Review> &reviews) override;
    void addReviews(const QList<reviews::Review> &reviews) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QString componentForReview(const QString &revId) const;

    QPointer<scs::SpaceCreatorProject> m_project;
    QList<reviews::Review> m_allReviews;
};

} // namespace spctr

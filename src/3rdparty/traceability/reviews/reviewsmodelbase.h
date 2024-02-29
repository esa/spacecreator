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
along with this program. If not, see
<https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include "review.h"
#include "tracecommonmodelbase.h"

#include <QList>

namespace reviews {

/*!
 * \brief Base model to hold reviews for a Qt view
 */
class ReviewsModelBase : public tracecommon::TraceCommonModelBase
{
    Q_OBJECT

public:
    enum RoleNames
    {
        ReviewIdRole = tracecommon::TraceCommonModelBase::UserRole + 1,
    };

    enum HEADER_SECTIONS
    {
        TITLE = 0,
        AUTHOR
    };

    explicit ReviewsModelBase(QObject *parent = nullptr);

    virtual void clear();

    void setReviews(const QList<Review> &reviews);
    void addReviews(const QList<Review> &reviews);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Review reviewFromIndex(const QModelIndex &idx) const;

    bool reviewIDExists(const QString &revID) const;

protected:
    QList<Review> m_reviews;
};

} // namespace requirement

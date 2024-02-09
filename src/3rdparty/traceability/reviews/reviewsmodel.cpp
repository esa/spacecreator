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

#include "reviewsmodel.h"

namespace reviews {

ReviewsModel::ReviewsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void ReviewsModel::clear()
{
    setReviews({});
}

/*!
 * Replaces the set of existing reviews with the given one
 */
void ReviewsModel::setReviews(const QList<Review> &reviews)
{
    beginResetModel();
    m_reviews = reviews;
    endResetModel();
}

/*!
 * Appends the given \a reviews to the existing ones
 */
void ReviewsModel::addReviews(const QList<Review> &reviews)
{
    beginInsertRows(QModelIndex(), m_reviews.size(), m_reviews.size() + reviews.size() - 1);
    m_reviews.append(reviews);
    endInsertRows();
}

QVariant ReviewsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section == TITLE) {
            return tr("Title");
        }
    }

    return {};
}

int ReviewsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_reviews.size();
}

int ReviewsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ReviewsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_reviews.size()) {
        return QVariant();
    }

    if (role == ReviewsModel::RoleNames::IssueLinkRole) {
        return m_reviews[index.row()].m_link;
    }

    if (role == Qt::DisplayRole) {
        return m_reviews[index.row()].m_longName;
    }
    if (role == Qt::ToolTipRole) {
        return m_reviews[index.row()].m_description;
    }

    if (role == ReviewsModel::RoleNames::IssueIdRole) {
        return m_reviews[index.row()].m_issueID;
    }

    if (role == ReviewsModel::RoleNames::TagsRole) {
        return m_reviews[index.row()].m_tags;
    }

    if (role == ReviewsModel::RoleNames::DetailDescriptionRole) {
        return m_reviews[index.row()].m_description;
    }

    return QVariant();
}

} // namespace requirement

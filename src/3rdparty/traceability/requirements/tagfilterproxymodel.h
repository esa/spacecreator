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

#include <QSortFilterProxyModel>

namespace requirement {

/*!
 * A filter model to filter a requirement model for tags.
 * All data is shown that has at least one of the tags to filter for.
 * If no tag is set for filtering, all data is shown.
 */
class TagFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit TagFilterProxyModel(QObject *parent = nullptr);

    void addTag(const QString &tag);
    void removeTag(const QString &tag);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QStringList m_tags;
};

} // namespace requirement

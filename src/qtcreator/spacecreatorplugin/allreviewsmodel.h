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

class AllReviewsModel : public reviews::ReviewsModelBase
{
    Q_OBJECT
public:
    explicit AllReviewsModel(scs::SpaceCreatorProject *project, QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QString componentForReview(const QString &revId) const;

    QPointer<scs::SpaceCreatorProject> m_project;
};

} // namespace spctr

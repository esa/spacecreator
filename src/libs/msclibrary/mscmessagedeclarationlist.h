/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscmessagedeclaration.h"

#include <qobjectlistmodelt.h>

namespace msc {

class MscMessageDeclarationList : public QObjectListModelT<MscMessageDeclaration *>
{
    Q_OBJECT
public:
    MscMessageDeclarationList(QObject *parent = nullptr);

    void trackObject(const QObject *obj, const bool on) override;

    QVariant data(const QModelIndex &index, int role) const override;

    MscMessageDeclarationList *clone() const;

    void setObjectList(const QList<MscMessageDeclaration *> &objects) override;
};

} // namespace msc

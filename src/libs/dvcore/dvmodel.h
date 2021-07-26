/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dvobject.h"
#include "vemodel.h"

#include <QList>
#include <QObject>
#include <QVector>
#include <memory>

#pragma once

namespace dvm {
class DVConnection;
class DVDevice;
class DVFunction;
struct DVModelPrivate;
class DVNode;

class DVModel : public shared::VEModel
{
    Q_OBJECT
public:
    explicit DVModel(QObject *parent = nullptr);
    ~DVModel() override;

    DVObject *getObject(const shared::Id &id) const override;
    DVObject *getObjectByName(const QString &name, DVObject::Type type, Qt::CaseSensitivity caseSensitivity) const;

    QList<DVConnection *> connections(DVDevice *device) const;
    QList<DVFunction *> functions(DVNode *node) const;
};

} // namespace dvm

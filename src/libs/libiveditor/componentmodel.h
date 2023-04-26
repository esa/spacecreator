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

#include "common.h"

#include <QStandardItemModel>
#include <memory>

class IVObject;

struct ComponentModelPrivate;
class ComponentModel : public QStandardItemModel
{
public:
    explicit ComponentModel(QObject *parent = nullptr);
    ~ComponentModel() override = default;

    IVObject *getObject(const shared::Id &id);
    bool load(const QString &path);
    bool loadComponent(const QString &path);

private:
    const std::unique_ptr<ComponentModelPrivate> d;
};

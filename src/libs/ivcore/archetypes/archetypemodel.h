/*
  Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "archetypeobject.h"
#include "vemodel.h"

#include <QAbstractItemModel>
#include <QVector>
#include <conversion/common/model.h>
#include <memory>

namespace shared {
}
namespace ivm {

struct ArchetypeModelPrivate;
class ArchetypeModel : public shared::VEModel
{
    Q_OBJECT
public:
    explicit ArchetypeModel(QObject *parent = nullptr);
    ~ArchetypeModel() override;

    void setRootObject(shared::Id rootId);
    ArchetypeObject *rootObject() const;
    shared::Id rootObjectId() const;

    ArchetypeObject *getObject(const shared::Id &id) const override;
    ArchetypeObject *getObjectByName(const QString &name, ArchetypeObject::Type type = ArchetypeObject::Type::Unknown,
            Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;

    void clear() override;

Q_SIGNALS:
    void rootObjectChanged(shared::Id rootId);

private:
    std::unique_ptr<ArchetypeModelPrivate> m_privateModel;
};

}

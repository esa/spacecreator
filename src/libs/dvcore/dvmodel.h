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

namespace dvm {
struct DVModelPrivate;

class DVModel : public shared::VEModel
{
    Q_OBJECT
public:
    explicit DVModel(QObject *parent = nullptr);
    ~DVModel() override;

    bool addObject(DVObject *obj);
    bool removeObject(DVObject *obj);

    void setRootObject(shared::Id rootId);
    DVObject *rootObject() const;
    shared::Id rootObjectId() const;

    DVObject *getObject(const shared::Id &id) const;

    QList<DVObject *> visibleObjects() const;
    QList<DVObject *> visibleObjects(shared::Id rootId) const;

    void clear();

public Q_SLOTS:
    void initFromObjects(const QVector<dvm::DVObject *> &objects);
    void addObjects(const QVector<dvm::DVObject *> &objects);

Q_SIGNALS:
    void objectsAdded(const QVector<dvm::DVObject *> &object);
    void objectRemoved(dvm::DVObject *object);
    void rootObjectChanged(shared::Id rootId);
    void modelReset();

private:
    bool addObjectImpl(DVObject *obj);

    const std::unique_ptr<DVModelPrivate> d;
};

} // namespace deploy

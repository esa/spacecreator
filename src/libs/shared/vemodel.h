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

#pragma once

#include "common.h"

#include <QObject>
#include <QVector>
#include <memory>

namespace shared {
class VEObject;
struct VEModelPrivate;

class VEModel : public QObject
{
    Q_OBJECT
public:
    explicit VEModel(QObject *parent = nullptr);
    ~VEModel() override;

    virtual bool addObject(VEObject *obj);
    virtual bool removeObject(VEObject *obj);
    virtual VEObject *getObject(const shared::Id &id) const;
    virtual void clear();

    QList<shared::Id> objectsOrder() const;
    QHash<shared::Id, VEObject *> objects() const;

public:
    template<typename T>
    void initFromObjects(const QVector<T> &objects, QStringList *warnings = nullptr)
    {
        clear();
        addObjects(objects, warnings);
    }

    template<typename T>
    void addObjects(const QVector<T> &objects, QStringList *warnings = nullptr)
    {
        QVector<T> addedObjects;
        for (auto obj : objects) {
            if (addObjectImpl(obj)) {
                addedObjects.append(obj);
            }
        }

        QVector<shared::Id> ids;
        auto it = addedObjects.begin();
        while (it != addedObjects.end()) {
            if (T obj = *it) {
                QString warning;
                if (!obj->postInit(&warning)) {
                    if (warnings) {
                        warnings->append(warning);
                    }
                    if (removeObject(obj)) {
                        it = addedObjects.erase(it);
                        delete (obj);
                        continue;
                        ;
                    }
                } else {
                    ids.append(obj->id());
                }
            }
            ++it;
        }

        if (!addedObjects.isEmpty()) {
            Q_EMIT objectsAdded(ids);
        }
    }

Q_SIGNALS:
    void objectsAdded(const QVector<shared::Id> &objectsIds);
    void objectRemoved(shared::Id objectId);
    void modelReset();

protected:
    virtual bool addObjectImpl(VEObject *obj);

private:
    const std::unique_ptr<VEModelPrivate> d;
};

} // namespace shared

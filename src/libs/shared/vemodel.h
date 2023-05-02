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

#include "../common/exceptions/inconsistentmodelexception.h"
#include "common.h"
#include "entityattribute.h"

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

    bool isEmpty() const;

    virtual bool addObject(VEObject *obj);
    virtual bool removeObject(VEObject *obj);
    virtual VEObject *getObject(const shared::Id &id) const;
    virtual void clear();

    /**
     * @brief Lookup for an object that has an attribute `attrName` with `value` set
     */
    virtual VEObject *getObjectByAttributeValue(const QString &attrName, const QVariant &value) const;

    const QList<shared::Id> &objectsOrder() const;
    const QHash<shared::Id, VEObject *> &objects() const;

    void setExtAttributes(const QHash<shared::Id, EntityAttributes> &attrs);
    QHash<shared::Id, EntityAttributes> extAttributes() const;
    EntityAttributes extEntityAttributes(const shared::Id &id) const;

public:
    template<typename C, typename T = std::decay_t<decltype(*std::begin(std::declval<C>()))>,
            typename = std::enable_if_t<std::is_base_of_v<VEObject, typename std::remove_pointer<T>::type>>>
    static T getObjectByAttributeValue(const C &objects, const QString &attrName, const QVariant &value)
    {
        const auto it = std::find_if(std::cbegin(objects), std::cend(objects),
                [&](const T &object) { return object->entityAttributeValue(attrName) == value; });
        if (it != std::cend(objects))
            return *it;

        return {};
    }

    template<typename C, typename T = std::decay_t<decltype(*std::begin(std::declval<C>()))>,
            typename = std::enable_if_t<std::is_base_of_v<VEObject, typename std::remove_pointer<T>::type>>>
    static T getObject(const C &objects, const shared::Id &id)
    {
        const auto it = std::find_if(
                std::cbegin(objects), std::cend(objects), [&id](const T &object) { return object->id() == id; });
        if (it != std::cend(objects))
            return *it;

        return {};
    }

    template<typename C, typename T = std::decay_t<decltype(*std::begin(std::declval<C>()))>,
            typename = std::enable_if_t<std::is_base_of_v<VEObject, typename std::remove_pointer<T>::type>>>
    void initFromObjects(const C &objects, const QHash<shared::Id, EntityAttributes> &extAttrs = {})
    {
        clear();
        setExtAttributes(extAttrs);
        addObjects(objects);
    }

    template<typename C, typename T = std::decay_t<decltype(*std::begin(std::declval<C>()))>,
            typename = std::enable_if_t<std::is_base_of_v<VEObject, typename std::remove_pointer<T>::type>>>
    auto addObjects(const C &objects)
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
                if (!obj->postInit()) {
                    if (removeObject(obj)) {
                        it = addedObjects.erase(it);
                        for (auto descant : obj->descendants()) {
                            removeObject(descant);
                            int idx = addedObjects.indexOf(descant);
                            if (idx >= 0) {
                                addedObjects[idx] = nullptr;
                            }
                        }
                        delete (obj); // deletes all descants as well
                        continue;
                    }
                } else {
                    ids.append(obj->id());
                }
            }
            ++it;
        }

        if (!ids.isEmpty()) {
            Q_EMIT objectsAdded(ids);
        }
    }

    /*!
      Returns all objects casted to the given type
    */
    template<typename T>
    QVector<T *> allObjectsByType() const
    {
        QVector<T *> result;
        const QHash<shared::Id, shared::VEObject *> &ivObjects = objects();
        for (auto obj : ivObjects) {
            if (auto func = dynamic_cast<T *>(obj)) {
                result.append(func);
            }
        }
        return result;
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

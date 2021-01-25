/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "aadlobject.h"
#include "aadlobjectiface.h"

#include <QAbstractItemModel>
#include <QVector>
#include <memory>

namespace ivm {

class AADLObjectComment;
class AADLObjectConnection;
class AADLObjectFunction;
class AADLObjectFunctionType;
class PropertyTemplateConfig;

struct AADLObjectsModelPrivate;
class AADLObjectsModel : public QObject
{
    Q_OBJECT
public:
    explicit AADLObjectsModel(PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);
    ~AADLObjectsModel() override;

    void setSharedTypesModel(AADLObjectsModel *sharedTypesModel);

    bool addObject(AADLObject *obj);
    bool removeObject(AADLObject *obj);

    void setRootObject(shared::Id id);
    AADLObject *rootObject() const;
    shared::Id rootObjectId() const;

    AADLObject *getObject(const shared::Id &id) const;
    AADLObject *getObjectByName(const QString &name, AADLObject::Type type = AADLObject::Type::Unknown,
            Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    AADLObjectIface *getIfaceByName(const QString &name, AADLObjectIface::IfaceType dir,
            const AADLObjectFunctionType *parent = nullptr,
            Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    QList<AADLObjectIface *> getIfacesByName(
            const QString &name, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    AADLObjectFunction *getFunction(const shared::Id &id) const;
    AADLObjectFunction *getFunction(const QString &name, Qt::CaseSensitivity caseSensitivity) const;
    AADLObjectFunctionType *getFunctionType(const shared::Id &id) const;
    QHash<QString, AADLObjectFunctionType *> getAvailableFunctionTypes(const AADLObjectFunction *fnObj) const;
    AADLObjectIface *getInterface(const shared::Id &id) const;
    AADLObjectIfaceRequired *getRequiredInterface(const shared::Id &id) const;
    AADLObjectIfaceProvided *getProvidedInterface(const shared::Id &id) const;
    AADLObjectConnection *getConnection(const shared::Id &id) const;
    AADLObjectComment *getCommentById(const shared::Id &id) const;
    AADLObjectConnection *getConnection(const QString &interfaceName, const QString &source, const QString &target,
            Qt::CaseSensitivity caseSensitivity) const;

    QSet<QString> nestedFunctionNames(const AADLObjectFunctionType *fnt = nullptr) const;
    QSet<QStringList> nestedFunctionPaths(const AADLObjectFunctionType *fnt = nullptr) const;

    AADLObjectConnection *getConnectionForIface(const shared::Id &id) const;
    QVector<AADLObjectConnection *> getConnectionsForIface(const shared::Id &id) const;

    const QHash<shared::Id, AADLObject *> &objects() const;

    QList<AADLObject *> visibleObjects() const;
    QList<AADLObject *> visibleObjects(shared::Id rootId) const;

    void clear();

    /*!
      Returns all objects of the given type
    */
    template<typename T>
    QVector<T *> allObjectsByType() const
    {
        QVector<T *> result;
        const QHash<shared::Id, ivm::AADLObject *> &aadlObjects = objects();
        for (auto obj : aadlObjects) {
            if (auto func = dynamic_cast<T *>(obj)) {
                result.append(func);
            }
        }
        return result;
    }

private:
    bool addObjectImpl(AADLObject *obj);

Q_SIGNALS:
    void aadlObjectsAdded(const QVector<ivm::AADLObject *> &object);
    void aadlObjectRemoved(ivm::AADLObject *object);
    void rootObjectChanged(shared::Id rootId);
    void modelReset();

public Q_SLOTS:
    bool initFromObjects(const QVector<ivm::AADLObject *> &visibleObjects);
    bool addObjects(const QVector<ivm::AADLObject *> &objects);

private:
    const std::unique_ptr<AADLObjectsModelPrivate> d;
};

}

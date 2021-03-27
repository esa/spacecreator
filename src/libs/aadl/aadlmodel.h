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

#include "aadliface.h"
#include "aadlobject.h"

#include <QAbstractItemModel>
#include <QVector>
#include <memory>

namespace ivm {

class AADLComment;
class AADLConnection;
class AADLFunction;
class AADLFunctionType;
class PropertyTemplateConfig;

struct AADLModelPrivate;
class AADLModel : public QObject
{
    Q_OBJECT
public:
    explicit AADLModel(PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);
    ~AADLModel() override;

    void setSharedTypesModel(AADLModel *sharedTypesModel);

    bool addObject(AADLObject *obj);
    bool removeObject(AADLObject *obj);

    void setRootObject(shared::Id id);
    AADLObject *rootObject() const;
    shared::Id rootObjectId() const;

    AADLObject *getObject(const shared::Id &id) const;
    AADLObject *getObjectByName(const QString &name, AADLObject::Type type = AADLObject::Type::Unknown,
            Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    AADLIface *getIfaceByName(const QString &name, AADLIface::IfaceType dir, const AADLFunctionType *parent = nullptr,
            Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    QList<AADLIface *> getIfacesByName(
            const QString &name, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    AADLFunction *getFunction(const shared::Id &id) const;
    AADLFunction *getFunction(const QString &name, Qt::CaseSensitivity caseSensitivity) const;
    AADLFunctionType *getFunctionType(const shared::Id &id) const;
    QHash<QString, AADLFunctionType *> getAvailableFunctionTypes(const AADLFunction *fnObj) const;
    AADLIface *getInterface(const shared::Id &id) const;
    AADLIfaceRequired *getRequiredInterface(const shared::Id &id) const;
    AADLIfaceProvided *getProvidedInterface(const shared::Id &id) const;
    AADLConnection *getConnection(const shared::Id &id) const;
    AADLComment *getCommentById(const shared::Id &id) const;
    AADLConnection *getConnection(const QString &interfaceName, const QString &source, const QString &target,
            Qt::CaseSensitivity caseSensitivity) const;

    QSet<QString> nestedFunctionNames(const AADLFunctionType *fnt = nullptr) const;
    QSet<QStringList> nestedFunctionPaths(const AADLFunctionType *fnt = nullptr) const;

    AADLConnection *getConnectionForIface(const shared::Id &id) const;
    QVector<AADLConnection *> getConnectionsForIface(const shared::Id &id) const;

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

public Q_SLOTS:
    void initFromObjects(const QVector<ivm::AADLObject *> &visibleObjects);
    void addObjects(const QVector<ivm::AADLObject *> &objects);

Q_SIGNALS:
    void aadlObjectsAdded(const QVector<ivm::AADLObject *> &object);
    void aadlObjectRemoved(ivm::AADLObject *object);
    void rootObjectChanged(shared::Id rootId);
    void modelReset();

private:
    bool addObjectImpl(AADLObject *obj);

private:
    const std::unique_ptr<AADLModelPrivate> d;
};

}

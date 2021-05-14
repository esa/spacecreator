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

#include "ivinterface.h"
#include "ivobject.h"
#include "vemodel.h"

#include <QAbstractItemModel>
#include <QVector>
#include <memory>

namespace ivm {

class IVComment;
class IVConnection;
class IVFunction;
class IVFunctionType;
class PropertyTemplateConfig;

struct IVModelPrivate;
class IVModel : public shared::VEModel
{
    Q_OBJECT
public:
    explicit IVModel(PropertyTemplateConfig *dynPropConfig, QObject *parent = nullptr);
    ~IVModel() override;

    void setSharedTypesModel(IVModel *sharedTypesModel);

    bool addObject(IVObject *obj);
    bool removeObject(IVObject *obj);

    void setRootObject(shared::Id rootId);
    IVObject *rootObject() const;
    shared::Id rootObjectId() const;

    IVObject *getObject(const shared::Id &id) const;
    IVObject *getObjectByName(const QString &name, IVObject::Type type = IVObject::Type::Unknown,
            Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    IVInterface *getIfaceByName(const QString &name, IVInterface::InterfaceType dir,
            const IVFunctionType *parent = nullptr, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    QList<IVInterface *> getIfacesByName(
            const QString &name, Qt::CaseSensitivity caseSensitivity = Qt::CaseInsensitive) const;
    IVFunction *getFunction(const shared::Id &id) const;
    IVFunction *getFunction(const QString &name, Qt::CaseSensitivity caseSensitivity) const;
    IVFunctionType *getFunctionType(const QString &name, Qt::CaseSensitivity caseSensitivity) const;
    IVFunctionType *getFunctionType(const shared::Id &id) const;
    QHash<QString, IVFunctionType *> getAvailableFunctionTypes(const IVFunction *fnObj) const;
    IVInterface *getInterface(const shared::Id &id) const;
    IVInterfaceRequired *getRequiredInterface(const shared::Id &id) const;
    IVInterfaceProvided *getProvidedInterface(const shared::Id &id) const;
    IVConnection *getConnection(const shared::Id &id) const;
    IVComment *getCommentById(const shared::Id &id) const;
    IVConnection *getConnection(const QString &interfaceName, const QString &source, const QString &target,
            Qt::CaseSensitivity caseSensitivity) const;

    QSet<QString> nestedFunctionNames(const IVFunctionType *fnt = nullptr) const;
    QSet<QStringList> nestedFunctionPaths(const IVFunctionType *fnt = nullptr) const;

    IVConnection *getConnectionForIface(const shared::Id &id) const;
    QVector<IVConnection *> getConnectionsForIface(const shared::Id &id) const;

    const QHash<shared::Id, IVObject *> &objects() const;

    QList<IVObject *> visibleObjects() const;
    QList<IVObject *> visibleObjects(shared::Id rootId) const;

    void clear();

    /*!
      Returns all objects of the given type
    */
    template<typename T>
    QVector<T *> allObjectsByType() const
    {
        QVector<T *> result;
        const QHash<shared::Id, ivm::IVObject *> &ivObjects = objects();
        for (auto obj : ivObjects) {
            if (auto func = dynamic_cast<T *>(obj)) {
                result.append(func);
            }
        }
        return result;
    }

public Q_SLOTS:
    void initFromObjects(const QVector<ivm::IVObject *> &objects);
    void addObjects(const QVector<ivm::IVObject *> &objects);

Q_SIGNALS:
    void objectsAdded(const QVector<ivm::IVObject *> &object);
    void objectRemoved(ivm::IVObject *object);
    void rootObjectChanged(shared::Id rootId);
    void modelReset();

private:
    bool addObjectImpl(IVObject *obj);

private:
    const std::unique_ptr<IVModelPrivate> d;
};

}

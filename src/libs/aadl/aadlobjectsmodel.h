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

#include <QObject>
#include <QVector>
#include <memory>

namespace aadl {

class AADLObjectComment;
class AADLObjectConnection;
class AADLObjectFunction;
class AADLObjectFunctionType;

struct AADLObjectsModelPrivate;
class AADLObjectsModel : public QObject
{
    Q_OBJECT
public:
    explicit AADLObjectsModel(QObject *parent = nullptr);
    ~AADLObjectsModel() override;

    bool addObject(AADLObject *obj);
    bool removeObject(AADLObject *obj);

    void setRootObject(utils::Id id);
    AADLObject *rootObject() const;
    utils::Id rootObjectId() const;

    AADLObject *getObject(const utils::Id &id) const;
    AADLObject *getObjectByName(const QString &name, AADLObject::Type type = AADLObject::Type::Unknown) const;
    AADLObjectIface *getIfaceByName(
            const QString &name, AADLObjectIface::IfaceType dir, AADLObjectFunctionType *parent = nullptr) const;
    AADLObjectFunction *getFunction(const utils::Id &id) const;
    AADLObjectFunctionType *getFunctionType(const utils::Id &id) const;
    QHash<QString, AADLObjectFunctionType *> getAvailableFunctionTypes(const AADLObjectFunction *fnObj) const;
    AADLObjectIface *getInterface(const utils::Id &id) const;
    AADLObjectIfaceRequired *getRequiredInterface(const utils::Id &id) const;
    AADLObjectIfaceProvided *getProvidedInterface(const utils::Id &id) const;
    AADLObjectConnection *getConnection(const utils::Id &id) const;
    AADLObjectComment *getCommentById(const utils::Id &id) const;

    AADLObjectConnection *getConnectionForIface(const utils::Id &id) const;
    QVector<AADLObjectConnection *> getConnectionsForIface(const utils::Id &id) const;

    const QHash<utils::Id, AADLObject *> &objects() const;
    QList<AADLObject *> visibleObjects() const;

    void clear();

Q_SIGNALS:
    void aadlObjectAdded(aadl::AADLObject *object);
    void aadlObjectRemoved(aadl::AADLObject *object);
    void modelReset();
    void rootObjectChanged(utils::Id rootId);

public Q_SLOTS:
    bool initFromObjects(const QVector<aadl::AADLObject *> &visibleObjects);

private:
    const std::unique_ptr<AADLObjectsModelPrivate> d;
};

}

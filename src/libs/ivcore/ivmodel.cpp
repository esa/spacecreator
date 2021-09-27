/*
  Copyright (C) 2019-2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivmodel.h"

#include "common.h"
#include "errorhub.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivnamevalidator.h"
#include "ivpropertytemplate.h"
#include "propertytemplateconfig.h"

#include <QtDebug>

namespace ivm {

struct IVModelPrivate {
    shared::PropertyTemplateConfig *m_dynPropConfig { nullptr };
    IVModel *m_sharedTypesModel { nullptr };
    shared::Id m_rootObjectId;
    QList<IVObject *> m_visibleObjects;
    QVector<QString> m_headerTitles;
};

IVModel::IVModel(shared::PropertyTemplateConfig *dynPropConfig, IVModel *sharedModel, QObject *parent)
    : shared::VEModel(parent)
    , d(new IVModelPrivate)
{
    d->m_dynPropConfig = dynPropConfig;
    d->m_sharedTypesModel = sharedModel;
}

IVModel::~IVModel() { }

bool IVModel::addObjectImpl(shared::VEObject *obj)
{
    if (ivm::IVObject *ivObj = obj->as<ivm::IVObject *>()) {
        if (shared::VEModel::addObjectImpl(obj)) {
            d->m_visibleObjects.append(ivObj);

            for (const auto attr : d->m_dynPropConfig->propertyTemplatesForObject(ivObj)) {
                if (attr->validate(ivObj) && !attr->isOptional()) {
                    const QVariant &currentValue = obj->entityAttributeValue(attr->name());
                    if (currentValue.isNull()) {
                        const QVariant &defaultValue = attr->defaultValue();
                        if (!defaultValue.isNull()) {
                            if (attr->info() == ivm::IVPropertyTemplate::Info::Attribute) {
                                obj->setEntityAttribute(attr->name(), defaultValue);
                            } else if (attr->info() == ivm::IVPropertyTemplate::Info::Property) {
                                obj->setEntityProperty(attr->name(), defaultValue);
                            } else {
                                QMetaEnum metaEnum = QMetaEnum::fromType<shared::PropertyTemplate::Info>();
                                shared::ErrorHub::addError(shared::ErrorItem::Warning,
                                        tr("Unknown dynamic property info: %1")
                                                .arg(metaEnum.valueToKey(int(attr->info()))));
                            }
                        }
                    }
                }
            }
            return true;
        }
    }

    return false;
}

bool IVModel::removeObject(shared::VEObject *obj)
{
    if (shared::VEModel::removeObject(obj)) {
        if (auto parentObj = qobject_cast<ivm::IVFunctionType *>(obj->parentObject())) {
            parentObj->removeChild(obj->as<ivm::IVObject *>());
        }
        return true;
    }
    return false;
}

void IVModel::setRootObject(shared::Id rootId)
{
    if (d->m_rootObjectId == rootId) {
        return;
    }

    d->m_rootObjectId = rootId;
    d->m_visibleObjects = visibleObjects(rootId);

    Q_EMIT rootObjectChanged(d->m_rootObjectId);
}

IVObject *IVModel::rootObject() const
{
    return getObject(d->m_rootObjectId);
}

shared::Id IVModel::rootObjectId() const
{
    return d->m_rootObjectId;
}

IVObject *IVModel::getObject(const shared::Id &id) const
{
    return qobject_cast<IVObject *>(shared::VEModel::getObject(id));
}

IVObject *IVModel::getObjectByName(const QString &name, IVObject::Type type, Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty())
        return nullptr;

    for (auto object : objects()) {
        if (auto obj = qobject_cast<ivm::IVObject *>(object)) {
            if ((type == IVObject::Type::Unknown || type == obj->type())
                    && obj->title().compare(name, caseSensitivity) == 0)
                return obj;
        }
    }
    return nullptr;
}

/*!
   Returns the first interface found, that has the given \p name and \p dir
 */
IVInterface *IVModel::getIfaceByName(const QString &name, IVInterface::InterfaceType dir, const IVFunctionType *parent,
        Qt::CaseSensitivity caseSensitivity) const
{
    if (name.isEmpty()) {
        return nullptr;
    }

    for (auto obj : objects()) {
        if (obj->title().compare(name, caseSensitivity) == 0) {
            if (IVInterface *iface = obj->as<IVInterface *>()) {
                if (iface->direction() == dir && (!parent || iface->parentObject() == parent)) {
                    return iface;
                }
            }
        }
    }

    return nullptr;
}

/*!
   Returns all interfaces with the given \p name
 */
QList<IVInterface *> IVModel::getIfacesByName(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    QList<IVInterface *> result;
    if (name.isEmpty()) {
        return result;
    }

    for (auto obj : objects()) {
        if (obj->title().compare(name, caseSensitivity) == 0) {
            if (IVInterface *iface = obj->as<IVInterface *>()) {
                result << iface;
            }
        }
    }

    return result;
}

IVFunction *IVModel::getFunction(const shared::Id &id) const
{
    return qobject_cast<IVFunction *>(getObject(id));
}

/*!
   Returns the function with the given name.
   If no such function exists nullptr is returned.
 */
IVFunction *IVModel::getFunction(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    return qobject_cast<IVFunction *>(getObjectByName(name, IVObject::Type::Function, caseSensitivity));
}

IVFunctionType *IVModel::getFunctionType(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    return qobject_cast<IVFunctionType *>(getObjectByName(name, IVObject::Type::FunctionType, caseSensitivity));
}

IVFunctionType *IVModel::getFunctionType(const shared::Id &id) const
{
    return qobject_cast<IVFunction *>(getObject(id));
}

IVFunctionType *IVModel::getSharedFunctionType(const QString &name, Qt::CaseSensitivity caseSensitivity) const
{
    return d->m_sharedTypesModel ? qobject_cast<IVFunctionType *>(
                   d->m_sharedTypesModel->getObjectByName(name, IVObject::Type::FunctionType, caseSensitivity))
                                 : nullptr;
}

IVFunctionType *IVModel::getSharedFunctionType(const shared::Id &id) const
{
    return d->m_sharedTypesModel ? qobject_cast<IVFunction *>(d->m_sharedTypesModel->getObject(id)) : nullptr;
}

QHash<QString, IVFunctionType *> IVModel::getAvailableFunctionTypes(const IVFunction *fnObj) const
{
    QHash<QString, IVFunctionType *> result;
    if (!fnObj)
        return result;

    auto isValid = [](const IVFunctionType *objFnType, const IVFunction *objFn) {
        IVObject *objFnTypeParent = objFnType->parentObject();

        if (!objFnTypeParent) // it's a global FunctionType
            return true;

        IVObject *objFnParent = objFn->parentObject();
        while (objFnParent) {
            if (objFnParent == objFnTypeParent)
                return true;
            objFnParent = objFnParent->parentObject();
        }
        return false;
    };

    for (auto obj : objects()) {
        if (IVFunctionType *objFnType = qobject_cast<IVFunctionType *>(obj)) {
            if (objFnType->isFunctionType() && isValid(objFnType, fnObj)) {
                result.insert(objFnType->title(), objFnType);
            }
        }
    }

    if (d->m_sharedTypesModel) {
        const auto sharedObjects = d->m_sharedTypesModel->objects();
        for (auto sharedObject : sharedObjects) {
            if (sharedObject->parentObject() == nullptr) {
                if (auto fnType = sharedObject->as<IVFunctionType *>()) {
                    if (fnType->isFunctionType()) {
                        result[fnType->title()] = fnType;
                    }
                }
            }
        }
    }

    return result;
}

IVInterface *IVModel::getInterface(const shared::Id &id) const
{
    return qobject_cast<IVInterface *>(getObject(id));
}

IVInterfaceRequired *IVModel::getRequiredInterface(const shared::Id &id) const
{
    return qobject_cast<IVInterfaceRequired *>(getObject(id));
}

IVInterfaceProvided *IVModel::getProvidedInterface(const shared::Id &id) const
{
    return qobject_cast<IVInterfaceProvided *>(getObject(id));
}

IVConnection *IVModel::getConnection(const shared::Id &id) const
{
    return qobject_cast<IVConnection *>(getObject(id));
}

IVComment *IVModel::getCommentById(const shared::Id &id) const
{
    return qobject_cast<IVComment *>(getObject(id));
}

IVConnection *IVModel::getConnectionForIface(const shared::Id &id) const
{
    for (auto obj : objects()) {
        if (auto connection = qobject_cast<IVConnection *>(obj)) {
            Q_ASSERT(connection->sourceInterface() != nullptr);
            Q_ASSERT(connection->targetInterface() != nullptr);
            if (connection->sourceInterface()->id() == id || connection->targetInterface()->id() == id)
                return connection;
        }
    }
    return nullptr;
}

QVector<IVConnection *> IVModel::getConnectionsForIface(const shared::Id &id) const
{
    QVector<IVConnection *> result;

    for (auto obj : objects()) {
        if (auto connection = qobject_cast<IVConnection *>(obj)) {
            if ((connection->sourceInterface() && connection->sourceInterface()->id() == id)
                    || (connection->targetInterface() && connection->targetInterface()->id() == id)) {
                result.append(connection);
            }
        }
    }
    return result;
}

QVector<IVConnection *> IVModel::getConnectionsForFunction(const shared::Id &id) const
{
    QVector<IVConnection *> result;

    for (auto obj : objects()) {
        if (auto connection = qobject_cast<IVConnection *>(obj)) {
            if ((connection->source() && connection->source()->id() == id)
                    || (connection->target() && connection->target()->id() == id)) {
                result.append(connection);
            }
        }
    }
    return result;
}

QList<IVObject *> IVModel::visibleObjects() const
{
    return d->m_visibleObjects;
}

//! Get the visible objects with this root id
QList<IVObject *> IVModel::visibleObjects(shared::Id rootId) const
{
    QList<IVObject *> visibleObjects;
    IVObject *rootObj = getObject(rootId);
    for (const auto &id : objectsOrder()) {
        if (auto obj = getObject(id)) {
            if (rootId.isNull()) {
                visibleObjects.append(obj);
                continue;
            }

            if (!obj->isConnection() && !obj->isConnectionGroup()
                    && (shared::isAncestorOf(rootObj, obj) || rootObj == nullptr)) {
                visibleObjects.append(obj);
            } else if (auto connection = qobject_cast<ivm::IVConnection *>(obj)) {
                const bool sourceIfaceAncestor =
                        shared::isAncestorOf<ivm::IVObject>(rootObj, connection->sourceInterface());
                const bool targetIfaceAncestor =
                        shared::isAncestorOf<ivm::IVObject>(rootObj, connection->targetInterface());
                if ((sourceIfaceAncestor && targetIfaceAncestor) || rootObj == nullptr) {
                    visibleObjects.append(obj);
                }
            }
        }
    }
    return visibleObjects;
}

void IVModel::clear()
{
    d->m_visibleObjects.clear();

    d->m_rootObjectId = shared::InvalidId;
    shared::VEModel::clear();
}

conversion::ModelType IVModel::modelType() const
{
    return conversion::ModelProperties<IVModel>::type;
}

shared::PropertyTemplateConfig *IVModel::dynPropConfig() const
{
    return d->m_dynPropConfig;
}

QString IVModel::defaultFunctionLanguage() const
{
    IVFunction f;
    shared::PropertyTemplate *t =
            d->m_dynPropConfig->propertyTemplateForObject(&f, meta::Props::token(meta::Props::Token::language));
    if (!t) {
        return {};
    }
    return t->defaultValue().toString();
}

QStringList IVModel::availableFunctionLanguages() const
{
    IVFunction f;
    shared::PropertyTemplate *t =
            d->m_dynPropConfig->propertyTemplateForObject(&f, meta::Props::token(meta::Props::Token::language));
    if (t) {
        return t->value().toStringList();
    }

    return {};
}

/*!
   Returns the connection with the given \p interfaceName connection from function \p source to function \p target
   If no such connection is found, a nullptr is returned.
 */
IVConnection *IVModel::getConnection(const QString &interfaceName, const QString &source, const QString &target,
        Qt::CaseSensitivity caseSensitivity) const
{
    for (shared::VEObject *obj : objects()) {
        if (IVConnection *connection = qobject_cast<IVConnection *>(obj)) {
            if (connection->targetInterfaceName().compare(interfaceName, caseSensitivity) == 0
                    && connection->sourceName().compare(source, caseSensitivity) == 0
                    && connection->targetName().compare(target, caseSensitivity) == 0) {
                return connection;
            }
        }
    }
    return nullptr;
}

static inline QVector<IVFunctionType *> nestedFunctions(const IVFunctionType *fnt)
{
    if (!fnt) {
        return {};
    }

    QVector<IVFunctionType *> children = fnt->functionTypes();
    for (const auto fn : fnt->functions()) {
        children.append(fn);
    }
    for (const auto child : children) {
        children << nestedFunctions(child);
    }
    return children;
}

QSet<QString> IVModel::nestedFunctionNames(const IVFunctionType *fnt) const
{
    QSet<QString> names;
    if (!fnt) {
        for (shared::VEObject *object : objects()) {
            if (auto obj = qobject_cast<ivm::IVObject *>(object)) {
                if (obj->type() == IVObject::Type::Function || obj->type() == IVObject::Type::FunctionType) {
                    names.insert(obj->title());
                }
            }
        }
    } else {
        names.insert(fnt->title());
        for (const auto obj : nestedFunctions(fnt)) {
            names.insert(obj->title());
        }
    }

    return names;
}

QSet<QStringList> IVModel::nestedFunctionPaths(const IVFunctionType *fnt) const
{
    QSet<QStringList> paths;
    if (!fnt) {
        for (shared::VEObject *object : objects()) {
            if (auto obj = qobject_cast<ivm::IVObject *>(object)) {
                if (obj->type() == IVObject::Type::Function || obj->type() == IVObject::Type::FunctionType) {
                    paths.insert(obj->path());
                }
            }
        }
    } else {
        paths.insert(fnt->path());
        for (const auto obj : nestedFunctions(fnt)) {
            paths.insert(obj->path());
        }
    }

    return paths;
}
}

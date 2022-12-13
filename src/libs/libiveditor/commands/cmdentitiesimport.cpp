/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cmdentitiesimport.h"

#include "commandids.h"
#include "errorhub.h"
#include "graphicsviewutils.h"
#include "ivconnection.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivxmlreader.h"
#include "standardpaths.h"

#include <QApplication>
#include <QBuffer>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QPointF>
#include <QTemporaryDir>
#include <QtDebug>

namespace Asn1Acn {
class Asn1SystemChecks;
}

namespace ive {
namespace cmd {

inline bool isRectangularType(ivm::IVObject *obj)
{
    return obj->isComment() || obj->isFunction() || obj->isFunctionType();
}

QSet<QString> fnTypeNames(const QVector<ivm::IVObject *> &objects)
{
    QSet<QString> names;
    for (auto it = objects.cbegin(); it != objects.cend(); ++it) {
        if (!isRectangularType(*it)) {
            continue;
        }
        const QString name = (*it)->title();
        if (!name.isEmpty()) {
            names.insert(name);
        }
    }
    return names;
}

CmdEntitiesImport::CmdEntitiesImport(const QByteArray &data, ivm::IVFunctionType *parent, ivm::IVModel *model,
        Asn1Acn::Asn1SystemChecks *asn1Checks, const QPointF &pos, const QString &destPath)
    : ASN1ComponentsImport(asn1Checks, shared::componentsLibraryPath(), destPath)
    , QUndoCommand()
    , m_model(model)
    , m_parent(parent)
{
    ivm::IVXMLReader parser;
    if (parser.read(data)) {
        QVector<ivm::IVObject *> objects = parser.parsedObjects();
        ivm::IVObject::sortObjectList(objects);
        const QSet<QString> functionNames = m_model->nestedFunctionNames();
        QSet<QString> importingNames = fnTypeNames(objects);
        QRectF importingRect;
        for (ivm::IVObject *obj : qAsConst(objects)) {
            obj->setModel(m_model);
            if (isRectangularType(obj) && functionNames.contains(obj->title())) {
                const QString objName = obj->title();
                while (functionNames.contains(obj->title())) {
                    do {
                        obj->setTitle(ivm::IVNameValidator::nextNameFor(obj));
                    } while (importingNames.contains(obj->title()));
                }
                if (objName != obj->title()) {
                    m_renamedFunctions.insert(objName, obj->title());
                    importingNames.insert(obj->title());
                }
            }

            if (isRectangularType(obj) || obj->parentObject() || m_parent) {
                m_importedEntities.append(obj);
                m_parentChildMappings[obj->id()] = obj->parentObject() ? obj->parentObject() : m_parent;
                if (obj->parentObject()) {
                    continue;
                } else {
                    m_rootEntities.append(obj);
                }

                if (isRectangularType(obj)) {
                    const QRectF objRect = shared::graphicsviewutils::rect(obj->coordinates());
                    importingRect |= objRect;
                }
            }
        }
        QRectF parentRect;
        if (m_parent) {
            const QString coordToken = ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates);
            const QString coordStr = m_parent->entityAttributeValue<QString>(coordToken);
            parentRect = shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(coordStr));
        }
        const QPointF basePoint = importingRect.topLeft();
        importingRect.moveTopLeft(pos);
        QList<QRectF> existingRects = existingModelRects();
        shared::graphicsviewutils::findGeometryForRect(importingRect, parentRect, existingRects);
        const QPointF offset = pos - basePoint;

        for (ivm::IVObject *obj : qAsConst(objects)) {
            switch (obj->type()) {
            case ivm::IVObject::Type::Function:
            case ivm::IVObject::Type::FunctionType:
            case ivm::IVObject::Type::Comment:
                prepareRectangularType(obj, offset, parentRect, existingRects);
                break;
            case ivm::IVObject::Type::RequiredInterface:
            case ivm::IVObject::Type::ProvidedInterface:
            case ivm::IVObject::Type::InterfaceGroup:
                prepareEndPointType(obj, offset);
                break;
            case ivm::IVObject::Type::Connection:
            case ivm::IVObject::Type::ConnectionGroup:
                prepareConnectionType(obj, objects);
                break;
            default:
                break;
            }
        }
    } else {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), "");
        setObsolete(true);
    }
}

CmdEntitiesImport::~CmdEntitiesImport()
{
    for (ivm::IVObject *obj : qAsConst(m_rootEntities)) {
        if (obj && !obj->parent()) {
            delete obj;
        }
    }
}

void CmdEntitiesImport::redo()
{
    if (m_importedEntities.isEmpty()) {
        return;
    }

    m_importedAsnFiles.clear();

    QVector<ivm::IVObject *> entities;
    for (ivm::IVObject *entity : qAsConst(m_rootEntities)) {
        if (m_parent) {
            m_parent->addChild(entity);
        }
        redoAsnFileImport(entity);
    }

    for (ivm::IVObject *entity : qAsConst(m_importedEntities)) {
        Q_ASSERT(entity);
        if (!entity) {
            return;
        }
        ivm::IVObject *parentObj = m_parentChildMappings.value(entity->id());
        if (auto parentFunc = qobject_cast<ivm::IVFunctionType *>(parentObj)) {
            entity->setParentObject(parentFunc);
            parentFunc->addChild(entity);
        }
        entities.append(entity);
        redoSourceCloning(entity);
    }
    if (!m_tempDir.isNull()) {
        m_tempDir.reset();
    }
    ivm::IVObject::sortObjectList(entities);
    m_model->addObjects(entities);
}

void CmdEntitiesImport::undo()
{
    m_tempDir.reset(new QTemporaryDir(shared::StandardPaths::writableLocation(QStandardPaths::CacheLocation)
            + QDir::separator() + QLatin1String("import")));

    for (auto it = m_importedEntities.crbegin(); it != m_importedEntities.crend(); ++it) {
        m_model->removeObject(*it);
        undoSourceCloning(*it);
    }
    for (auto it = m_rootEntities.crbegin(); it != m_rootEntities.crend(); ++it) {
        if (m_parent) {
            m_parent->removeChild(*it);
        } else {
            (*it)->setParentObject(nullptr);
        }
    }
    undoAsnFileImport();
}

bool CmdEntitiesImport::mergeWith(const QUndoCommand *command)
{
    Q_UNUSED(command)
    return false;
}

int CmdEntitiesImport::id() const
{
    return ImportEntities;
}

void CmdEntitiesImport::prepareRectangularType(
        ivm::IVObject *obj, const QPointF &offset, QRectF &parentRect, QList<QRectF> &existingRects)
{
    if (!obj->parentObject()) {
        QVector<QPointF> coordinates = shared::graphicsviewutils::polygon(obj->coordinates());
        if (!coordinates.isEmpty()) {
            if (!offset.isNull()) {
                std::for_each(coordinates.begin(), coordinates.end(), [offset](QPointF &point) { point += offset; });
                obj->setCoordinates(shared::graphicsviewutils::coordinates(coordinates));
            }
        } else {
            QRectF itemRect { parentRect.topLeft(), shared::graphicsviewutils::kDefaultGraphicsItemSize };
            shared::graphicsviewutils::findGeometryForRect(itemRect, parentRect, existingRects);
            existingRects.append(itemRect);
            obj->setCoordinates(shared::graphicsviewutils::coordinates(itemRect));
        }
    }
}

void CmdEntitiesImport::prepareEndPointType(ivm::IVObject *obj, const QPointF &offset)
{
    if (auto fn = qobject_cast<ivm::IVFunctionType *>(obj->parentObject())) {
        QVector<QPointF> coordinates = shared::graphicsviewutils::polygon(obj->coordinates());
        if (!fn->parentObject()) {
            if (!coordinates.isEmpty()) {
                if (!offset.isNull()) {
                    std::for_each(
                            coordinates.begin(), coordinates.end(), [offset](QPointF &point) { point += offset; });
                    obj->setCoordinates(shared::graphicsviewutils::coordinates(coordinates));
                }
            } else {
                static const QString coordToken = ivm::meta::Props::token(ivm::meta::Props::Token::coordinates);
                QList<QRectF> interfacesRects;
                for (auto siblingInterface : fn->allInterfaces()) {
                    if (siblingInterface->id() == obj->id()) {
                        continue;
                    }

                    const QString coordStr = siblingInterface->entityAttributeValue<QString>(coordToken);
                    if (coordStr.isEmpty()) {
                        continue;
                    }

                    const QPointF siblingPos =
                            shared::graphicsviewutils::pos(ivm::IVObject::coordinatesFromString(coordStr));
                    const QRectF siblingRect = shared::graphicsviewutils::adjustFromPoint(
                            siblingPos, shared::graphicsviewutils::kInterfaceBaseLength);
                    interfacesRects << siblingRect;
                }

                const QString coordStr = fn->entityAttributeValue<QString>(coordToken);
                QRectF fnRect = shared::graphicsviewutils::rect(ivm::IVObject::coordinatesFromString(coordStr));
                QPointF ifacePos { 0, 0 };
                shared::graphicsviewutils::findGeometryForPoint(ifacePos, fnRect, interfacesRects);
                obj->setCoordinates(shared::graphicsviewutils::coordinates(ifacePos));
            }
        }
    }
}

void CmdEntitiesImport::prepareConnectionType(ivm::IVObject *obj, const QVector<ivm::IVObject *> &objects)
{
    auto findIface = [](const ivm::IVConnection::EndPointInfo &endPointInfo, const QVector<ivm::IVObject *> &objects) {
        auto it = std::find_if(objects.cbegin(), objects.cend(), [&endPointInfo](ivm::IVObject *obj) {
            if (auto iface = obj->as<ivm::IVInterface *>()) {
                return iface->title() == endPointInfo.m_interfaceName && iface->function()
                        && iface->function()->title() == endPointInfo.m_functionName
                        && iface->direction() == endPointInfo.m_ifaceDirection;
            }
            return false;
        });
        return it != objects.cend() ? (*it)->as<ivm::IVInterface *>() : nullptr;
    };

    auto createEnvIface = [&](const ivm::IVConnection::EndPointInfo &pointInfo,
                                  const QVector<ivm::IVObject *> &objects) -> ivm::IVConnection::EndPointInfo {
        /// TODO: restore needed fields
        ivm::IVInterface *envIface = findIface(pointInfo, objects);
        const ivm::IVInterface::CreationInfo info = ivm::IVInterface::CreationInfo::cloneIface(envIface, m_parent);
        ivm::IVInterface *iface = ivm::IVInterface::createIface(info);

        m_importedEntities.append(iface);
        m_rootEntities.append(iface);
        m_parentChildMappings[iface->id()] = info.function;

        /// TODO: setup coordinates after function extracting
        return { iface->function()->title(), iface->title(), pointInfo.m_ifaceDirection };
    };

    auto checkEndPoints = [&](ivm::IVConnection::EndPointInfo &startInfo, ivm::IVConnection::EndPointInfo &endInfo,
                                  const QVector<ivm::IVObject *> &objects) -> bool {
        if (!startInfo.isReady() && !endInfo.isReady())
            return false;

        bool functionNameChanged = false;
        auto it = m_renamedFunctions.find(startInfo.m_functionName);
        if (it != m_renamedFunctions.end()) {
            startInfo.m_functionName = it.value();
            functionNameChanged = true;
        }
        it = m_renamedFunctions.find(endInfo.m_functionName);
        if (it != m_renamedFunctions.end()) {
            endInfo.m_functionName = it.value();
            functionNameChanged = true;
        }

        if (findIface(startInfo, objects)) {
            if (findIface(endInfo, objects)) {
                return functionNameChanged;
            } else {
                endInfo = createEnvIface(startInfo, objects);
            }
        } else if (findIface(endInfo, objects)) {
            startInfo = createEnvIface(endInfo, objects);
        } else {
            return false;
        }

        return true;
    };

    if (auto connection = qobject_cast<ivm::IVConnection *>(obj)) {
        ivm::IVConnection::EndPointInfo startInfo = connection->delayedStart();
        ivm::IVConnection::EndPointInfo endInfo = connection->delayedEnd();
        if (checkEndPoints(startInfo, endInfo, objects)) {
            connection->setDelayedStart(startInfo);
            connection->setDelayedEnd(endInfo);
        }
    }

    if (!obj->parentObject() && m_parent) {
        /// Remove coordinates for connections, they should be regenerated
        /// cause position of interfaces are shifted to new parent sides
        obj->setCoordinates({});
    }
}

QList<QRectF> CmdEntitiesImport::existingModelRects() const
{
    QList<QRectF> existingRects;
    for (const auto &obj : m_model->allObjectsByType<shared::VEObject>()) {
        if (obj->parentObject() == m_parent) {
            const QRectF objRect = shared::graphicsviewutils::rect(obj->coordinates());
            if (objRect.isValid()) {
                existingRects.append(objRect);
            }
        }
    }
    return existingRects;
}

} // namespace ive
} // namespace cmd

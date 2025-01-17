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

#include "asn1systemchecks.h"
#include "commandids.h"
#include "errorhub.h"
#include "geometry.h"
#include "graphicsviewutils.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivfunctiontype.h"
#include "ivinterfacegroup.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "standardpaths.h"
#include "topohelper/geometry.h"

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
    if (!obj)
        return false;

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

CmdEntitiesImport::CmdEntitiesImport(const QList<ivm::IVObject *> &objects, ivm::IVFunctionType *parent,
        ivm::IVModel *model, IVComponentModel *componentModel, Asn1Acn::Asn1SystemChecks *asn1Checks,
        const QPointF &pos)
    : ComponentImportHelper(componentModel, asn1Checks)
    , QUndoCommand()
    , m_model(model)
    , m_parent(parent)
{
    QString errorString;
    if (!init(objects, pos, &errorString)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, errorString, "");
        setObsolete(true);
    }
}

bool CmdEntitiesImport::init(const QVector<ivm::IVObject *> &objects, const QPointF &pos, QString *errorString)
{
    if (objects.isEmpty()) {
        if (errorString) {
            *errorString = tr("Nothing to import");
        }
        return false;
    }

    const QSet<QString> functionNames = m_model->nestedFunctionNames();
    QSet<QString> importingNames = fnTypeNames(objects);
    QRectF importingRect;
    for (ivm::IVObject *obj : qAsConst(objects)) {
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
            if (!obj->parentObject() || obj->parentObject() == m_parent) {
                m_rootEntities.append(obj);
                if (isRectangularType(obj)) {
                    const QRectF objRect = topohelp::geom::rect(obj->coordinates());
                    if (!objRect.isNull())
                        importingRect |= objRect;
                }
            }
        }
    }
    QRectF parentRect;
    if (m_parent) {
        const QString coordToken = ivm::meta::Props::token(ivm::meta::Props::Token::RootCoordinates);
        const QString coordStr = m_parent->entityAttributeValue<QString>(coordToken);
        parentRect = topohelp::geom::rect(ivm::IVObject::coordinatesFromString(coordStr));
    }
    const QPointF insertPos = shared::isValidPosition(pos) ? pos : QPointF(0, 0);
    const QPointF basePoint = importingRect.topLeft();
    importingRect.moveTopLeft(insertPos);
    QList<QRectF> existingRects = existingModelRects(m_parent);
    topohelp::geom::findGeometryForRect(importingRect, parentRect, existingRects);
    const QPointF offset = importingRect.topLeft() - basePoint;

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
            prepareConnectionType(obj, offset, objects);
            break;
        default:
            break;
        }
    }
    return true;
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
    m_importedSources.clear();
    if (!m_tempDir.isNull()) {
        m_tempDir.reset();
    }

    QVector<ivm::IVObject *> entities;
    for (ivm::IVObject *entity : qAsConst(m_rootEntities)) {
        if (m_parent) {
            m_parent->addChild(entity);
        }
        redoAsnFilesImport(entity);
    }

    QList<ivm::IVObject *> entitiesForSourceImport;
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
        if (!entity->isReference())
            entitiesForSourceImport << entity;
    }
    redoSourcesCloning(entitiesForSourceImport);

    ivm::IVObject::sortObjectList(entities);
    m_model->addObjects(entities);
}

void CmdEntitiesImport::undo()
{
    m_tempDir.reset(new QTemporaryDir(shared::StandardPaths::writableLocation(QStandardPaths::CacheLocation)
            + QDir::separator() + QLatin1String("import")));

    for (auto it = m_importedEntities.crbegin(); it != m_importedEntities.crend(); ++it) {
        m_model->removeObject(*it);
    }
    for (auto it = m_rootEntities.crbegin(); it != m_rootEntities.crend(); ++it) {
        if (m_parent) {
            m_parent->removeChild(*it);
        } else {
            (*it)->setParentObject(nullptr);
        }
    }

    undoSourcesCloning();
    undoAsnFilesImport();
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
        QVector<QPointF> coordinates = topohelp::geom::polygon(obj->coordinates());
        if (!coordinates.isEmpty()) {
            if (!offset.isNull()) {
                std::for_each(coordinates.begin(), coordinates.end(), [offset](QPointF &point) { point += offset; });
                obj->setCoordinates(topohelp::geom::coordinates(coordinates));
            }
        } else {
            QRectF itemRect { parentRect.topLeft(), topohelp::kDefaultGraphicsItemSize };
            topohelp::geom::findGeometryForRect(itemRect, parentRect, existingRects);
            existingRects.append(itemRect);
            obj->setCoordinates(topohelp::geom::coordinates(itemRect));
        }
    }
}

void CmdEntitiesImport::prepareEndPointType(ivm::IVObject *obj, const QPointF &offset)
{
    if (auto fn = qobject_cast<ivm::IVFunctionType *>(obj->parentObject())) {
        QVector<QPointF> coordinates = topohelp::geom::polygon(obj->coordinates());
        if (!fn->parentObject()) {
            if (!coordinates.isEmpty()) {
                if (!offset.isNull()) {
                    std::for_each(
                            coordinates.begin(), coordinates.end(), [offset](QPointF &point) { point += offset; });
                    obj->setCoordinates(topohelp::geom::coordinates(coordinates));
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

                    const QPointF siblingPos = topohelp::geom::pos(ivm::IVObject::coordinatesFromString(coordStr));
                    const QRectF siblingRect =
                            topohelp::geom::adjustFromPoint(siblingPos, topohelp::kInterfaceBaseLength);
                    interfacesRects << siblingRect;
                }

                const QString coordStr = fn->entityAttributeValue<QString>(coordToken);
                QRectF fnRect = topohelp::geom::rect(ivm::IVObject::coordinatesFromString(coordStr));
                QPointF ifacePos { 0, 0 };
                shared::graphicsviewutils::findGeometryForPoint(ifacePos, fnRect, interfacesRects);
                obj->setCoordinates(topohelp::geom::coordinates(ifacePos));
            }
        }
    }
}

void CmdEntitiesImport::prepareConnectionType(
        ivm::IVObject *obj, const QPointF &offset, const QVector<ivm::IVObject *> &objects)
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

    if (!obj->parentObject()) {
        QVector<QPointF> coordinates = topohelp::geom::polygon(obj->coordinates());
        if (!coordinates.isEmpty()) {
            if (!offset.isNull()) {
                std::for_each(coordinates.begin(), coordinates.end(), [offset](QPointF &point) { point += offset; });
                obj->setCoordinates(topohelp::geom::coordinates(coordinates));
            }
            if (auto connectionGroup = qobject_cast<ivm::IVConnectionGroup *>(obj)) {
                if (auto iface = connectionGroup->sourceInterfaceGroup()) {
                    iface->setCoordinates(topohelp::geom::coordinates(coordinates.front()));
                } else if (auto iface = findIface(connectionGroup->delayedStart(), objects)) {
                    iface->setCoordinates(topohelp::geom::coordinates(coordinates.front()));
                }
                if (auto iface = connectionGroup->targetInterfaceGroup()) {
                    iface->setCoordinates(topohelp::geom::coordinates(coordinates.last()));
                } else if (auto iface = findIface(connectionGroup->delayedEnd(), objects)) {
                    iface->setCoordinates(topohelp::geom::coordinates(coordinates.last()));
                }
            }
        }
    }
}

QList<QRectF> CmdEntitiesImport::existingModelRects(ivm::IVObject *parent) const
{
    QList<QRectF> existingRects;
    for (const auto &obj : m_model->objects()) {
        if (obj->parentObject() == parent && isRectangularType(obj->as<ivm::IVObject *>())) {
            const QRectF objRect = topohelp::geom::rect(obj->coordinates());
            if (objRect.isValid()) {
                existingRects.append(objRect);
            }
        }
    }
    return existingRects;
}

} // namespace ive
} // namespace cmd

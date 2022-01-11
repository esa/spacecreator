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

#include "asn1reader.h"
#include "asn1systemchecks.h"
#include "commandids.h"
#include "errorhub.h"
#include "file.h"
#include "graphicsviewutils.h"
#include "itemeditor/common/ivutils.h"
#include "itemeditor/graphicsitemhelpers.h"
#include "ivconnection.h"
#include "ivexporter.h"
#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivxmlreader.h"

#include <QApplication>
#include <QBuffer>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QPointF>
#include <QStandardPaths>
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
    : UndoCommand()
    , m_asn1Checks(asn1Checks)
    , m_model(model)
    , m_parent(parent)
    , m_destPath(destPath)
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

    Q_EMIT asn1FilesImported(m_importedAsnFiles);
}

void CmdEntitiesImport::undo()
{
    m_tempDir.reset(new QTemporaryDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation)
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

void CmdEntitiesImport::redoSourceCloning(const ivm::IVObject *object)
{
    if (!object
            || !(object->type() == ivm::IVObject::Type::Function
                       || object->type() == ivm::IVObject::Type::FunctionType)) {
        return;
    }

    const QStringList objPath = ivm::IVObject::path(object);
    const QString rootName = objPath.isEmpty() ? object->title() : objPath.front();
    const QString subPath = relativePathForObject(object);
    const QString sourcePath =
            m_tempDir.isNull() ? shared::componentsLibraryPath() + QDir::separator() + rootName : m_tempDir->path();
    const QDir sourceDir { sourcePath };
    const QDir targetDir { m_destPath };
    shared::copyDir(sourceDir.filePath(subPath), targetDir.filePath(subPath));
}

void CmdEntitiesImport::undoSourceCloning(const ivm::IVObject *object)
{
    if (!object
            || !(object->type() == ivm::IVObject::Type::Function
                       || object->type() == ivm::IVObject::Type::FunctionType)) {
        return;
    }

    const QString subPath = relativePathForObject(object);
    const QString destPath = m_tempDir->path() + QDir::separator() + subPath;
    const QString sourcePath { m_destPath + QDir::separator() + subPath };
    shared::copyDir(sourcePath, destPath);
    QDir sourceDir(sourcePath);
    sourceDir.removeRecursively();
}

static inline bool isSame(const QString &filePath1, const QString &filePath2)
{
    QFile file1(filePath1);
    if (!file1.open(QIODevice::ReadOnly)) {
        qWarning() << file1.fileName() << file1.errorString();
        return false;
    }
    QFile file2(filePath2);
    if (!file2.open(QIODevice::ReadOnly)) {
        qWarning() << file2.fileName() << file2.errorString();
        return false;
    }
    while (!file1.atEnd() && !file2.atEnd()) {
        if (file1.readLine() != file2.readLine())
            return false;
    }
    return true;
}

static inline QStringList asn1ModuleDuplication(
        Asn1Acn::Asn1SystemChecks *asn1Checks, const QVector<QFileInfo> &asn1FileInfos)
{
    if (asn1FileInfos.empty()) {
        return {};
    }
    Asn1Acn::Asn1Reader parser;
    QStringList errorMessages;
    QStringList modulesNames;
    const std::map<QString, std::unique_ptr<Asn1Acn::File>> asn1Data =
            parser.parseAsn1Files(asn1FileInfos, &errorMessages);
    if (asn1Data.empty()) {
        QStringList filenames;
        for (const QFileInfo &fi : qAsConst(asn1FileInfos)) {
            filenames.append(fi.fileName());
        }
        shared::ErrorHub::addError(shared::ErrorItem::Error,
                QObject::tr("%1 was/were not imported: ").arg(filenames.join(", ")), errorMessages.join(", "));
        return modulesNames;
    }

    QStringList projectDefinitionsNames;
    QStringList projectTypeAssignmentsNames;
    const std::vector<Asn1Acn::Definitions *> &projectDefinitions = asn1Checks->definitionsList();
    for_each(projectDefinitions.cbegin(), projectDefinitions.cend(), [&](Asn1Acn::Definitions *defs) {
        projectDefinitionsNames << defs->name();
        projectTypeAssignmentsNames << defs->typeAssignmentNames();
    });

    for (auto it = asn1Data.cbegin(); it != asn1Data.cend(); ++it) {
        for (const auto &defs : it->second->definitionsList()) {
            if (projectDefinitionsNames.contains(defs->name())) {
                modulesNames.append(defs->name());
            } else {
                for (const QString &typeAssignment : defs->typeAssignmentNames()) {
                    if (projectTypeAssignmentsNames.contains(typeAssignment)) {
                        modulesNames.append(defs->name());
                    }
                }
            }
        }
    }
    return modulesNames;
}

void CmdEntitiesImport::redoAsnFileImport(const ivm::IVObject *object)
{
    if (!object
            || !(object->type() == ivm::IVObject::Type::Function
                       || object->type() == ivm::IVObject::Type::FunctionType)) {
        return;
    }

    const QStringList objPath = ivm::IVObject::path(object);
    const QString rootName = objPath.isEmpty() ? object->title() : objPath.front();
    const QString sourcePrefix = m_tempDir.isNull() ? shared::componentsLibraryPath() : m_tempDir->path();
    const QDir sourceDir { sourcePrefix + QDir::separator() + rootName };
    const QDir targetDir { m_destPath };

    QList<QFileInfo> asnFiles = sourceDir.entryInfoList(
            { QLatin1String("*.asn1"), QLatin1String("*.asn"), QLatin1String("*.acn") }, QDir::Files);

    QVector<QFileInfo> fileInfos;
    std::copy_if(asnFiles.begin(), asnFiles.end(), std::back_inserter(fileInfos), [targetDir](const QFileInfo &fi) {
        const QString destFilePath { targetDir.filePath(fi.fileName()) };
        return !isSame(destFilePath, fi.absoluteFilePath());
    });

    auto alreadyExistingModules = asn1ModuleDuplication(m_asn1Checks, fileInfos);
    if (!alreadyExistingModules.isEmpty()) {
        auto res = QMessageBox::question(qApp->activeWindow(), tr("Import ASN1 files"),
                tr("%1 module(s) already exist(s) in project or have(s) same type assignment(s), "
                   "do you want add it/them anyway?")
                        .arg(alreadyExistingModules.join(", ")));
        if (res == QMessageBox::No) {
            return; // CHECK: probably skip only files with module duplication
        }
    }

    for (const QFileInfo &file : qAsConst(fileInfos)) {
        QString destFilePath { targetDir.filePath(file.fileName()) };
        if (QFile::exists(destFilePath)) {
            auto res = QMessageBox::question(qApp->activeWindow(), tr("Import ASN1 files"),
                    tr("%1 already exists in project directory, do you want to rename importing one")
                            .arg(file.fileName()));
            if (res == QMessageBox::No) {
                continue;
            }
            bool ok;
            const QString text = QInputDialog::getText(qApp->activeWindow(), tr("Rename importing ASN1 file"),
                    tr("File name:"), QLineEdit::Normal, rootName + file.fileName(), &ok);
            if (!ok || !text.isEmpty())
                destFilePath = targetDir.filePath(text);
        }
        if (shared::copyFile(file.absoluteFilePath(), destFilePath)) {
            m_importedAsnFiles.append(destFilePath);
        } else {
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("%1 wasn't imported").arg(file.fileName()));
        }
    }
}

void CmdEntitiesImport::undoAsnFileImport()
{
    for (const QString &filePath : qAsConst(m_importedAsnFiles)) {
        QFile::remove(filePath);
    }

    Q_EMIT asn1FileRemoved(m_importedAsnFiles);
}

QString CmdEntitiesImport::relativePathForObject(const ivm::IVObject *object) const
{
    return shared::kRootImplementationPath + QDir::separator() + object->title().toLower();
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
        return it != objects.cend();
    };
    auto checkEndPoint = [&](ivm::IVConnection::EndPointInfo &endPointInfo, const QVector<ivm::IVObject *> &objects) {
        if (!endPointInfo.isReady()) {
            return false;
        }

        bool endPointDataChanged = false;
        auto it = m_renamedFunctions.find(endPointInfo.m_functionName);
        if (it != m_renamedFunctions.end()) {
            endPointInfo.m_functionName = it.value();
            endPointDataChanged = true;
        }

        if (findIface(endPointInfo, objects)) {
            return endPointDataChanged;
        }
        /// TODO: restore needed fields
        ivm::IVInterface::CreationInfo info;
        info.name = endPointInfo.m_interfaceName;
        info.type = endPointInfo.m_ifaceDirection;
        info.model = m_model;
        info.function = m_parent;
        ivm::IVInterface *iface = ivm::IVInterface::createIface(info);

        m_importedEntities.append(iface);
        m_parentChildMappings[iface->id()] = info.function;

        /// TODO: setup coordinates after function extracting
        endPointInfo.m_functionName = m_parent->title();
        endPointInfo.m_interfaceName = iface->title();
        return endPointDataChanged;
    };

    if (auto connection = qobject_cast<ivm::IVConnection *>(obj)) {
        ivm::IVConnection::EndPointInfo info = connection->delayedStart();
        if (checkEndPoint(info, objects))
            connection->setDelayedStart(info);
        info = connection->delayedEnd();
        if (checkEndPoint(info, objects))
            connection->setDelayedEnd(info);
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
    for (auto obj : m_model->allObjectsByType<shared::VEObject>()) {
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

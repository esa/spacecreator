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
        const QVector<ivm::IVObject *> objects = parser.parsedObjects();
        static const QPointF outOfScene { std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max() };
        QPointF basePoint { outOfScene };
        const QSet<QString> functionNames = m_model->nestedFunctionNames();
        for (ivm::IVObject *obj : objects) {
            obj->setModel(m_model);
            if (obj->parentObject()) {
                m_parentChildMappings[obj->id()] = obj->parentObject();
                continue;
            } else {
                m_parentChildMappings[obj->id()] = m_parent;
            }
            if (functionNames.contains(obj->title())) {
                obj->removeEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name));
                obj->setTitle(ivm::IVNameValidator::nextNameFor(obj));
            }
            QVector<QPointF> coordinates = shared::graphicsviewutils::polygon(obj->coordinates());
            std::for_each(coordinates.cbegin(), coordinates.cend(), [&basePoint](const QPointF &point) {
                if (point.x() < basePoint.x())
                    basePoint.setX(point.x());
                if (point.y() < basePoint.y())
                    basePoint.setY(point.y());
            });
            m_rootEntities.append(obj);
        }
        const QPointF offset = basePoint == outOfScene ? QPointF() : pos - basePoint;
        for (ivm::IVObject *obj : objects) {
            QVector<QPointF> coordinates = shared::graphicsviewutils::polygon(obj->coordinates());
            if (coordinates.isEmpty() && !obj->parentObject()) {
                obj->setCoordinates(shared::graphicsviewutils::coordinates(
                        QRectF(pos, shared::graphicsviewutils::kDefaultGraphicsItemSize)));
            } else if (!offset.isNull()) {
                std::for_each(coordinates.begin(), coordinates.end(), [offset](QPointF &point) { point += offset; });
                obj->setCoordinates(shared::graphicsviewutils::coordinates(coordinates));
            }
            m_importedEntities.append(obj);
        }
    } else {
        shared::ErrorHub::addError(shared::ErrorItem::Error, parser.errorString(), "");
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

static inline QStringList asn1ModuleDuplication(Asn1Acn::Asn1SystemChecks *asn1Checks, const QFileInfo &asn1FileInfo)
{
    Asn1Acn::Asn1Reader parser;
    QStringList errorMessages;
    QStringList modulesNames;
    const std::unique_ptr<Asn1Acn::File> asn1Data = parser.parseAsn1File(asn1FileInfo, &errorMessages);
    if (!asn1Data) {
        shared::ErrorHub::addError(shared::ErrorItem::Error,
                QObject::tr("%1 wasn't imported: ").arg(asn1FileInfo.fileName()), errorMessages.join(", "));
        return modulesNames;
    }

    for (const auto &defs : asn1Data->definitionsList()) {
        const std::vector<Asn1Acn::Definitions *> projectDefinitions = asn1Checks->definitionsList();
        auto it = std::find_if(projectDefinitions.cbegin(), projectDefinitions.cend(),
                [defName = defs->name()](
                        const Asn1Acn::Definitions *definitions) { return definitions->name() == defName; });
        if (it != projectDefinitions.cend())
            modulesNames.append(defs->name());
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

    const QList<QFileInfo> rootFiles = sourceDir.entryInfoList(
            { QLatin1String("*.asn1"), QLatin1String("*.asn"), QLatin1String("*.acn") }, QDir::Files);

    for (const QFileInfo &file : rootFiles) {
        QString destFilePath { targetDir.filePath(file.fileName()) };
        if (QFile::exists(destFilePath) && isSame(destFilePath, file.absoluteFilePath())) {
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
        auto alreadyExistingModules = asn1ModuleDuplication(m_asn1Checks, file);
        if (!alreadyExistingModules.isEmpty()) {
            auto res = QMessageBox::question(qApp->activeWindow(), tr("Import ASN1 files"),
                    tr("%1 module already exists in project, do you want add it anyway?")
                            .arg(alreadyExistingModules.join(", ")));
            if (res == QMessageBox::No) {
                continue;
            }
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

} // namespace ive
} // namespace cmd

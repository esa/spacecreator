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

#include "aadlfunctiontype.h"
#include "aadlmodel.h"
#include "aadlnamevalidator.h"
#include "aadlxmlreader.h"
#include "baseitems/common/aadlutils.h"
#include "commandids.h"
#include "xmldocexporter.h"

#include <QBuffer>
#include <QDir>
#include <QPointF>
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QtDebug>

namespace ive {
namespace cmd {

CmdEntitiesImport::CmdEntitiesImport(const QByteArray &data, ivm::AADLFunctionType *parent, ivm::AADLModel *model,
        const QPointF &pos, const QString &destPath)
    : QUndoCommand()
    , m_model(model)
    , m_parent(parent)
    , m_destPath(destPath)
{
    ivm::AADLXMLReader parser;
    QObject::connect(&parser, &ivm::AADLXMLReader::objectsParsed, m_model,
            [this, pos, parent](const QVector<ivm::AADLObject *> &objects) {
                static const QPointF outOfScene { std::numeric_limits<qreal>::max(),
                    std::numeric_limits<qreal>::max() };
                QPointF basePoint { outOfScene };
                const QSet<QString> functionNames = m_model->nestedFunctionNames();
                for (auto obj : objects) {
                    obj->setObjectsModel(m_model);
                    if (obj->parentObject()) {
                        continue;
                    }
                    if (functionNames.contains(obj->title())) {
                        obj->removeAttr(ivm::meta::Props::token(ivm::meta::Props::Token::name));
                        obj->setTitle(ivm::AADLNameValidator::nextNameFor(obj));
                    }
                    QVector<QPointF> coordinates = ive::polygon(obj->coordinates());
                    std::for_each(coordinates.cbegin(), coordinates.cend(), [&basePoint](const QPointF &point) {
                        if (point.x() < basePoint.x())
                            basePoint.setX(point.x());
                        if (point.y() < basePoint.y())
                            basePoint.setY(point.y());
                    });
                    m_rootEntities.append(obj);
                }
                const QPointF offset = basePoint == outOfScene ? QPointF() : pos - basePoint;
                for (auto obj : objects) {
                    QVector<QPointF> coordinates = ive::polygon(obj->coordinates());
                    if (coordinates.isEmpty() && !obj->parentObject()) {
                        obj->setCoordinates(ive::coordinates(QRectF(pos, ive::DefaultGraphicsItemSize)));
                    } else if (!offset.isNull()) {
                        std::for_each(
                                coordinates.begin(), coordinates.end(), [offset](QPointF &point) { point += offset; });
                        obj->setCoordinates(ive::coordinates(coordinates));
                    }
                    m_importedEntities.append(obj);
                }
            });
    QObject::connect(&parser, &ivm::AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });
    parser.read(data);
}

CmdEntitiesImport::~CmdEntitiesImport()
{
    const QVector<QPointer<ivm::AADLObject>> &objects = m_rootEntities;
    for (ivm::AADLObject *obj : objects) {
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

    QVector<ivm::AADLObject *> entities;
    if (m_parent) {
        for (ivm::AADLObject *entity : qAsConst(m_rootEntities)) {
            m_parent->addChild(entity);
        }
    }

    for (ivm::AADLObject *entity : qAsConst(m_importedEntities)) {
        Q_ASSERT(entity);
        if (entity) {
            entities.append(entity);
        }
        redoSourceCloning(entity);
    }
    if (!m_tempDir.isNull()) {
        m_tempDir.reset();
    }
    m_model->addObjects(entities);
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

void CmdEntitiesImport::redoSourceCloning(const ivm::AADLObject *object)
{
    if (!object || object->aadlType() != ivm::AADLObject::Type::Function) {
        return;
    }

    const QString subPath = relativePathForObject(object);
    const QString sourcePrefix = m_tempDir.isNull() ? componentsLibraryPath() : m_tempDir->path();
    const QString sourcePath { sourcePrefix + QDir::separator() + subPath };
    const QString destPath { m_destPath + QDir::separator() + subPath };
    shared::copyDir(sourcePath, destPath);
}

void CmdEntitiesImport::undoSourceCloning(const ivm::AADLObject *object)
{
    if (!object || object->aadlType() != ivm::AADLObject::Type::Function) {
        return;
    }
    const QString subPath = relativePathForObject(object);
    const QString sourcePath = m_tempDir->path() + QDir::separator() + subPath;
    const QString destPath { m_destPath + QDir::separator() + subPath };
    shared::copyDir(destPath, sourcePath);
    QDir destDir(destPath);
    destDir.removeRecursively();
}

QString CmdEntitiesImport::relativePathForObject(const ivm::AADLObject *object) const
{
    return QStringLiteral("work/%1").arg(object->title()).toLower();
}

} // namespace ive
} // namespace cmd

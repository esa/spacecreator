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

#include "ivfunctiontype.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivxmlreader.h"
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

CmdEntitiesImport::CmdEntitiesImport(const QByteArray &data, ivm::IVFunctionType *parent, ivm::IVModel *model,
        const QPointF &pos, const QString &destPath)
    : QUndoCommand()
    , m_model(model)
    , m_parent(parent)
    , m_destPath(destPath)
{
    ivm::IVXMLReader parser;
    QObject::connect(&parser, &ivm::IVXMLReader::objectsParsed, m_model,
            [this, pos, parent](const QVector<ivm::IVObject *> &objects) {
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
                        obj->setTitle(ivm::IVNameValidator::nextNameFor(obj));
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
    QObject::connect(&parser, &ivm::IVXMLReader::error, [](const QString &msg) { qWarning() << msg; });
    parser.read(data);
}

CmdEntitiesImport::~CmdEntitiesImport()
{
    const QVector<QPointer<ivm::IVObject>> &objects = m_rootEntities;
    for (ivm::IVObject *obj : objects) {
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

    QVector<ivm::IVObject *> entities;
    if (m_parent) {
        for (ivm::IVObject *entity : qAsConst(m_rootEntities)) {
            m_parent->addChild(entity);
        }
    }

    for (ivm::IVObject *entity : qAsConst(m_importedEntities)) {
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

void CmdEntitiesImport::redoSourceCloning(const ivm::IVObject *object)
{
    if (!object || object->type() != ivm::IVObject::Type::Function) {
        return;
    }

    const QString subPath = relativePathForObject(object);
    const QString sourcePrefix = m_tempDir.isNull() ? componentsLibraryPath() : m_tempDir->path();
    const QString sourcePath { sourcePrefix + QDir::separator() + subPath };
    const QString destPath { m_destPath + QDir::separator() + subPath };
    shared::copyDir(sourcePath, destPath);
}

void CmdEntitiesImport::undoSourceCloning(const ivm::IVObject *object)
{
    if (!object || object->type() != ivm::IVObject::Type::Function) {
        return;
    }
    const QString subPath = relativePathForObject(object);
    const QString sourcePath = m_tempDir->path() + QDir::separator() + subPath;
    const QString destPath { m_destPath + QDir::separator() + subPath };
    shared::copyDir(destPath, sourcePath);
    QDir destDir(destPath);
    destDir.removeRecursively();
}

QString CmdEntitiesImport::relativePathForObject(const ivm::IVObject *object) const
{
    return QStringLiteral("work/%1").arg(object->title()).toLower();
}

} // namespace ive
} // namespace cmd

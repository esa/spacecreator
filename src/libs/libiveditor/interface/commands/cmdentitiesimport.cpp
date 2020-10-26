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

#include "aadlobjectsmodel.h"
#include "aadlxmlreader.h"
#include "baseitems/common/aadlutils.h"
#include "commandids.h"
#include "xmldocexporter.h"

#include <QBuffer>
#include <QPointF>
#include <QtDebug>

namespace aadlinterface {
namespace cmd {

CmdEntitiesImport::CmdEntitiesImport(aadl::AADLObject *entity, aadl::AADLObjectsModel *model, const QPointF &pos)
    : QUndoCommand()
    , m_model(model)

{
    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        qWarning() << "Can't open buffer for exporting:" << buffer.errorString();
        return;
    }

    if (!XmlDocExporter::exportObjects({ entity }, &buffer)) {
        qWarning() << "Error during component export";
        return;
    }
    buffer.close();

    aadl::AADLXMLReader parser;
    QObject::connect(&parser, &aadl::AADLXMLReader::objectsParsed, m_model,
            [this, pos](const QVector<aadl::AADLObject *> &objects) {
                static const QPointF outOfScene { std::numeric_limits<qreal>::max(),
                    std::numeric_limits<qreal>::max() };
                QPointF basePoint { outOfScene };
                for (auto obj : objects) {
                    if (obj->parentObject()) {
                        continue;
                    }
                    QVector<QPointF> coordinates = aadlinterface::polygon(obj->coordinates());
                    std::for_each(coordinates.cbegin(), coordinates.cend(), [&basePoint](const QPointF &point) {
                        if (point.x() < basePoint.x())
                            basePoint.setX(point.x());
                        if (point.y() < basePoint.y())
                            basePoint.setY(point.y());
                    });
                }
                const QPointF offset = basePoint == outOfScene ? QPointF() : pos - basePoint;
                for (auto obj : objects) {
                    QVector<QPointF> coordinates = aadlinterface::polygon(obj->coordinates());
                    if (coordinates.isEmpty() && !obj->parentObject()) {
                        obj->setCoordinates(
                                aadlinterface::coordinates(QRectF(pos, aadlinterface::DefaultGraphicsItemSize)));
                    } else if (!offset.isNull()) {
                        std::for_each(
                                coordinates.begin(), coordinates.end(), [offset](QPointF &point) { point += offset; });
                        obj->setCoordinates(aadlinterface::coordinates(coordinates));
                    }
                    m_importedEntities.append(obj);
                }
            });
    QObject::connect(&parser, &aadl::AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    if (buffer.open(QIODevice::ReadOnly)) {
        parser.read(&buffer);
    }
    buffer.close();
}

CmdEntitiesImport::~CmdEntitiesImport()
{
    const QVector<QPointer<aadl::AADLObject>> &objects = m_importedEntities;
    for (aadl::AADLObject *obj : objects)
        if (obj && !obj->parent())
            delete obj;
}

void CmdEntitiesImport::redo()
{
    if (m_importedEntities.isEmpty()) {
        return;
    }

    QVector<aadl::AADLObject *> entities;
    for (auto entity : m_importedEntities) {
        Q_ASSERT(entity);
        if (entity) {
            entities.append(entity);
        }
    }
    m_model->addObjects(entities);
}

void CmdEntitiesImport::undo()
{
    for (auto it = m_importedEntities.crbegin(); it != m_importedEntities.crend(); ++it) {
        m_model->removeObject(*it);
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

} // namespace aadlinterface
} // namespace cmd

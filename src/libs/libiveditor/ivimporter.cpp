/*
  Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivimporter.h"

#include "commands/cmdentitiesimport.h"
#include "commands/cmdentitiesinstantiate.h"
#include "commandsstack.h"
#include "errorhub.h"
#include "geometry.h"
#include "interfacedocument.h"
#include "itemeditor/graphicsitemhelpers.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "ivcomponentmodel.h"
#include "ivmodel.h"
#include "ivxmlreader.h"
#include "topohelper/geometry.h"

#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QGraphicsView>
#include <QMimeData>

namespace ive {

static QSet<QString> nestedFunctionNames(const QVector<ivm::IVObject *> &objects)
{
    QSet<QString> names;
    std::for_each(objects.constBegin(), objects.constEnd(), [&](ivm::IVObject *obj) {
        if (auto fn = qobject_cast<ivm::IVFunctionType *>(obj)) {
            names.insert(obj->title());
            names.unite(nestedFunctionNames(fn->children()));
        }
    });
    return names;
}

static QSet<QString> nestedFunctionNames(ivm::IVFunctionType *fn)
{
    if (!fn) {
        return {};
    }

    QSet<QString> names { fn->title() };
    for (ivm::IVFunction *childFn : fn->functions()) {
        names << childFn->title();
        names.unite(nestedFunctionNames(childFn));
    }
    for (ivm::IVFunctionType *childFn : fn->functionTypes()) {
        names << childFn->title();
        names.unite(nestedFunctionNames(childFn));
    }
    return names;
}

IVImporter::IVImporter(InterfaceDocument *document, QGraphicsView *graphicsView, QObject *parent)
    : QObject(parent)
    , m_document(document)
    , m_graphicsView(graphicsView)
{
}

/*!
 * Import the components from the clipboard at the current mouse position
 */
void IVImporter::pasteItems()
{
    const QPoint viewportCursorPos = m_graphicsView->viewport()->mapFromGlobal(QCursor::pos());
    QPointF sceneDropPoint;
    if (m_graphicsView->viewport()
                    ->rect()
                    .marginsRemoved(topohelp::kContentMargins.toMargins())
                    .contains(viewportCursorPos)) {
        sceneDropPoint = m_graphicsView->mapToScene(viewportCursorPos);
    }
    pasteItems(sceneDropPoint);
}

/*!
 * Import the components from the clipboard at the given position
 * \param sceneDropPoint Position in the scene to create the new component(s)
 */
void IVImporter::pasteItems(QPointF sceneDropPoint)
{
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();
    if (!mimeData) {
        return;
    }
    if (!mimeData->hasText()) {
        return;
    }

    const QByteArray data = mimeData->text().toUtf8();
    ivm::IVXMLReader parser;
    if (!parser.read(data)) {
        return;
    }
    QVector<ivm::IVObject *> objects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(objects);
    resetIds(objects);
    const QHash<shared::Id, EntityAttributes> extAttrs = parser.externalAttributes();
    for (ivm::IVObject *obj : std::as_const(objects)) {
        const EntityAttributes attrs = extAttrs.value(obj->id());
        for (auto attrIt = attrs.constBegin(); attrIt != attrs.constEnd(); ++attrIt) {
            obj->setEntityAttribute(attrIt.value());
        }
    }

    const QStringList intersectedNamesList = conflictingNames(objects);
    if (!intersectedNamesList.isEmpty()) {
        const QString msg = tr("Entities couldn't be pasted because of Function names conflict(s): %1")
                                    .arg(intersectedNamesList.join(QLatin1String(", ")));
        shared::ErrorHub::addError(shared::ErrorItem::Error, msg);
        return;
    }

    ivm::IVFunctionType *parentObject = functionAtPosition(sceneDropPoint);

    auto cmdImport = new cmd::CmdEntitiesImport(objects, parentObject, m_document->objectsModel(),
            m_document->componentModel(), m_document->asn1Check(), sceneDropPoint);
    m_document->commandsStack()->push(cmdImport);
}

/*!
 * Import the component with the given ID from the components model at the given point.
 * If an invalid position is passed, a "next best" position is chosen.
 * \param id the ID of the component to import
 * \param sceneDropPoint Position in the scene to create the new component
 */
void IVImporter::importEntity(const shared::Id &id, QPointF sceneDropPoint)
{
    Q_ASSERT(m_document);
    ivm::IVObject *obj = m_document->componentModel()->getObject(id);
    if (!obj) {
        return;
    }

    QList<ivm::IVObject *> objects;
    QSharedPointer<ivm::IVComponentLibrary::Component> component = m_document->componentModel()->component(id);
    ivm::IVXMLReader parser;
    if (!parser.readFile(component->componentPath)) {
        return;
    }
    objects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(objects);
    resetIds(objects);
    const QHash<shared::Id, EntityAttributes> extAttrs = parser.externalAttributes();
    for (auto it = extAttrs.constBegin(); it != extAttrs.constEnd(); ++it) {
        auto objIt = std::find_if(objects.constBegin(), objects.constEnd(),
                [id = it.key()](ivm::IVObject *obj) { return obj->id() == id; });
        if (objIt != objects.constEnd()) {
            for (const EntityAttribute &attr : obj->sortedAttributesValues(it.value())) {
                (*objIt)->setEntityAttribute(attr);
            }
        }
    }

    const QStringList intersectedNamesList = conflictingNames(objects);
    if (!intersectedNamesList.isEmpty()) {
        const QString msg = tr("Chosen entity [%1] couldn't be imported because of Function names conflict(s): %2")
                                    .arg(obj->titleUI(), intersectedNamesList.join(QLatin1String(", ")));
        shared::ErrorHub::addError(shared::ErrorItem::Error, msg);
        return;
    }

    ivm::IVFunctionType *parentObject = functionAtPosition(sceneDropPoint);

    auto cmdImport = new cmd::CmdEntitiesImport(objects, parentObject, m_document->objectsModel(),
            m_document->componentModel(), m_document->asn1Check(), sceneDropPoint);
    m_document->commandsStack()->push(cmdImport);
}

/*!
 * Import the component with the given ID from the shared types model at the given point.
 * If an invalid position is passed, a "next best" position is chosen.
 * \param id the ID of the component to import
 * \param sceneDropPoint Position in the scene to create the new component
 */
void IVImporter::instantiateEntity(const shared::Id &id, QPointF sceneDropPoint)
{
    Q_ASSERT(m_document);
    ivm::IVObject *obj = m_document->sharedTypesModel()->getObject(id);
    if (!obj || obj->type() != ivm::IVObject::Type::FunctionType) {
        return;
    }

    ivm::IVFunctionType *parentObject = functionAtPosition(sceneDropPoint);
    /// TODO: check if it's better to load from fs instead
    auto cmdInstantiate = new cmd::CmdEntitiesInstantiate(obj->as<ivm::IVFunctionType *>(), parentObject,
            m_document->objectsModel(), m_document->sharedTypesModel(), m_document->asn1Check(), sceneDropPoint);
    m_document->commandsStack()->push(cmdInstantiate);
}

/*!
 * Import the component with the given ID as reference from the components model at the given point.
 * If an invalid position is passed, a "next best" position is chosen.
 * \param id the ID of the component to import
 * \param sceneDropPoint Position in the scene to create the new component
 */
void IVImporter::linkEntity(const shared::Id &id, QPointF sceneDropPoint)
{
    Q_ASSERT(m_document);
    ivm::IVObject *obj = m_document->componentModel()->getObject(id);
    if (!obj || obj->type() != ivm::IVObject::Type::Function) {
        return;
    }

    QList<ivm::IVObject *> objects;
    QSharedPointer<ivm::IVComponentLibrary::Component> component = m_document->componentModel()->component(id);

    ivm::IVXMLReader parser;
    if (!parser.readFile(component->componentPath)) {
        return;
    }
    objects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(objects);
    resetIds(objects);
    const QHash<shared::Id, EntityAttributes> extAttrs = parser.externalAttributes();
    for (auto it = extAttrs.constBegin(); it != extAttrs.constEnd(); ++it) {
        auto objIt = std::find_if(objects.constBegin(), objects.constEnd(),
                [id = it.key()](ivm::IVObject *obj) { return obj->id() == id; });
        if (objIt != objects.constEnd()) {
            for (const EntityAttribute &attr : obj->sortedAttributesValues(it.value())) {
                (*objIt)->setEntityAttribute(attr);
            }
        }
    }
    std::for_each(objects.begin(), objects.end(), [](shared::VEObject *obj) {
        obj->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::reference), true);
    });

    const QStringList intersectedNamesList = conflictingNames(objects);
    if (!intersectedNamesList.isEmpty()) {
        const QString msg =
                tr("Chosen entity [%1] couldn't be imported as reference because of Function names conflict(s): %2")
                        .arg(obj->titleUI(), intersectedNamesList.join(QLatin1String(", ")));
        shared::ErrorHub::addError(shared::ErrorItem::Error, msg);
        return;
    }

    ivm::IVFunctionType *parentObject = functionAtPosition(sceneDropPoint);

    auto cmdLink = new cmd::CmdEntitiesImport(objects, parentObject, m_document->objectsModel(),
            m_document->componentModel(), m_document->asn1Check(), sceneDropPoint);
    m_document->commandsStack()->push(cmdLink);
}

/*!
 * Returns the function(type) that is below the given scene position \p screneDropPoint
 * Does eventually update the scene \p screneDropPoint
 */
ivm::IVFunctionType *IVImporter::functionAtPosition(QPointF &sceneDropPoint)
{
    ivm::IVFunctionType *parentObject = nullptr;
    if (shared::isValidPosition(sceneDropPoint)) {
        QGraphicsItem *itemAtScenePos = m_document->scene()->itemAt(sceneDropPoint, m_graphicsView->transform());
        while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
            itemAtScenePos = itemAtScenePos->parentItem();
        }
        parentObject = gi::functionObject(itemAtScenePos);
    } else {
        parentObject = dynamic_cast<ivm::IVFunctionType *>(m_document->objectsModel()->rootObject());

        /// TODO: find proper place for imporing function in the scene if sceneDropPoint invalid (for example action
        /// from context menu is invoked)
        sceneDropPoint = QPoint();
    }

    return parentObject;
}

QStringList IVImporter::conflictingNames(const QVector<ivm::IVObject *> &objects) const
{
    QStringList intersectedNamesList;
    const QSet<QString> existingFunctionNames = m_document->objectsModel()->nestedFunctionNames();
    const QSet<QString> intersectedNames = nestedFunctionNames(objects).intersect(existingFunctionNames);
    if (!intersectedNames.isEmpty()) {
        intersectedNamesList.reserve(intersectedNames.size());
        for (const QString &name : intersectedNames) {
            intersectedNamesList.append(name);
        }
    }
    return intersectedNamesList;
}

void IVImporter::resetIds(const QVector<ivm::IVObject *> &objects) const
{
    for (ivm::IVObject *obj : objects) {
        const shared::Id oldId = obj->id();
        obj->recreateId();
        const shared::Id newId = obj->id();
        for (ivm::IVObject *updateObj : objects) {
            updateObj->updateIdDependency(obj, oldId, newId);
        }
    }
}

} // namespace ive

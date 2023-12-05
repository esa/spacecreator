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

#include "common.h"

#include <QObject>
#include <QPointF>
#include <QPointer>

#pragma once

class QGraphicsView;

namespace ivm {
class IVFunctionType;
}
namespace ive {
class InterfaceDocument;

/*!
 * The IVImporter class handles/ starts various kinds of importing components to the model.
 * This class does:
 * - Checks if imports are possible at all (name clashes ...)
 * - Find a proper position/parent item to place the new item(s)
 * - Load the object(s) to be imported
 * - Call the command to integrate the new object(s) to the model
 */
class IVImporter : public QObject
{
    Q_OBJECT
public:
    IVImporter(InterfaceDocument *document, QGraphicsView *graphicsView, QObject *parent = nullptr);

public Q_SLOTS:
    void pasteItems();
    void pasteItems(const QPointF &sceneDropPoint);
    void importEntity(const shared::Id &id, QPointF sceneDropPoint);
    void instantiateEntity(const shared::Id &id, QPointF sceneDropPoint);
    void linkEntity(const shared::Id &id, QPointF sceneDropPoint);

private:
    ivm::IVFunctionType *functionAtPosition(QPointF &sceneDropPoint);

    QPointer<QGraphicsView> m_graphicsView;
    QPointer<InterfaceDocument> m_document;
};

} // namespace ive

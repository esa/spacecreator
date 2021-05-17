/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "common.h"

#include <QObject>

class QGraphicsView;
class QMouseEvent;
class QContextMenuEvent;

namespace ivm {
class IVObject;
}

namespace shared {
namespace ui {
class VEInteractiveObject;
}
}

namespace ive {
class IVItemModel;
class InterfaceDocument;

class CreatorTool : public QObject
{
    Q_OBJECT

public:
    CreatorTool(InterfaceDocument *doc);
    ~CreatorTool() override;

    enum class ToolType
    {
        Pointer = 0,
        Function,
        FunctionType,
        Comment,
        ProvidedInterface,
        RequiredInterface,
        MultiPointConnection,
        DirectConnection,
        GroupConnection,
        ReCreateConnection,
    };

    CreatorTool::ToolType toolType() const;
    void setCurrentToolType(CreatorTool::ToolType type);
    void removeSelectedItems();
    void groupSelectedItems();

Q_SIGNALS:
    void created();
    void functionCreated(const shared::Id &id);
    void propertyEditorRequest(const shared::Id &id);
    void informUser(const QString &title, const QString &message) const;

    void cutActionTriggered();
    void copyActionTriggered();
    void pasteActionTriggered(const QPointF &scenePos);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    bool onMousePress(QMouseEvent *e);
    bool onMouseRelease(QMouseEvent *e);
    bool onMouseMove(QMouseEvent *e);
    bool onContextMenu(QContextMenuEvent *e);

private:
    struct CreatorToolPrivate;
    CreatorToolPrivate *d;
};

}

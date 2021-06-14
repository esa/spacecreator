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
#include "interface/creatortool.h"
#include "ivinterface.h"
#include "ivitemmodel.h"

#include <QObject>
#include <memory>

class QGraphicsScene;
class QUndoCommand;

namespace ive {
class InterfaceDocument;

class IVCreatorTool : public shared::ui::CreatorTool
{
    Q_OBJECT

public:
    IVCreatorTool(InterfaceDocument *doc);
    ~IVCreatorTool() override;

    enum ToolType
    {
        Pointer = Tool::None,
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

    void removeSelectedItems() override;
    void groupSelectedItems();

Q_SIGNALS:
    void functionCreated(const shared::Id &id);

protected:
    bool onMousePress(QMouseEvent *e) override;
    bool onMouseRelease(QMouseEvent *e) override;
    bool onMouseMove(QMouseEvent *e) override;

    void populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos) override;
    void populateContextMenu_user(QMenu *menu, const QPointF &scenePos) override;
    bool handleToolType(int type, const QPointF &pos) override;

    IVItemModel *model() const override;

private:
    void handleComment(QGraphicsScene *scene, const QPointF &pos);
    void handleFunctionType(QGraphicsScene *scene, const QPointF &pos);
    void handleFunction(QGraphicsScene *scene, const QPointF &pos);
    void handleInterface(QGraphicsScene *scene, ivm::IVInterface::InterfaceType type, const QPointF &pos);
    bool handleConnectionCreate(const QPointF &pos);
    void handleDirectConnection(const QPointF &pos);
    void handleConnectionReCreate(const QVector<QPointF> &graphicPoints);
    void handleConnection(const QVector<QPointF> &graphicPoints) const;

    bool warnConnectionPreview(const QPointF &pos) override;

    QUndoCommand *createInterfaceCommand(const ivm::IVInterface::CreationInfo &info) const;

private:
    InterfaceDocument *m_doc { nullptr };
};

} // namespace ive

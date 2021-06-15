/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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
#include "dvitemmodel.h"
#include "interface/creatortool.h"

namespace shared {
namespace cmd {
class CommandsStackBase;
} // namespace cmd
} // namespace shared

namespace dve {

class DVCreatorTool : public shared::ui::CreatorTool
{
    Q_OBJECT

public:
    enum ToolType
    {
        Pointer = Tool::None,
        Partition,
        MultiPointConnection,
        DirectConnection,
        ReCreateConnection,
    };

    DVCreatorTool(QGraphicsView *view, DVItemModel *model, shared::cmd::CommandsStackBase *commandsStack,
            QObject *parent = nullptr);
    ~DVCreatorTool() override;

    void removeSelectedItems() override;

protected:
    DVItemModel *model() const override;

    bool onMousePress(QMouseEvent *e) override;
    bool onMouseRelease(QMouseEvent *e) override;
    bool onMouseMove(QMouseEvent *e) override;

    void populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos) override;
    void populateContextMenu_user(QMenu *menu, const QPointF &scenePos) override;

    bool handleToolType(int type, const QPointF &pos) override;
    bool warnConnectionPreview(const QPointF &globalPos) override;

private:
    void handlePartition(const QPointF &pos);
    bool handleConnectionCreate(const QPointF &pos);
    void handleDirectConnection(const QPointF &pos);
    void handleConnection(const QVector<QPointF> &graphicPoints) const;
    void handleConnectionReCreate(const QVector<QPointF> &graphicPoints);

    bool prepareMultiPointConnectionPreview(QMouseEvent *e);
    bool prepareReCreateConnectionPreview(QMouseEvent *e);
    bool prepareRectPreview(QMouseEvent *e);
    bool prepareDirectConnectionPreview(QMouseEvent *e);

private:
    shared::cmd::CommandsStackBase *m_commandsStack { nullptr };
};

} // namespace dve

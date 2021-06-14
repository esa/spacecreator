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

#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QPointF>
#include <QPointer>
#include <QSet>

class QMenu;
class QGraphicsView;
class QMouseEvent;
class QGraphicsRectItem;
class QGraphicsPathItem;
class QContextMenuEvent;

namespace shared {
namespace ui {
class VEItemModel;
class VEInteractiveObject;

class CreatorTool : public QObject
{
    Q_OBJECT

public:
    enum Tool
    {
        None = 0
    };
    CreatorTool(QGraphicsView *view, VEItemModel *model, QObject *parent = nullptr);
    ~CreatorTool() override;

    int toolType() const;
    void setCurrentToolType(int type);
    virtual void removeSelectedItems() = 0;

Q_SIGNALS:
    void created();
    void propertyEditorRequest(const shared::Id &id) const;
    void informUser(const QString &title, const QString &message) const;
    void cutActionTriggered();
    void copyActionTriggered();
    void pasteActionTriggered(const QPointF &scenePos);

protected:
    void clearPreviewItem();
    virtual VEItemModel *model() const;

    QPointF cursorInScene() const;
    QPointF cursorInScene(const QPoint &screenPos) const;

    bool eventFilter(QObject *watched, QEvent *event) override;

    virtual bool onMousePress(QMouseEvent *e) = 0;
    virtual bool onMouseRelease(QMouseEvent *e) = 0;
    virtual bool onMouseMove(QMouseEvent *e) = 0;
    virtual bool onContextMenu(QContextMenuEvent *e);

    virtual void populateContextMenu_commonCreate(QMenu *menu, const QPointF &scenePos) = 0;
    virtual void populateContextMenu_commonEdit(QMenu *menu, const QPointF &scenePos);
    virtual void populateContextMenu_propertiesDialog(QMenu *menu);
    virtual void populateContextMenu_user(QMenu *menu, const QPointF &scenePos) = 0;

    virtual bool handleToolType(int type, const QPointF &pos) = 0;
    virtual bool warnConnectionPreview(const QPointF &globalPos) = 0;

    bool showContextMenu(const QPoint &globalPos);

protected:
    QSet<shared::ui::VEInteractiveObject *> m_collidedItems;
    QPointer<QGraphicsView> m_view;
    QPointer<shared::ui::VEItemModel> m_model;
    QGraphicsRectItem *m_previewItem = nullptr;
    QGraphicsPathItem *m_previewConnectionItem = nullptr;
    QVector<QPointF> m_connectionPoints;
    QPointF m_clickScenePos;
    QCursor m_cursor;
    int m_toolType = 0;
};

} // namespace ui
} // namespace shared

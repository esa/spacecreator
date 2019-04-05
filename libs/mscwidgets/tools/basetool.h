/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QGraphicsObject>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QPixmap>
#include <QPointer>

class QMouseEvent;

namespace msc {

class ChartViewModel;

class BaseTool : public QObject
{
    Q_OBJECT
public:
    BaseTool(QGraphicsView *view, QObject *parent = nullptr);
    ~BaseTool() override;

    enum class ToolType
    {
        Pointer = 0,
        ActionCreator,
        ConditionCreator,
        InstanceCreator,
        InstanceStopper,
        MessageCreator,
        CommentCreator,
        EntityDeleter,
        TimerCreator,
        HierarchyCreator,
        CoregionCreator
    };
    Q_ENUM(ToolType)

    virtual BaseTool::ToolType toolType() const = 0;

    bool isActive() const;

    QPixmap icon() const;
    QString title() const;
    QString description() const;

public Q_SLOTS:
    void setActive(bool active);
    virtual void setView(QGraphicsView *view);

Q_SIGNALS:
    void activeChanged(bool to);

protected:
    QPointer<QGraphicsView> m_view;
    QPointer<QGraphicsScene> m_scene;
    bool m_active = false;
    QPixmap m_icon;
    QString m_title = tr("Untitled");
    QString m_description = tr("Unimplemented tool");
    QPointer<QGraphicsObject> m_previewItem = nullptr;
    QCursor m_cursor;

    bool eventFilter(QObject *watched, QEvent *event) override;

    virtual bool onMousePress(QMouseEvent *e);
    virtual bool onMouseRelease(QMouseEvent *e);
    virtual bool onMouseMove(QMouseEvent *e);

    virtual void createPreviewItem() {}
    virtual void commitPreviewItem() {}

    virtual void movePreviewItem(const QPointF &cursorInScene);
    virtual void removePreviewItem();

    QPointF cursorInScene() const;
    QPointF cursorInScene(const QPoint &screenPos) const;
};

} // ns msc

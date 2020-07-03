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

#include "basetool.h"

#include "baseitems/common/mscutils.h"

#include <QDebug>
#include <QGraphicsView>
#include <QMouseEvent>

namespace msc {

BaseTool::BaseTool(QGraphicsView *view, QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_scene(m_view ? m_view->scene() : nullptr)
    , m_cursor(Qt::ArrowCursor)
{
}

BaseTool::~BaseTool()
{
    setActive(false); // uninstall view's eventFilter
}

bool BaseTool::isActive() const
{
    return m_active;
}

QPixmap BaseTool::icon() const
{
    return m_icon;
}

QString BaseTool::title() const
{
    return m_title;
}

QString BaseTool::description() const
{
    return m_description;
}

void BaseTool::setView(QGraphicsView *view)
{
    if (m_view == view)
        return;

    const bool wasActive = isActive();
    if (m_view && wasActive)
        setActive(false);

    m_view = view;
    m_scene = m_view ? m_view->scene() : nullptr;

    if (wasActive)
        setActive(true);
}

void BaseTool::setActive(bool active)
{
    if (active == m_active)
        return;

    if (m_active) {
        removePreviewItem();
    }

    m_active = active;
    if (m_view) {
        if (m_active) {
            m_view->viewport()->installEventFilter(this);
            createPreviewItem();
            m_view->viewport()->setCursor(m_cursor);
        } else {
            m_view->viewport()->removeEventFilter(this);
        }
    }

    Q_EMIT activeChanged(m_active);
}

bool BaseTool::processKeyPress(QKeyEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void BaseTool::setAction(QAction *action)
{
    if (m_action)
        m_action->disconnect(this);

    m_action = action;

    connect(this, &msc::BaseTool::activeChanged, action, &QAction::setChecked);
}

QAction *BaseTool::action() const
{
    return m_action;
}

bool BaseTool::onMousePress(QMouseEvent *e)
{
    Q_UNUSED(e);
    return true;
}

bool BaseTool::onMouseRelease(QMouseEvent *e)
{
    if ((e->button() & Qt::LeftButton) && e->modifiers() == Qt::NoModifier) {
        commitPreviewItem();
        createPreviewItem();
    }

    return true;
}

bool BaseTool::onMouseMove(QMouseEvent *e)
{
    if (m_view)
        movePreviewItem(cursorInScene(e->globalPos()));
    return true;
}

bool BaseTool::eventFilter(QObject *watched, QEvent *event)
{
    if (m_view && watched == m_view->viewport())
        switch (event->type()) {
        case QEvent::MouseButtonPress: {
            return onMousePress(static_cast<QMouseEvent *>(event));
        }
        case QEvent::MouseButtonRelease: {
            return onMouseRelease(static_cast<QMouseEvent *>(event));
        }
        case QEvent::MouseMove: {
            return onMouseMove(static_cast<QMouseEvent *>(event));
        }
        default:
            break;
        }

    return false;
}

void BaseTool::movePreviewItem(const QPointF &scenePos)
{
    if (!m_previewItem)
        return;

    m_previewItem->setPos(scenePos - m_previewItem->boundingRect().center());
}

void BaseTool::removePreviewItem() { }

QPointF BaseTool::cursorInScene() const
{
    return cursorInScene(QCursor::pos()); // TODO: add current screen detection
}

QPointF BaseTool::cursorInScene(const QPoint &globalPos) const
{
    QPointF sceneCoordinates;
    if (m_view) {
        const QPoint viewCoordinates = m_view->mapFromGlobal(globalPos);
        sceneCoordinates = m_view->mapToScene(viewCoordinates);
    }
    return sceneCoordinates;
}

} // ns msc

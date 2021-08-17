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

#include "creatortool.h"

#include "ui/veconnectiongraphicsitem.h"
#include "ui/veinteractiveobject.h"
#include "veitemmodel.h"
#include "veobject.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <QPointer>

namespace shared {
namespace ui {

CreatorTool::CreatorTool(QGraphicsView *view, VEItemModel *model, QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_model(model)
{
    Q_ASSERT(view != nullptr);

    if (view && view->viewport()) {
        view->installEventFilter(this);
        view->viewport()->installEventFilter(this);
    }
}

CreatorTool::~CreatorTool() { }

int CreatorTool::toolType() const
{
    return m_toolType;
}

void CreatorTool::setCurrentToolType(int type)
{
    if (m_toolType == type)
        return;

    m_toolType = type;

    clearPreviewItem();

    if (m_view)
        m_view->setFocus();
}

bool CreatorTool::eventFilter(QObject *watched, QEvent *event)
{
    if (m_view && watched == m_view->viewport()) {
        switch (event->type()) {
        case QEvent::MouseButtonPress:
            return onMousePress(static_cast<QMouseEvent *>(event));
        case QEvent::MouseButtonRelease:
            return onMouseRelease(static_cast<QMouseEvent *>(event));
        case QEvent::MouseMove:
            return onMouseMove(static_cast<QMouseEvent *>(event));
        case QEvent::MouseButtonDblClick: {
            QMouseEvent *e = static_cast<QMouseEvent *>(event);
            if (e->button() & Qt::RightButton) // block double click for right click and handle it
                return onMousePress(e);
            return false;
        }
        default:
            break;
        }
    }

    if (m_view == watched) {
        if (event->type() == QEvent::ContextMenu)
            return onContextMenu(static_cast<QContextMenuEvent *>(event));

        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            switch (keyEvent->key()) {
            case Qt::Key_Escape: {
                if (toolType() == Tool::None) {
                    if (auto scene = m_view->scene()) {
                        scene->clearSelection();
                    }
                } else {
                    clearPreviewItem();
                    Q_EMIT created();
                }
            } break;
            }
        }
    }

    return false;
}

bool CreatorTool::onContextMenu(QContextMenuEvent *e)
{
    if (!m_view || e->reason() == QContextMenuEvent::Mouse)
        return false;

    QGraphicsScene *scene = m_view->scene();
    if (!scene)
        return false;

    QPoint viewPos, globalPos;
    if (!scene->selectedItems().isEmpty()) {
        QGraphicsItem *selectedItem = scene->selectedItems().first();
        const QPointF &scenePos = selectedItem->mapToScene(selectedItem->boundingRect().bottomRight());
        viewPos = m_view->mapFromScene(scenePos);
        globalPos = m_view->mapToGlobal(viewPos);
    } else {
        globalPos = QCursor::pos();
        viewPos = m_view->mapFromGlobal(globalPos);
    }

    // onMousePress is needed to set an appropriate m_previewItem
    QMouseEvent mouseEvent(QEvent::MouseButtonPress, viewPos, Qt::RightButton, Qt::RightButton | Qt::MaxMouseButton,
            nullptr); // Qt::MaxMouseButton is a fake button
                      // to distinguish this mouse event
                      // and thus avoid selecting of another object
    onMousePress(&mouseEvent);

    return showContextMenu(globalPos);
}

static inline QRectF adjustToSize(const QRectF &rect, const QSizeF &minSize)
{
    QRectF itemRect = rect;
    if (itemRect.width() < minSize.width())
        itemRect.setWidth(minSize.width());
    if (itemRect.height() < minSize.height())
        itemRect.setHeight(minSize.height());
    return itemRect;
};

bool CreatorTool::showContextMenu(const QPoint &globalPos)
{
    QMenu *menu = new QMenu(m_view);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    const QPointF scenePos = cursorInScene(globalPos);
    populateContextMenu_commonCreate(menu, scenePos);
    populateContextMenu_commonEdit(menu, scenePos);
    populateContextMenu_propertiesDialog(menu);
    populateContextMenu_user(menu, scenePos);

    if (menu->isEmpty()) {
        delete menu;
        menu = nullptr;
        return false;
    }

    connect(menu, &QMenu::aboutToHide, this, [this]() {
        if (m_previewItem) {
            m_previewItem->setVisible(false);
        }
    });
    menu->exec(globalPos);
    clearPreviewItem();
    return true;
}

bool CreatorTool::handleToolType(int type, const QPointF &pos)
{
    Q_UNUSED(type)
    Q_UNUSED(pos)

    clearPreviewItem();
    Q_EMIT created();
    return true;
}

VEItemModel *CreatorTool::model() const
{
    return m_model;
}

void CreatorTool::clearPreviewItem()
{
    for (VEInteractiveObject *iObj : qAsConst(m_collidedItems)) {
        iObj->enableHighlight();
    }
    m_collidedItems.clear();

    m_clickScenePos = QPointF();

    m_connectionPoints.clear();
    if (m_previewConnectionItem) {
        const auto id = m_previewConnectionItem->data(Qt::UserRole).toUuid();
        if (!id.isNull()) {
            if (auto item = m_model->getItem(id)) {
                item->setVisible(true);
            }
        }

        m_previewConnectionItem->scene()->removeItem(m_previewConnectionItem);
        delete m_previewConnectionItem;
        m_previewConnectionItem = nullptr;
    }

    if (m_previewItem) {
        m_previewItem->scene()->removeItem(m_previewItem);
        delete m_previewItem;
        m_previewItem = nullptr;
    }
}

void CreatorTool::populateContextMenu_commonEdit(QMenu *menu, const QPointF &scenePos)
{
    const QList<QGraphicsItem *> selectedItems = m_view->scene()->selectedItems();
    auto action = menu->addAction(
            QIcon(QLatin1String(":/toolbar/icns/copy.svg")), tr("Copy"), this,
            [this]() { Q_EMIT copyActionTriggered(); }, QKeySequence::Copy);
    action->setEnabled(!selectedItems.isEmpty());

    action = menu->addAction(
            QIcon(QLatin1String(":/toolbar/icns/cut.svg")), tr("Cut"), this, [this]() { Q_EMIT cutActionTriggered(); },
            QKeySequence::Cut);
    action->setEnabled(!selectedItems.isEmpty());

    action = menu->addAction(
            QIcon(QLatin1String(":/toolbar/icns/paste.svg")), tr("Paste"), this,
            [this, scenePos]() { Q_EMIT pasteActionTriggered(scenePos); }, QKeySequence::Paste);
    action->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

void CreatorTool::populateContextMenu_propertiesDialog(QMenu *menu)
{
    QGraphicsScene *scene = m_view->scene();
    if (!scene) {
        return;
    }

    QGraphicsItem *gi = scene->selectedItems().isEmpty() ? nullptr : scene->selectedItems().first();
    auto veIObj = gi ? qobject_cast<VEInteractiveObject *>(gi->toGraphicsObject()) : nullptr;
    if (auto connectionItem = qobject_cast<VEConnectionGraphicsItem *>(veIObj)) {
        menu->addSeparator();
        QAction *action = menu->addAction(tr("Re-create path"));
        action->setEnabled(connectionItem);

        connect(action, &QAction::triggered, this, [connectionItem]() {
            connectionItem->layout();
            connectionItem->updateEntity();
        });
    } else if (veIObj) {
        menu->addSeparator();
        QAction *action = menu->addAction(tr("Properties"));
        action->setEnabled(true);

        connect(action, &QAction::triggered,
                [this, veIObj]() { Q_EMIT propertyEditorRequest(veIObj->entity()->id()); });
    }
}

QPointF CreatorTool::cursorInScene() const
{
    return cursorInScene(QCursor::pos()); // TODO: add current screen detection
}

QPointF CreatorTool::cursorInScene(const QPoint &globalPos) const
{
    QPointF sceneCoordinates;
    if (m_view) {
        const QPoint viewCoordinates = m_view->viewport()->mapFromGlobal(globalPos);
        sceneCoordinates = m_view->mapToScene(viewCoordinates);
    }
    return sceneCoordinates;
}

} // namespace ui
} // namespace shared

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

#include "creatortool.h"

#include "aadlcommentgraphicsitem.h"
#include "aadlcontainergraphicsitem.h"
#include "aadlfunctiongraphicsitem.h"
#include "commands/cmdcommentitemcreate.h"
#include "commands/cmdcontaineritemcreate.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMenu>
#include <QMouseEvent>
#include <app/commandsstack.h>
#include <tab_aadl/aadlobjectcontainer.h>
#include <tab_aadl/aadlobjectfunction.h>
#include <tab_aadl/aadlobjectsmodel.h>

namespace taste3 {
namespace aadl {

namespace cmd {

}

CreatorTool::CreatorTool(QGraphicsView *view, AADLObjectsModel *model, QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_model(model)
{
    setObjectName(QLatin1String("CreatorTool"));

    Q_ASSERT(view);
    Q_ASSERT(model);

    if (m_view && m_view->viewport())
        m_view->viewport()->installEventFilter(this);
}

CreatorTool::ToolType CreatorTool::toolType() const
{
    return m_toolType;
}

void CreatorTool::setCurrentToolType(CreatorTool::ToolType type)
{
    m_toolType = type;
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
        default:
            break;
        }
    }

    return false;
}

bool CreatorTool::onMousePress(QMouseEvent *e)
{
    if (!m_view)
        return false;

    auto scene = m_view->scene();
    if (!scene)
        return false;

    const QPointF scenePos = cursorInScene(e->globalPos());
    if (!m_previewItem) {
        m_previewItem =
                scene->addRect(QRectF(scenePos, QSize(0, 0)), QPen(Qt::cyan, 2, Qt::DotLine), QBrush(Qt::lightGray));
    } else {
        m_previewItem->setRect(QRectF(scenePos, QSizeF(0, 0)));
    }
    if (!m_previewItem->scene())
        scene->addItem(m_previewItem);
    m_previewItem->setVisible(true);
    return true;
}

bool CreatorTool::onMouseRelease(QMouseEvent *e)
{
    if (!m_previewItem || !m_view)
        return false;

    const QPointF scenePos = cursorInScene(e->globalPos());

    if (e->button() & Qt::RightButton || m_toolType == ToolType::Pointer) {
        QMenu *menu = new QMenu(m_view);
        menu->setAttribute(Qt::WA_DeleteOnClose);
        menu->addAction(tr("Container"), this, [this]() { createItem(ToolType::Container); });
        menu->addAction(tr("Function"), this, [this]() { createItem(ToolType::Function); });
        menu->addAction(tr("Comment"), this, [this]() { createItem(ToolType::Comment); });
        connect(menu, &QMenu::aboutToHide, this, [this]() { m_previewItem->setVisible(false); });
        menu->exec(m_view->mapToGlobal(m_view->mapFromScene(scenePos)));
    } else {
        createItem(m_toolType);
    }
    return true;
}

bool CreatorTool::onMouseMove(QMouseEvent *e)
{
    if (!m_previewItem || !m_view)
        return false;

    const QPointF scenePos = cursorInScene(e->globalPos());
    QRectF currentRect = m_previewItem->rect();
    currentRect.setBottomRight(scenePos);
    m_previewItem->setRect(currentRect.normalized());
    return true;
}

QPointF CreatorTool::cursorInScene() const
{
    return cursorInScene(QCursor::pos()); // TODO: add current screen detection
}

QPointF CreatorTool::cursorInScene(const QPoint &globalPos) const
{
    QPointF sceneCoordinates;
    if (m_view) {
        const QPoint viewCoordinates = m_view->mapFromGlobal(globalPos);
        sceneCoordinates = m_view->mapToScene(viewCoordinates);
    }
    return sceneCoordinates;
}

void CreatorTool::createItem(CreatorTool::ToolType type)
{
    if (!m_view || !m_previewItem)
        return;

    const QRectF itemSceneRect = m_previewItem->rect();
    const QVariantList params = { itemSceneRect, qVariantFromValue(m_model.data()) };
    if (auto scene = m_view->scene()) {
        switch (type) {
        case ToolType::Comment: {
            taste3::cmd::CommandsStack::current()->push(cmd::CommandsFactory::create(cmd::CreateCommentEntity, params));
        } break;
        case ToolType::Container: {
            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateContainerEntity, params));
        } break;
        case ToolType::Function: {
            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateFunctionEntity, params));
        } break;
        case ToolType::ProvidedInterface: {
            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateProvidedInterfaceEntity, params));
        } break;
        case ToolType::RequiredInterface: {
            taste3::cmd::CommandsStack::current()->push(
                    cmd::CommandsFactory::create(cmd::CreateRequiredInterfaceEntity, params));
        } break;
        }
    }
    m_previewItem->setVisible(false);

    emit created();
}

} // namespace aadl
} // namespace taste3

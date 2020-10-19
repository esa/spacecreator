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

#include "commentcreatortool.h"

#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/mscutils.h"
#include "chartitem.h"
#include "chartlayoutmanager.h"
#include "commands/cmdcommentitemchangegeometry.h"
#include "commands/cmdentitycommentchange.h"
#include "commentitem.h"
#include "msccomment.h"

#include <QMouseEvent>
#include <QUndoStack>

namespace msc {

CommentCreatorTool::CommentCreatorTool(bool isGlobal, ChartLayoutManager *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
    , m_isGlobalComment(isGlobal)
{
    if (m_isGlobalComment) {
        m_title = tr("Comment");
        m_icon = QPixmap(":/icons/toolbar/global_comment.svg");
        m_description = tr("Create new Comment the Chart");
    } else {
        m_title = tr("Comment");
        m_icon = QPixmap(":/icons/toolbar/comment.svg");
        m_description = tr("Create new Comment for item");
    }
}

msc::BaseTool::ToolType msc::CommentCreatorTool::toolType() const
{
    return BaseTool::ToolType::CommentCreator;
}

void CommentCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active)
        return;

    CommentItem *item = new CommentItem(m_model->currentChart(), m_model);
    item->setText(tr("Add new comments here"));
    item->setOpacity(0.5);
    item->setGlobalPreview(m_isGlobalComment);
    m_scene->addItem(item);

    m_previewItem = item;
}

void CommentCreatorTool::commitPreviewItem()
{
    if (!m_previewItem || !m_activeChart)
        return;

    CommentItem *item = qobject_cast<CommentItem *>(m_previewItem);
    if (!item)
        return;

    const QString itemComment = item->text();
    if (m_isGlobalComment) {
        const QRectF contentRect = m_model->itemForChart()->contentRect();
        QRectF itemSceneRect = item->sceneBoundingRect();
        if (itemSceneRect.left() < contentRect.left())
            itemSceneRect.moveLeft(contentRect.left());
        if (itemSceneRect.top() < contentRect.top())
            itemSceneRect.moveTop(contentRect.top());
        if (itemSceneRect.right() > contentRect.right())
            itemSceneRect.setRight(contentRect.right());
        if (itemSceneRect.bottom() > contentRect.bottom())
            itemSceneRect.setBottom(contentRect.bottom());

        QRect newRect;
        if (CoordinatesConverter::sceneToCif(itemSceneRect, newRect)) {
            QUndoStack *undoStack = m_model->undoStack();
            undoStack->beginMacro(tr("Create comment"));
            undoStack->push(new cmd::CmdCommentItemChangeGeometry(
                    m_model->currentChart()->cifRect(), newRect, m_model->currentChart(), m_model));
            undoStack->push(new cmd::CmdEntityCommentChange(m_model->currentChart(), itemComment, m_model));
            undoStack->endMacro();
        }
    } else {
        auto previewEntity = m_model->nearestEntity(m_previewItem->sceneBoundingRect().center());
        m_model->undoStack()->push(new cmd::CmdEntityCommentChange(previewEntity, itemComment, m_model));
    }

    m_model->updateLayout();

    removePreviewItem();

    Q_EMIT created();
}

} // namespace msc

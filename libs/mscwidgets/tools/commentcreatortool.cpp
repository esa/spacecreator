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

#include "baseitems/commentitem.h"
#include "baseitems/common/utils.h"
#include "commands/common/commandsstack.h"

#include <QMouseEvent>

namespace msc {

CommentCreatorTool::CommentCreatorTool(bool isGlobal, ChartViewModel *model, QGraphicsView *view, QObject *parent)
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

    CommentItem *item = new CommentItem;
    item->setGlobal(m_isGlobalComment);
    item->setText(tr("Add new comments here"));
    item->setOpacity(0.5);
    m_scene->addItem(item);

    m_previewItem = item;
}

void CommentCreatorTool::commitPreviewItem()
{
    if (!m_previewItem || !m_activeChart)
        return;

    m_previewEntity = m_isGlobalComment ? m_model->currentChart()
                                        : m_model->nearestEntity(m_previewItem->sceneBoundingRect().center());

    const CommentItem *item = qobject_cast<CommentItem *>(m_previewItem);
    const QString itemComment = item ? item->text() : QString();
    const QVariantList cmdParams = { QVariant::fromValue<msc::MscEntity *>(m_previewEntity), itemComment };
    msc::cmd::CommandsStack::push(msc::cmd::Id::ChangeComment, cmdParams);

    removePreviewItem();
    startWaitForModelLayoutComplete(m_previewEntity);

    Q_EMIT created();
}

void CommentCreatorTool::removePreviewItem()
{
    if (!m_previewItem)
        return;

    utils::removeSceneItem(m_previewItem);
    delete m_previewItem.data();
}

} // namespace msc

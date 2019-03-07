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

#include "hierarchycreatortool.h"

#include "baseitems/common/utils.h"
#include "commands/cmddocumentcreate.h"
#include "commands/common/commandsstack.h"
#include "documentitem.h"
#include "hierarchyviewmodel.h"

namespace msc {

HierarchyCreatorTool::HierarchyCreatorTool(MscDocument::HierarchyType hierarchyType, HierarchyViewModel *model,
                                           QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(nullptr, view, parent)
    , m_hierarchyType(hierarchyType)
    , m_hierarchyViewModel(model)
{
    initTool();
}

BaseTool::ToolType HierarchyCreatorTool::toolType() const
{
    return BaseTool::ToolType::HierarchyCreator;
}

void HierarchyCreatorTool::createPreviewItem()
{
    if (!m_scene || m_previewItem || !m_active || !m_hierarchyViewModel->selectedDocument())
        return;

    MscDocument *document = new MscDocument(
            QObject::tr("Document_%1").arg(m_hierarchyViewModel->selectedDocument()->documents().size()));
    document->setHierarchyType(m_hierarchyType);
    document->addChart(new MscChart(document->name() + QObject::tr("_msc")));
    DocumentItem *item = new DocumentItem(document);

    m_previewItem = item;
    m_previewEntity = item->document();

    m_scene->addItem(m_previewItem);
}

void HierarchyCreatorTool::commitPreviewItem()
{
    if (!m_previewEntity) {
        return;
    }

    auto document = qobject_cast<MscDocument *>(m_previewEntity);
    const QVariantList &cmdParams = { QVariant::fromValue<msc::MscDocument *>(document),
                                      QVariant::fromValue<msc::MscDocument *>(
                                              m_hierarchyViewModel->selectedDocument()) };

    utils::removeSceneItem(m_previewItem);
    delete m_previewItem.data();

    msc::cmd::CommandsStack::push(msc::cmd::Id::CreateDocument, cmdParams);

    Q_EMIT created();
}

void HierarchyCreatorTool::initTool()
{
    switch (m_hierarchyType) {
    case MscDocument::HierarchyAnd:
        m_title = tr("HierarchyAnd");
        m_description = tr("Hierarchy And");
        m_icon = QPixmap(":/icons/document_and.png");
        break;
    case MscDocument::HierarchyException:
        m_title = tr("HierarchyException");
        m_description = tr("Hierarchy Exception");
        m_icon = QPixmap(":/icons/document_exception.png");
        break;
    case MscDocument::HierarchyIs:
        m_title = tr("HierarchyIs");
        m_description = tr("Hierarchy Is");
        m_icon = QPixmap(":/icons/document_is_scenario.png");
        break;
    case MscDocument::HierarchyLeaf:
        m_title = tr("HierarchyLeaf");
        m_description = tr("Hierarchy Leaf");
        m_icon = QPixmap(":/icons/document_leaf.png");
        break;
    case MscDocument::HierarchyOr:
        m_title = tr("HierarchyOr");
        m_description = tr("Hierarchy Or");
        m_icon = QPixmap(":/icons/document_or.png");
        break;
    case MscDocument::HierarchyParallel:
        m_title = tr("HierarchyParallel");
        m_description = tr("Hierarchy Parallel");
        m_icon = QPixmap(":/icons/document_parallel.png");
        break;
    case MscDocument::HierarchyRepeat:
        m_title = tr("HierarchyRepeat");
        m_description = tr("Hierarchy Repeat");
        m_icon = QPixmap(":/icons/document_repeat.png");
        break;
    default:
        break;
    }
}

} // ns msc

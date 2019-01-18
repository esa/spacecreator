/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

namespace msc {

HierarchyCreatorTool::HierarchyCreatorTool(ToolType type, ChartViewModel *model, QGraphicsView *view, QObject *parent)
    : BaseCreatorTool(model, view, parent)
    , m_toolType(type)
{
    initTool();
}

ToolType HierarchyCreatorTool::toolType() const
{
    return m_toolType;
}

void HierarchyCreatorTool::createPreviewItem() {}

void HierarchyCreatorTool::commitPreviewItem() {}

void HierarchyCreatorTool::initTool()
{
    switch (m_toolType) {
    case ToolType::HierarchyAndCreator:
        m_title = tr("HierarchyAnd");
        m_description = tr("Hierarchy And");
        m_icon = QPixmap(":/icons/document_and.png");
        break;
    case ToolType::HierarchyExceptionCreator:
        m_title = tr("HierarchyException");
        m_description = tr("Hierarchy Exception");
        m_icon = QPixmap(":/icons/document_exception.png");
        break;
    case ToolType::HierarchyIsCreator:
        m_title = tr("HierarchyIs");
        m_description = tr("Hierarchy Is");
        m_icon = QPixmap(":/icons/document_is_scenario.png");
        break;
    case ToolType::HierarchyLeafCreator:
        m_title = tr("HierarchyLeaf");
        m_description = tr("Hierarchy Leaf");
        m_icon = QPixmap(":/icons/document_leaf.png");
        break;
    case ToolType::HierarchyOrCreator:
        m_title = tr("HierarchyOr");
        m_description = tr("Hierarchy Or");
        m_icon = QPixmap(":/icons/document_or.png");
        break;
    case ToolType::HierarchyParallelCreator:
        m_title = tr("HierarchyParallel");
        m_description = tr("Hierarchy Parallel");
        m_icon = QPixmap(":/icons/document_parallel.png");
        break;
    case ToolType::HierarchyRepeatCreator:
        m_title = tr("HierarchyRepeat");
        m_description = tr("Hierarchy Repeat");
        m_icon = QPixmap(":/icons/document_repeat.png");
        break;
    default:
        break;
    }
}

} // ns msc

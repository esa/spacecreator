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

#include "pointertool.h"

#include <QDebug>

namespace msc {

PointerTool::PointerTool(QGraphicsView *view, QObject *parent)
    : BaseTool(view, parent)
{
    m_title = tr("Pointer");
    m_description = tr("Select/move/resize items");
    m_icon = QPixmap(":/icn/toolbar/icons/toolbar/pointer.png");
}

ToolType PointerTool::toolType() const
{
    return msc::ToolType::Pointer;
}

bool PointerTool::onMousePress(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PointerTool::onMouseRelease(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool PointerTool::onMouseMove(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

void PointerTool::createPreviewItem() {}

void PointerTool::commitPreviewItem() {}

} // ns msc

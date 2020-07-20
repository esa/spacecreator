/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "graphicsscenebase.h"

namespace shared {
namespace ui {

GraphicsSceneBase::GraphicsSceneBase(QObject *parent)
    : QGraphicsScene(parent)
{
}

bool GraphicsSceneBase::mousePressed() const
{
    return m_mousePressed;
}

void GraphicsSceneBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    setMousePressed(true);
}

void GraphicsSceneBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setMousePressed(false);
    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicsSceneBase::setMousePressed(bool pressed)
{
    if (pressed == m_mousePressed) {
        return;
    }

    m_mousePressed = pressed;
    Q_EMIT mousePressedChanged(m_mousePressed);
}

}
}

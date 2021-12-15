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

#pragma once

#include <QGraphicsScene>

namespace shared {
namespace ui {

/*!
   Class to extend the QGraphicsScene with the property \e mousePressed, if the mouse is currently pressed in that scene
 */
class GraphicsSceneBase : public QGraphicsScene
{
    Q_OBJECT

    Q_PROPERTY(bool mousePressed READ mousePressed NOTIFY mousePressedChanged)

public:
    GraphicsSceneBase(QObject *parent = nullptr);

    bool mousePressed() const;

Q_SIGNALS:
    void mousePressedChanged(bool mousePressed);

protected:
    void helpEvent(QGraphicsSceneHelpEvent *helpEvent) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void setMousePressed(bool pressed);

private:
    bool m_mousePressed = false;
};

}
}

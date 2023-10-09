/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

   This application is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   This application is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
*/

#pragma once

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QObject>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <memory>

namespace shared {
namespace ui {

struct MiniMapPrivate;
class MiniMap : public QGraphicsView
{
    Q_OBJECT
public:
    enum class Location
    {
        North = 0,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest,
    };

    Q_ENUM(Location);

    explicit MiniMap(QWidget *parent = nullptr);
    ~MiniMap(); // the definition (even an empty) is necessary for the dptr (std::unique_ptr)

    void setupSourceView(QGraphicsView *view);

    void setDimColor(const QColor &to);
    QColor dimColor() const;

    MiniMap::Location location() const;
    void setLocation(MiniMap::Location location);

Q_SIGNALS:
    void visibilityChanged(bool visible);

protected:
    void resizeEvent(QResizeEvent *e) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void drawItems(
            QPainter *painter, int numItems, QGraphicsItem **items, const QStyleOptionGraphicsItem *options) override;

    void drawForeground(QPainter *painter, const QRectF &rect) override;
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    void processMouseInput();
    bool checkMouseEvent(QMouseEvent *e, Qt::MouseButton current, Qt::MouseButton started,
            Qt::KeyboardModifier mod = Qt::NoModifier) const;
    void updateCursorInMappedViewport(const QPoint &pos, Qt::CursorShape targetShape);
    QRectF mappedViewportOnScene() const;
    void adjustGeometry();
    void updateScaling();

    const std::unique_ptr<MiniMapPrivate> d;

    MiniMap::Location posToLocation(const QPointF &pos, const QRectF &viewport) const;
    void followMouse(const QPointF &globalPos);

    enum class Stickiness
    {
        Strict,
        Dynamic, // follow an edge by mouse movement
    };
    QRect stickToEdge(MiniMap::Location edge, const QRect &srcGeometry, MiniMap::Stickiness flow) const;
};

} // namespace ui
} // namespace shared

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

#include <QGraphicsView>

namespace shared {
namespace ui {

class GraphicsViewBase : public QGraphicsView
{
    Q_OBJECT

    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(qreal minZoomPercent READ minZoomPercent WRITE setMinZoomPercent NOTIFY minZoomPercentChanged)
    Q_PROPERTY(qreal maxZoomPercent READ maxZoomPercent WRITE setMaxZoomPercent NOTIFY maxZoomPercentChanged)
    Q_PROPERTY(qreal zoomStepPercent READ zoomStepPercent WRITE setZoomStepPercent NOTIFY zoomStepPercentChanged)

public:
    explicit GraphicsViewBase(QGraphicsScene *scene, QWidget *parent = nullptr);
    explicit GraphicsViewBase(QWidget *parent = nullptr);
    ~GraphicsViewBase() override;

    double zoom() const;
    qreal minZoomPercent() const;
    qreal maxZoomPercent() const;
    qreal zoomStepPercent() const;

Q_SIGNALS:
    void mouseMoved(const QString &coordsInfo);
    void zoomChanged(qreal percent);
    void minZoomPercentChanged(qreal percent);
    void maxZoomPercentChanged(qreal percent);
    void zoomStepPercentChanged(qreal percent);

public Q_SLOTS:
    void setZoom(double percent);
    void setMinZoomPercent(qreal percent);
    void setMaxZoomPercent(qreal percent);
    void setZoomStepPercent(qreal percent);

protected:
    // Return a list of coordinate+name that should be shown in the statusbar on mouse moves
    virtual QList<QPair<QPointF, QString>> mouseMoveCoordinates(
            QGraphicsScene *scene, const QPoint &screenPos, const QPointF &scenePos) const = 0;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    struct GraphicsViewBasePrivate;
    GraphicsViewBasePrivate *d;
};

}
}

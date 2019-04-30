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

#pragma once

#include "chartitem.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>
#include <QPointer>

class QGraphicsView;

namespace msc {
namespace utils {

class CoordinatesConverter
{
public:
    static void init(QGraphicsScene *scene, ChartItem *chartItem);
    static QPoint sceneToCif(const QPointF &scenePoint, bool *ok = nullptr);
    static QVector<QPoint> sceneToCif(const QVector<QPointF> &scenePoints, bool *ok = nullptr);
    static QPointF cifToScene(const QPoint &cifPoint, bool *ok = nullptr);
    static QVector<QPointF> cifToScene(const QVector<QPoint> &cifPoints, bool *ok);
    static bool sceneToCif(const QRectF sceneRect, QRect &cifRect);
    static bool cifToScene(const QRect &cifRect, QRectF &sceneRect);
    static ChartItem *currentChartItem();

    /// @{
    /** group test helper/access function */
    static void setPhysicalDPI(QPoint dpi);
    static void setLogicalDPI(QPoint dpi);
    void setScene(QGraphicsScene *scene);
    static CoordinatesConverter *instance();
    /// @}

private:
    static constexpr qreal m_mmInInch { 25.4 };
    static constexpr qreal m_cifMmScaleFactor { 10. };
    static CoordinatesConverter *m_instance;

    QPointer<QGraphicsScene> m_scene = nullptr;
    QPointer<QGraphicsView> m_view = nullptr;
    QPointer<ChartItem> m_chartItem = nullptr;
    QPoint m_dpiPhysical;
    QPoint m_dpiLogical;

    static QPoint sceneOriginInView(QGraphicsView *view);

    CoordinatesConverter();
    CoordinatesConverter(const CoordinatesConverter &other) = delete;
    CoordinatesConverter &operator=(const CoordinatesConverter &other) = delete;
};

} // ns utils
} // ns msc

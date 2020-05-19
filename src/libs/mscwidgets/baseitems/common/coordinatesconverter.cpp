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

#include "coordinatesconverter.h"

#include "graphicsview.h"

#include <QDebug>
#include <QtMath>

namespace msc {
namespace shared {

CoordinatesConverter *CoordinatesConverter::m_instance = nullptr;

CoordinatesConverter::CoordinatesConverter() { }

CoordinatesConverter *CoordinatesConverter::instance()
{
    if (!m_instance)
        m_instance = new CoordinatesConverter();

    return m_instance;
}

void CoordinatesConverter::init(QGraphicsScene *scene, ChartItem *chartItem)
{
    if (CoordinatesConverter *pInstance = instance()) {
        pInstance->m_chartItem = chartItem;
        pInstance->setScene(scene);
    }
}

void CoordinatesConverter::setScene(QGraphicsScene *scene)
{
    if (m_scene != scene)
        m_scene = scene;

    if (!m_scene)
        return;

    QGraphicsView *view = m_scene->views().isEmpty() ? nullptr : m_scene->views().first();
    const bool viewChanged = m_view != view;
    m_view = view;

    if (viewChanged && m_view) {
        setDPI(QPointF(m_view->physicalDpiX(), m_view->physicalDpiY()),
                QPointF(m_view->logicalDpiX(), m_view->logicalDpiY()));
    }

    if (!m_view)
        qWarning() << "CoordinatesConverter::setScene: received a scene with no attached view, no further scene<->mm "
                      "conversion is possible.";
    else if (viewChanged)
        qDebug() << "CoordinatesConverter: current DPI: physical:" << m_dpiPhysical << "logical:" << m_dpiLogical;
}

/*!
   \fn CoordinatesConverter::sceneOriginInView
   \returns Point (0.,0.) in scene coordinates mapped to pixels in \a view.
    Origin of CIF's Coordinates System is top left corner of a view,
    but QGraphicsScene is not bound to the view.
    This mapped point is used to perform correct CS translation.
 */
QPoint CoordinatesConverter::sceneOriginInView(QGraphicsView *view)
{
    static const QPointF sceneOrigin { 0., 0. };
    return view ? view->mapFromScene(currentChartItem()->contentRect().topLeft()) : sceneOrigin.toPoint();
}

static inline qreal viewScaleFactor(QGraphicsView *graphicsView)
{
    const auto view = qobject_cast<GraphicsView *>(graphicsView);
    return view && !qFuzzyIsNull(view->zoom()) ? view->zoom() / 100.0 : 1.0;
}

QPoint CoordinatesConverter::sceneToCif(const QPointF &scenePointSrc, bool *ok)
{
    const QPointF scenePoint = scenePointSrc;

    QPoint mmPoint = scenePoint.toPoint(); // fallback
    if (ok)
        *ok = false;

    if (CoordinatesConverter *converter = instance()) {
        if (!converter->m_dpiPhysical.isNull()) {
            if (converter->m_view) {
                const QPointF &sceneOriginPixels = sceneOriginInView(converter->m_view);
                const QPointF &targetPixels = converter->m_view->mapFromScene(scenePoint) - sceneOriginPixels;
                const QPointF &targetInch = targetPixels * m_mmInInch;

                const qreal scaleFactor = viewScaleFactor(converter->m_view);
                const qreal mmX = (targetInch.x() / converter->m_dpiPhysical.x()) / scaleFactor;
                const qreal mmY = (targetInch.y() / converter->m_dpiPhysical.y()) / scaleFactor;

                const QPointF mmPoints { mmX, mmY };
                const QPointF mmPointsCif { mmPoints * m_cifMmScaleFactor };

                mmPoint = mmPointsCif.toPoint();

                if (ok)
                    *ok = true;
            } else {
                qWarning() << "CoordinatesConverter: Can't obtain current view";
            }
        } else {
            qWarning() << "CoordinatesConverter: Can't obtain current DPI";
        }
    }
    return mmPoint;
}

bool CoordinatesConverter::sceneToCif(const QPointF &scenePoint, QPoint &cifPoint)
{
    bool converted(false);
    cifPoint = sceneToCif(scenePoint, &converted);
    return converted;
}

QVector<QPoint> CoordinatesConverter::sceneToCif(const QVector<QPointF> &scenePoints, bool *ok)
{
    if (ok)
        *ok = true;

    QVector<QPoint> pixels;

    if (const int pointsAmount = scenePoints.size()) {
        pixels.reserve(pointsAmount);
        for (const QPointF &scenePoint : scenePoints) {
            bool converted(false);
            const QPoint &cifPoint = sceneToCif(scenePoint, &converted);
            pixels << cifPoint;
            if (!converted && ok)
                *ok = false;
        }
    }

    return pixels;
}

QPointF CoordinatesConverter::cifToScene(const QPoint &cifPoint, bool *ok)
{
    QPointF scenePoint = cifPoint; // fallback
    if (ok)
        *ok = false;

    if (CoordinatesConverter *converter = instance()) {
        if (!converter->m_dpiPhysical.isNull()) {
            if (converter->m_view) {
                const QPointF sceneOriginPixels(sceneOriginInView(converter->m_view));
                const QPointF cifPointOneMm(scenePoint / m_cifMmScaleFactor);

                const qreal scaleFactor = viewScaleFactor(converter->m_view);
                const qreal pixelsX = (cifPointOneMm.x() * converter->m_dpiPhysical.x()) / m_mmInInch * scaleFactor;
                const qreal pixelsY = (cifPointOneMm.y() * converter->m_dpiPhysical.y()) / m_mmInInch * scaleFactor;
                const QPointF pixels(QPointF(pixelsX, pixelsY) + sceneOriginPixels);

                scenePoint = converter->m_view->mapToScene(pixels.toPoint());

                if (ok)
                    *ok = true;
            } else {
                qWarning() << "CoordinatesConverter: Can't obtain current view";
            }
        } else {
            qWarning() << "CoordinatesConverter: Can't obtain current DPI";
        }
    }

    return scenePoint;
}

QVector<QPointF> CoordinatesConverter::cifToScene(const QVector<QPoint> &pixelPoints, bool *ok)
{
    if (ok)
        *ok = true;

    QVector<QPointF> sceneCoords;

    if (const int pointsAmount = pixelPoints.size()) {
        sceneCoords.reserve(pointsAmount);
        for (const QPoint &pixel : pixelPoints) {
            bool converted(false);
            const QPointF &scenePoint = cifToScene(pixel, &converted);
            sceneCoords << scenePoint;
            if (!converted && ok)
                *ok = false;
        }
    }

    if (ok)
        *ok = sceneCoords.size() == pixelPoints.size();

    return sceneCoords;
}

bool CoordinatesConverter::sceneToCif(const QRectF sceneRect, QRect &cifRect)
{
    const QVector<QPointF> scenePoints { sceneRect.topLeft(), { sceneRect.width(), sceneRect.height() } };
    bool converted(false);
    const QVector<QPoint> &cifPoints = shared::CoordinatesConverter::sceneToCif(scenePoints, &converted);
    if (converted)
        cifRect = QRect(cifPoints.first(), QSize(cifPoints.last().x(), cifPoints.last().y()));

    return converted;
}

bool CoordinatesConverter::cifToScene(const QRect &cifRect, QRectF &sceneRect)
{
    const QVector<QPoint> cifPoints { cifRect.topLeft(), cifRect.bottomRight() };
    bool converted(false);
    const QVector<QPointF> scenePoints = shared::CoordinatesConverter::cifToScene(cifPoints, &converted);
    sceneRect = { scenePoints.first(), scenePoints.last() };
    return converted;
}

ChartItem *CoordinatesConverter::currentChartItem()
{
    if (CoordinatesConverter *converter = instance())
        return converter->m_chartItem;
    return nullptr;
}

void CoordinatesConverter::setDPI(const QPointF &physical, const QPointF &logical)
{
    instance()->m_dpiPhysical = physical;
    instance()->m_dpiLogical = logical;
}

QPointF CoordinatesConverter::vector2DInScene(const int mmX, const int mmY)
{
    bool converted(false);
    const QPointF &scenePoint = cifToScene({ mmX, mmY }, &converted);
    if (!converted) {
        qWarning() << "CoordinatesConverter::vector2DInScene: failed";
    }

    return converted ? scenePoint : QPointF();
}

qreal CoordinatesConverter::widthInScene(const int mmX)
{
    return CoordinatesConverter::vector2DInScene(mmX, 0).x();
}

qreal CoordinatesConverter::heightInScene(const int mmY)
{
    return CoordinatesConverter::vector2DInScene(0, mmY).y();
}

} // ns utils
} // ns msc

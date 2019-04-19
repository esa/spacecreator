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

#include "instanceitem.h"

#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/utils.h"
#include "baseitems/grippoint.h"
#include "baseitems/grippointshandler.h"
#include "baseitems/instanceenditem.h"
#include "baseitems/instanceheaditem.h"
#include "baseitems/objectslinkitem.h"
#include "baseitems/textitem.h"
#include "cif/cifblockfactory.h"
#include "cif/cifblocks.h"
#include "cif/ciflines.h"
#include "commands/common/commandsstack.h"
#include "messageitem.h"
#include "mscchart.h"
#include "mscinstance.h"

#include <QApplication>
#include <QBrush>
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QPainter>
#include <functional>

namespace msc {

InstanceItem::InstanceItem(msc::MscInstance *instance, MscChart *chart, QGraphicsItem *parent)
    : InteractiveObject(instance, parent)
    , m_instance(instance)
    , m_chart(chart)
    , m_axisSymbol(new QGraphicsLineItem(this))
    , m_headSymbol(new InstanceHeadItem(this))
    , m_endSymbol(new InstanceEndItem(m_instance->explicitStop(), this))
{
    Q_ASSERT(m_instance != nullptr);

    connect(m_instance, &msc::MscInstance::nameChanged, this, &msc::InstanceItem::setName);
    connect(m_instance, &msc::MscInstance::denominatorOrKindChanged, this, &msc::InstanceItem::setDenominatorAndKind);
    connect(m_instance, &msc::MscInstance::explicitStopChanged, this, &msc::InstanceItem::setExplicitStop);

    m_headSymbol->setName(m_instance->name());
    m_headSymbol->setKind(m_instance->denominatorAndKind());
    scheduleLayoutUpdate();

    setFlags(ItemSendsGeometryChanges | ItemIsSelectable);

    // values are based on screenshot from https://git.vikingsoftware.com/esa/msceditor/issues/30
    QPen axisPen(Qt::darkGray);
    axisPen.setWidthF(3.);
    m_axisSymbol->setPen(axisPen);

    connect(m_headSymbol, &InstanceHeadItem::nameEdited, this, &InstanceItem::onNameEdited);
    connect(m_headSymbol, &InstanceHeadItem::kindEdited, this, &InstanceItem::onKindEdited);
    connect(m_headSymbol, &InstanceHeadItem::manualMoveRequested, this, [this](const QPointF &from, const QPointF &to) {
        if (GripPoint *gp = m_gripPoints->gripPoint(GripPoint::Location::Center)) {
            onMoveRequested(gp, from, to);
        }
    });

    connect(m_headSymbol, &InstanceHeadItem::manualMoveFinished, this, [this](const QPointF &from, const QPointF &to) {
        onManualGeometryChangeFinished(GripPoint::Center, from, to);
    });

    connect(m_headSymbol, &InstanceHeadItem::layoutUpdated, this, &InstanceItem::scheduleLayoutUpdate);
}

MscInstance *InstanceItem::modelItem() const
{
    return m_instance;
}

QString InstanceItem::name() const
{
    return m_headSymbol->name();
}

QString InstanceItem::kind() const
{
    return m_headSymbol->kind();
}

void InstanceItem::setAxisHeight(qreal height, utils::CifUpdatePolicy cifUpdate)
{
    if (qFuzzyCompare(m_axisHeight, height) && cifUpdate == utils::CifUpdatePolicy::DontChange) {
        return;
    }

    m_axisHeight = height;

    bool writeCif(false);
    switch (cifUpdate) {
    case utils::CifUpdatePolicy::DontChange: {
        break;
    }
    case utils::CifUpdatePolicy::UpdateIfExists: {
        writeCif = geometryManagedByCif();
        break;
    }
    case utils::CifUpdatePolicy::ForceCreate: {
        writeCif = true;
        break;
    }
    }
    if (writeCif) {
        QSignalBlocker suppressCifChanged(this);
        updateCif();
    }

    rebuildLayout();
}

qreal InstanceItem::axisHeight() const
{
    return m_axisHeight;
}

QLineF InstanceItem::axis() const
{
    return m_axisSymbol->line().translated(pos());
}

void InstanceItem::setName(const QString &name)
{
    updatePropertyString(QLatin1String("name"), name);
    m_headSymbol->setName(name);
}

void InstanceItem::setDenominatorAndKind(const QString &kind)
{
    updatePropertyString(QLatin1String("denominatorAndKind"), kind);
    m_headSymbol->setKind(m_instance->denominatorAndKind());
}

void InstanceItem::setExplicitStop(bool exStop)
{
    m_endSymbol->setStop(exStop);
    rebuildLayout();
}

void InstanceItem::updatePropertyString(const QLatin1String &property, const QString &value)
{
    if (property.size() == 0) // QLatin1String::isEmpty was introduced in Qt 5.10
        return;

    if (m_instance->property(property.data()).toString() == value)
        return;

    m_instance->setProperty(property.data(), value);
    QMetaObject::invokeMethod(this, "reflectTextLayoutChange", Qt::QueuedConnection);
}

void InstanceItem::rebuildLayout()
{
    prepareGeometryChange();

    if (m_boundingRect.isEmpty())
        applyCif();

    const QPointF &prevP1 = m_axisSymbol->line().p1();
    QRectF headRect(m_headSymbol->boundingRect());
    const qreal endSymbolHeight = m_endSymbol->height();

    m_boundingRect.setTopLeft(headRect.topLeft());
    m_boundingRect.setWidth(headRect.width());
    m_boundingRect.setHeight(headRect.height() + m_axisHeight + endSymbolHeight);
    updateGripPoints();

    // move end symb to the bottom:
    const QRectF footerRect(m_boundingRect.left(), m_boundingRect.bottom() - endSymbolHeight, m_boundingRect.width(),
                            endSymbolHeight);
    m_endSymbol->setRect(footerRect);

    // line between the head and end symbols:
    const QPointF p1(headRect.center().x(), headRect.bottom());
    const QPointF p2 = m_endSymbol->isStop() ? footerRect.center() : QPointF(footerRect.center().x(), footerRect.top());
    m_axisSymbol->setLine(QLineF(p1, p2));

    if (prevP1 != p1) {
        // local geometry changed due the text layout change, compensate it:
        const QPointF &shift = prevP1 - p1;
        if (!shift.isNull()) {
            // Related messages to/from Env will be moved all together,
            // To avoid it block the InteractiveObject::relocated
            // (fired in InteractiveObject::itemChange)
            moveSilentlyBy(shift);
        }
    }
}

void InstanceItem::applyCif()
{
    if (const cif::CifBlockShared &cifBlock = cifBlockByType(mainCifType())) {
        const QVector<QPoint> &cifPoints = cifBlock->payload().value<QVector<QPoint>>();
        if (cifPoints.size() == 3) {
            bool converted(false);
            const QVector<QPointF> &scenePoints = utils::CoordinatesConverter::cifToScene(cifPoints, &converted);

            const QPointF &textBoxTopLeft = scenePoints.at(0);
            const QPointF &textBoxSize = scenePoints.at(1);
            const QPointF &axisHeight = scenePoints.at(2);

            QSignalBlocker keepSilent(this);
            QSignalBlocker keepSilentHeader(m_headSymbol);

            m_headSymbol->setTextboxSize({ textBoxSize.x(), textBoxSize.y() });
            const QRectF currTextBox = m_headSymbol->textBoxSceneRect();
            const QPointF shift = textBoxTopLeft - currTextBox.topLeft();
            moveBy(shift.x(), shift.y());

            m_axisHeight = axisHeight.y();
        }
    }
}

void InstanceItem::onMoveRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() != GripPoint::Location::Center)
        return;

    const QPointF delta { (to - from).x(), 0. };
    if (delta.isNull())
        return;

    setPos(pos() + delta);

    updateCif();
}

void InstanceItem::onResizeRequested(GripPoint *gp, const QPointF &from, const QPointF &to)
{
    Q_UNUSED(gp);
    Q_UNUSED(from);
    Q_UNUSED(to);
}

QPainterPath InstanceItem::shape() const
{
    QPainterPath result;
    result.addRect(m_headSymbol->boundingRect());
    result.addRect(m_endSymbol->boundingRect());
    result.addPath(ObjectsLinkItem::hoverableLine(m_axisSymbol->line()));
    return result;
}

void InstanceItem::setBoundingRect(const QRectF &geometry)
{
    if (geometry == boundingRect())
        return;

    prepareGeometryChange();
    m_boundingRect = geometry;
    if (m_gripPoints)
        m_gripPoints->updateLayout();
    scheduleLayoutUpdate();
}

InstanceItem *InstanceItem::createDefaultItem(MscInstance *instance, MscChart *chart, const QPointF &pos)
{
    InstanceItem *messageItem = new InstanceItem(instance, chart);
    messageItem->setPos(pos);

    return messageItem;
}

QPair<QPointF, bool> InstanceItem::commentPoint() const
{
    return qMakePair(QPointF(m_headSymbol->rectGeometry().right(), m_headSymbol->rectGeometry().center().y()), false);
}

void InstanceItem::prepareHoverMark()
{
    InteractiveObject::prepareHoverMark();
    m_gripPoints->setUsedPoints({ GripPoint::Location::Center });

    const qreal zVal(m_gripPoints->zValue() - 1.);
    m_headSymbol->setZValue(zVal);
    m_axisSymbol->setZValue(zVal);
    m_endSymbol->setZValue(zVal);

    connect(m_gripPoints, &GripPointsHandler::manualGeometryChangeFinish, this,
            &InstanceItem::onManualGeometryChangeFinished, Qt::UniqueConnection);
}

void InstanceItem::onNameEdited(const QString &newName)
{
    if (newName.isEmpty())
        return;

    if (m_chart->instanceByName(newName)) {
        m_headSymbol->setName(m_instance->name());
    } else {
        m_instance->setName(newName);

        using namespace msc::cmd;
        CommandsStack::push(RenameEntity, { QVariant::fromValue<MscEntity *>(this->modelItem()), newName });
    }
}

void InstanceItem::onKindEdited(const QString &newKind)
{
    using namespace msc::cmd;
    CommandsStack::push(RenameInstanceKind, { QVariant::fromValue<MscEntity *>(this->modelItem()), newKind });
    // Update to have the bold text correct
    m_headSymbol->setKind(m_instance->denominatorAndKind());
}

void InstanceItem::reflectTextLayoutChange()
{
    if (QGraphicsScene *scene = this->scene()) {
        const QRectF &myRect = sceneBoundingRect();
        const QRectF &sceneRect = scene->sceneRect();
        const QRectF &intersection = myRect.intersected(sceneRect);
        if (intersection != myRect) {
            Q_EMIT needUpdateLayout();
            return;
        }
    }
}

cif::CifLine::CifType InstanceItem::mainCifType() const
{
    return cif::CifLine::CifType::Instance;
}

void InstanceItem::updateCif()
{
    using namespace cif;

    if (!geometryManagedByCif()) {
        CifBlockShared emptyCif = CifBlockFactory::createBlockInstance();
        emptyCif->addLine(CifLineShared(new CifLineInstance()));
        m_instance->addCif(emptyCif);
    }

    const QRectF &textBoxRect = m_headSymbol->textBoxSceneRect();
    const QPointF &axisStart = axis().p1();
    const QPointF axisEnd { axisStart.x(), axisStart.y() + m_axisHeight };
    const QRectF axisRectScene { axisStart, axisEnd };

    QRect textBoxRectCif, axisRectCif;
    if (!utils::CoordinatesConverter::sceneToCif(textBoxRect, textBoxRectCif)) {
        qWarning() << Q_FUNC_INFO << "Can't convert text box coordinates to CIF";
        return;
    }

    if (!utils::CoordinatesConverter::sceneToCif(axisRectScene, axisRectCif)) {
        qWarning() << Q_FUNC_INFO << "Can't convert axis coordinates to CIF";
        return;
    }

    const CifBlockShared &cifBlock = cifBlockByType(mainCifType());
    Q_ASSERT(cifBlock != nullptr);

    const QPoint wh { textBoxRectCif.width(), textBoxRectCif.height() };
    const QPoint axisHeight { CifBlockInstance::AxisWidth, axisRectCif.height() };

    const QVector<QPoint> &storedCif = cifBlock->payload().value<QVector<QPoint>>();
    const QVector<QPoint> newCif { textBoxRectCif.topLeft(), wh, axisHeight };
    if (storedCif != newCif) {
        cifBlock->setPayload(QVariant::fromValue(newCif));
        Q_EMIT cifChanged();
    }
}

void InstanceItem::onManualGeometryChangeFinished(GripPoint::Location, const QPointF &from, const QPointF &to)
{
    std::function<QPointF(InstanceItem *, const QPointF &, const QRectF &)> avoidOverlaps;
    avoidOverlaps = [&avoidOverlaps](InstanceItem *caller, const QPointF &delta, const QRectF &shiftedRect) {
        if (delta.isNull())
            return delta;

        const QRectF &callerRect = shiftedRect.isNull() ? caller->sceneBoundingRect() : shiftedRect.translated(delta);
        for (InstanceItem *otherItem : utils::itemByPos<InstanceItem, QRectF>(caller->scene(), callerRect)) {
            if (otherItem != caller) {
                const QRectF &otherRect = otherItem->sceneBoundingRect();
                if (callerRect.intersects(otherRect)) {
                    qreal nextShiftX(0.);
                    if (delta.x() < 0)
                        nextShiftX = otherRect.left() - callerRect.right();
                    else
                        nextShiftX = otherRect.right() - callerRect.left();

                    const QPointF nextShift { nextShiftX, 0. };
                    return nextShift + avoidOverlaps(caller, nextShift, callerRect);
                }
            }
        }

        return QPointF(0., 0.);
    };

    const QPointF &delta = avoidOverlaps(this, { (to - from).x(), 0. }, QRectF());
    if (!delta.isNull())
        setPos(pos() + delta);

    const QRectF &myRect = sceneBoundingRect();
    const QRectF &chartBox = utils::CoordinatesConverter::currentChartItem()
            ? utils::CoordinatesConverter::currentChartItem()->box()
            : QRectF();
    if (!chartBox.isNull()) {
        utils::CoordinatesConverter::currentChartItem()->setBox(chartBox
                                                                | myRect.marginsAdded(ChartItem::chartMargins()));
    }

    updateCif();
    Q_EMIT cifChanged();
    Q_EMIT moved(this);
}

} // namespace msc

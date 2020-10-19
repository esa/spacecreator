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

#include "aadlchecks.h"
#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/mscutils.h"
#include "baseitems/instanceenditem.h"
#include "baseitems/instanceheaditem.h"
#include "baseitems/objectslinkitem.h"
#include "baseitems/textitem.h"
#include "chartlayoutmanager.h"
#include "cif/cifblockfactory.h"
#include "cif/cifblocks.h"
#include "cif/ciflines.h"
#include "commands/cmdchangeinstanceorder.h"
#include "commands/cmdchangeinstanceposition.h"
#include "commands/cmdentitynamechange.h"
#include "commands/cmdinstancekindchange.h"
#include "iveditorcore.h"
#include "messageitem.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "ui/grippoint.h"
#include "ui/grippointshandler.h"

#include <QApplication>
#include <QBrush>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QUndoStack>
#include <QtDebug>
#include <functional>

namespace msc {

InstanceItem::InstanceItem(
        msc::MscInstance *instance, ChartLayoutManager *chartLayoutManager, MscChart *chart, QGraphicsItem *parent)
    : InteractiveObject(instance, chartLayoutManager, parent)
    , m_instance(instance)
    , m_chart(chart)
    , m_axisSymbol(new QGraphicsLineItem(this))
    , m_headSymbol(new InstanceHeadItem(chart, this))
    , m_endSymbol(new InstanceEndItem(m_instance->explicitStop(), this))
{
    Q_ASSERT(m_instance != nullptr);

    connect(m_instance, &msc::MscInstance::nameChanged, this, &msc::InstanceItem::setName);
    connect(m_instance, &msc::MscInstance::denominatorOrKindChanged, this, &msc::InstanceItem::setDenominatorAndKind);
    connect(m_instance, &msc::MscInstance::explicitStopChanged, this, &msc::InstanceItem::setExplicitStop);

    setFlags(ItemSendsGeometryChanges | ItemIsSelectable);

    // values are based on screenshot from https://git.vikingsoftware.com/esa/msceditor/issues/30
    QPen axisPen(Qt::darkGray);
    axisPen.setWidthF(2.);
    m_axisSymbol->setPen(axisPen);

    connect(m_headSymbol, &InstanceHeadItem::nameEdited, this, &InstanceItem::onNameEdited);
    connect(m_headSymbol, &InstanceHeadItem::kindEdited, this, &InstanceItem::onKindEdited);
    connect(m_headSymbol, &InstanceHeadItem::manualMoveRequested, this,
            [this](const QPointF &from, const QPointF &to) { onManualMoveProgress(nullptr, from, to); });

    connect(m_headSymbol, &InstanceHeadItem::manualMoveFinished, this,
            [this](const QPointF &from, const QPointF &to) { onManualMoveFinish(nullptr, from, to); });

    connect(m_headSymbol, &InstanceHeadItem::layoutUpdated, this, [this]() {
        instantLayoutUpdate();
        Q_EMIT needUpdateLayout();
    });

    if (m_chartLayoutManager && m_chartLayoutManager->aadlChecker()) {
        m_headSymbol->setAadlChecker(m_chartLayoutManager->aadlChecker());
        connect(m_chartLayoutManager->aadlChecker(), &msc::AadlChecks::ivCoreChanged, this,
                &msc::InstanceItem::checkAadlFunction);
    }

    scheduleLayoutUpdate();
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

void InstanceItem::setAxisHeight(qreal height)
{
    if (qFuzzyCompare(1. + height, 1. + m_axisHeight))
        return;

    m_axisHeight = height;

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
    checkAadlFunction();
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
    const qreal xOffset = geometryManagedByCif() && boundingRect().isValid()
            ? (boundingRect().width() - m_headSymbol->boundingRect().width()) / 2
            : 0.0;

    const qreal endSymbolHeight = m_endSymbol->height();
    prepareGeometryChange();

    QRectF headRect(m_headSymbol->boundingRect());
    QRectF br = boundingRect();
    br.setWidth(headRect.width());
    br.setHeight(headRect.height() + m_axisHeight + endSymbolHeight);
    setBoundingRect(br);

    // move end symb to the bottom:
    QRectF footerRect(boundingRect());
    footerRect.setTop(footerRect.bottom() - endSymbolHeight);
    m_endSymbol->setRect(footerRect);

    // line between the head and end symbols:
    const QPointF p1(headRect.center().x(), headRect.bottom());
    const QPointF p2 = m_endSymbol->isStop() ? footerRect.center() : QPointF(footerRect.center().x(), footerRect.top());
    m_axisSymbol->setLine(QLineF(p1, p2));

    if (!qFuzzyIsNull(xOffset)) {
        moveSilentlyBy(QPointF(xOffset, 0));
        avoidOverlaps(this, QPointF(xOffset, 0), QRectF());
        if (geometryManagedByCif())
            updateCif();
        Q_EMIT needUpdateLayout();
    }
}

/*!
   If the instance is not stopped, the bottom is moved to be at the botton of the msc chart box
 */
void InstanceItem::syncHeightToChartBox()
{
    if (modelItem()->explicitStop()) {
        return;
    }

    const qreal deltaH = getChartContentBox().bottom() - sceneBoundingRect().bottom();
    if (!qFuzzyIsNull(deltaH)) {
        setAxisHeight(axisHeight() + deltaH);
    }
}

/**
    Applies the x-position of the instance if it is stored in the CIF information.
    The y-values are ignored
 */
void InstanceItem::applyCif()
{
    if (const cif::CifBlockShared &cifBlock = cifBlockByType(mainCifType())) {
        const QVector<QPoint> &cifPoints = cifBlock->payload().value<QVector<QPoint>>();
        if (cifPoints.size() == 3) {
            bool converted(false);
            const QVector<QPointF> &scenePoints = CoordinatesConverter::cifToScene(cifPoints, &converted);

            const QPointF &textBoxTopLeft = scenePoints.at(0);
            const QPointF &textBoxSize = scenePoints.at(1);

            m_headSymbol->setTextboxSize({ textBoxSize.x(), textBoxSize.y() });
            const QRectF currTextBox = m_headSymbol->textBoxSceneRect();
            const QPointF shift = textBoxTopLeft - currTextBox.topLeft();
            // TODO: should we check for overlap here?
            moveBy(shift.x(), 0.0); // Only apply the horizontal value

            rebuildLayout();
        }
    }
}

void InstanceItem::onManualMoveProgress(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    QPointF delta { (to - from).x(), 0. };
    if (delta.isNull())
        return;

    const QRectF newRect = sceneBoundingRect().translated(delta.x(), delta.y());
    const QRectF contentRect = m_chartLayoutManager->itemForChart()->contentRect();

    if (contentRect.left() > newRect.left())
        delta += QPointF(contentRect.left() - newRect.left(), 0.);
    else if (contentRect.right() < newRect.right())
        delta += QPointF(contentRect.right() - newRect.right(), 0.);

    if (!qFuzzyIsNull(delta.x()))
        setPos(pos() + delta);
}

QPainterPath InstanceItem::shape() const
{
    QPainterPath result;
    result.addRect(m_headSymbol->boundingRect());
    result.addRect(m_endSymbol->boundingRect());
    result.addPath(ObjectsLinkItem::hoverableLine(m_axisSymbol->line()));
    return result;
}

void InstanceItem::setGeometry(const QRectF &geometry)
{
    if (geometry == boundingRect())
        return;

    prepareGeometryChange();
    setBoundingRect(geometry);
    scheduleLayoutUpdate();
}

/*!
   Returns the bounding rect in scene coordinates \sa QGraphicsItem::sceneBoundingRect()
   For instances that are stopped, the box is extended to the bottom of the chart
 */
QRectF InstanceItem::extendedSceneBoundingRect() const
{
    if (!m_instance || !m_instance->explicitStop() || !m_chartLayoutManager || !m_chartLayoutManager->chartItem()) {
        return sceneBoundingRect();
    }

    const qreal chartbottom = m_chartLayoutManager->chartItem()->contentRect().bottom();
    QRectF box = sceneBoundingRect();
    box.setBottom(chartbottom);
    return box;
}

InstanceItem *InstanceItem::createDefaultItem(
        ChartLayoutManager *model, MscInstance *instance, MscChart *chart, const QPointF &pos)
{
    InstanceItem *instanceItem = new InstanceItem(instance, model, chart);
    instanceItem->setPos(pos);

    return instanceItem;
}

QPair<QPointF, bool> InstanceItem::commentPoint() const
{
    return qMakePair(QPointF(m_headSymbol->rectGeometry().right(), m_headSymbol->rectGeometry().center().y()), false);
}

void InstanceItem::postCreatePolishing()
{
    InteractiveObject::postCreatePolishing();
    m_headSymbol->showCompleter();
}

void InstanceItem::initGripPoints()
{
    InteractiveObject::initGripPoints();
    gripPointsHandler()->setUsedPoints({ shared::ui::GripPoint::Location::Center });

    const qreal zVal(gripPointsHandler()->zValue() - 1.);
    m_headSymbol->setZValue(zVal);
    m_axisSymbol->setZValue(zVal);
    m_endSymbol->setZValue(zVal);
}

void InstanceItem::onNameEdited(const QString &newName)
{
    Q_ASSERT(m_chart);
    if (newName.isEmpty()) {
        return;
    }

    m_headSymbol->removeCompleter();

    if (m_chart->instanceByName(newName)) {
        // revert to old name as the given name is used already
        m_headSymbol->setName(m_instance->name());

    } else {
        m_chartLayoutManager->undoStack()->push(
                new cmd::CmdEntityNameChange(modelEntity(), newName, m_chartLayoutManager));

        msc::AadlChecks *checker = m_chartLayoutManager ? m_chartLayoutManager->aadlChecker() : nullptr;
        if (checker && checker->hasIvCore() && !checker->functionsNames().contains(m_instance->name())) {
            const int result = QMessageBox::question(nullptr, tr("No AADL function"),
                    tr("The AADL model doesn't contain a function called:\n%1\n"
                       "\nDo you want to add it to the AADL model?")
                            .arg(newName));
            if (result == QMessageBox::Yes) {
                checker->ivCore()->addFunction(newName);
                checkAadlFunction();
            }
        }
    }
}

void InstanceItem::onKindEdited(const QString &newKind)
{
    m_chartLayoutManager->undoStack()->push(new cmd::CmdInstanceKindChange(m_instance, newKind, m_chartLayoutManager));
    // Update to have the bold text correct
    m_headSymbol->setKind(m_instance->denominatorAndKind());
}

/*!
   Checks in the aadl model, if there is a corresponding function
 */
void InstanceItem::checkAadlFunction()
{
    const bool aadlOk = aadlFunctionOk();
    QPen axisPen(aadlOk ? Qt::darkGray : msc::AadlErrorColor);
    axisPen.setWidthF(2.);
    m_axisSymbol->setPen(axisPen);
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

bool cifChangedEnough(const QVector<QPoint> &storedCif, const QVector<QPoint> newCif)
{
    if (storedCif.size() != newCif.size())
        return true;

    static constexpr int HARDCODED_TOLERANCE { 10 }; // 1mm
    for (int i = 0; i < storedCif.size(); ++i) {
        const QPoint &delta = storedCif.at(i) - newCif.at(i);
        if (qAbs(delta.x()) > HARDCODED_TOLERANCE || qAbs(delta.y()) > HARDCODED_TOLERANCE) {
            return true;
        }
    }
    return false;
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
    QRect textBoxRectCif;
    if (!CoordinatesConverter::sceneToCif(textBoxRect, textBoxRectCif)) {
        qWarning() << Q_FUNC_INFO << "Can't convert text box coordinates to CIF";
        return;
    }

    QPoint axisPointCif;
    if (!CoordinatesConverter::sceneToCif({ 0., m_axisHeight }, axisPointCif)) {
        qWarning() << Q_FUNC_INFO << "Can't convert axis coordinates to CIF";
        return;
    }

    const CifBlockShared &cifBlock = cifBlockByType(mainCifType());
    Q_ASSERT(cifBlock != nullptr);

    const QPoint wh { textBoxRectCif.width(), textBoxRectCif.height() };
    const QPoint axisHeight { CifBlockInstance::AxisWidth, axisPointCif.y() };

    const QVector<QPoint> &storedCif = cifBlock->payload().value<QVector<QPoint>>();
    const QVector<QPoint> newCif { textBoxRectCif.topLeft(), wh, axisHeight };
    if (cifChangedEnough(storedCif, newCif)) {
        cifBlock->setPayload(QVariant::fromValue(newCif));
        Q_EMIT cifChanged();
    }
}

QPointF InstanceItem::avoidOverlaps(InstanceItem *caller, const QPointF &delta, const QRectF &shiftedRect) const
{
    if (delta.isNull())
        return delta;

    const QRectF &callerRect = shiftedRect.isNull() ? caller->sceneBoundingRect() : shiftedRect.translated(delta);
    for (InstanceItem *otherItem :
            shared::graphicsviewutils::itemByPos<InstanceItem, QRectF>(caller->scene(), callerRect)) {
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
}

void InstanceItem::onManualMoveFinish(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    const QPointF &delta = avoidOverlaps(this, { (to - from).x(), 0. }, QRectF());
    if (!delta.isNull())
        setPos(pos() + delta);

    QVector<QPoint> points = prepareChangePositionCommand();
    if (!points.isEmpty()) {
        QUndoStack *undoStack = m_chartLayoutManager->undoStack();

        undoStack->beginMacro(QStringLiteral("Change Instance geometry"));
        if (!geometryManagedByCif() && m_chartLayoutManager) {
            const QVector<MscInstance *> instances = m_chartLayoutManager->currentChart()->instances();
            const int oldIdx = instances.indexOf(m_instance);
            int newIdx = 0;
            for (MscInstance *instance : instances) {
                if (instance == m_instance)
                    continue;

                if (auto item = m_chartLayoutManager->itemForInstance(instance))
                    if (x() > item->x())
                        ++newIdx;
            }

            if (oldIdx != newIdx) {
                undoStack->push(new cmd::CmdChangeInstanceOrder(m_instance, newIdx, m_chartLayoutManager));
            }
        }
        undoStack->push(new cmd::CmdChangeInstancePosition(m_instance, points));
        undoStack->endMacro();
    }
}

#ifdef QT_DEBUG
void InstanceItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    auto rectToStr = [](const QRectF &r) {
        QString res("[%1,%2] (%3,%4)");
        return res.arg(r.topLeft().x()).arg(r.topLeft().y()).arg(r.width()).arg(r.height());
    };

    if (const cif::CifBlockShared &cifBlock = cifBlockByType(mainCifType())) {
        const QPoint axisToCif = CoordinatesConverter::sceneToCif({ 0, m_axisHeight });
        const QPointF axisFromCif = CoordinatesConverter::cifToScene(axisToCif);
        QString tt = cifBlock->toString(0) + "\naxis:" + QString::number(m_axisHeight) + " "
                + QString::number(m_axisSymbol->line().length()) + "\naxisToCif:" + QString::number(axisToCif.x()) + " "
                + QString::number(axisToCif.y()) + "\naxisFromCif:" + QString::number(axisFromCif.x()) + " "
                + QString::number(axisFromCif.y()) + "\nme mbr: " + rectToStr(boundingRect())
                + "\nme bb: " + rectToStr(boundingRect()) + "\nme sbr: " + rectToStr(sceneBoundingRect());

        if (!aadlFunctionOk()) {
            tt += "\n\n" + tr("This instance does not have a corresponding AADL function in the aadl model.");
        }
        setToolTip(tt);
    }

    InteractiveObject::hoverMoveEvent(event);
}
#endif

QVector<QPoint> InstanceItem::prepareChangePositionCommand() const
{
    const QRectF &textBoxRect = m_headSymbol->textBoxSceneRect();
    QRect textBoxRectCif;
    if (!CoordinatesConverter::sceneToCif(textBoxRect, textBoxRectCif)) {
        qWarning() << Q_FUNC_INFO << "Can't convert text box coordinates to CIF";
        return {};
    }

    QPoint axisPointCif;
    if (!CoordinatesConverter::sceneToCif({ 0., m_axisHeight }, axisPointCif)) {
        qWarning() << Q_FUNC_INFO << "Can't convert axis coordinates to CIF";
        return {};
    }

    const QPoint wh { textBoxRectCif.width(), textBoxRectCif.height() };
    const QPoint axisHeight { cif::CifBlockInstance::AxisWidth, axisPointCif.y() };

    const QVector<QPoint> geometryHolder { textBoxRectCif.topLeft(), wh, axisHeight };

    return geometryHolder;
}

bool InstanceItem::aadlFunctionOk() const
{
    if (m_chartLayoutManager && m_chartLayoutManager->aadlChecker()) {
        return m_chartLayoutManager->aadlChecker()->checkInstance(m_instance);
    }
    return true;
}

void InstanceItem::setInitialXLocation(const QPointF &requested, const QRectF &chartRect, qreal horSpan)
{
    const bool isFirstInstance = m_chart->instances().size() && m_chart->instances().first() == modelItem();
    const qreal targetX = isFirstInstance ? chartRect.left() : (requested.x() + horSpan);
    const QRectF &instanceRect = sceneBoundingRect();
    const QPointF defaultShift { targetX, chartRect.top() - instanceRect.top() };
    const QPointF &totalShift = avoidOverlaps(this, defaultShift, QRectF());
    const QPointF &shift = (totalShift.x() > defaultShift.x() ? totalShift : defaultShift) - instanceRect.topLeft();

    if (!shift.isNull())
        moveBy(shift.x(), 0); // notify any attached message so it could update itself
}

QRectF InstanceItem::kindBox() const
{
    return m_headSymbol->rectGeometry();
}

InstanceHeadItem *InstanceItem::headerItem() const
{
    return m_headSymbol;
}

qreal InstanceItem::defaultAxisHeight()
{
    static constexpr int defaultAxisHeightCIF { 300 };
    static qreal defaultAxisHeightScene { 0. };

    if (qFuzzyIsNull(defaultAxisHeightScene)) {
        defaultAxisHeightScene = CoordinatesConverter::heightInScene(defaultAxisHeightCIF);
    }

    return defaultAxisHeightScene;
}

/*!
   Returns the left point where a creator message should point to.
   From the MSC standard: "The arrowhead points to the <instance head symbol> of the created instance."
   \note The retunred point is in scene coordinates
 */
QPointF InstanceItem::leftCreatorTarget() const
{
    return QPointF(sceneBoundingRect().left(), m_headSymbol->kindSceneRect().center().y());
}

/*!
   Returns the right point where a creator message should point to.
   From the MSC standard: "The arrowhead points to the <instance head symbol> of the created instance."
   \note The retunred point is in scene coordinates
 */
QPointF InstanceItem::rightCreatorTarget() const
{
    return QPointF(sceneBoundingRect().right(), m_headSymbol->kindSceneRect().center().y());
}

} // namespace msc

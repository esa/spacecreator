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
#include "baseitems/instanceenditem.h"
#include "baseitems/instanceheaditem.h"
#include "baseitems/objectslinkitem.h"
#include "chartlayoutmanagerbase.h"
#include "cif/cifblockfactory.h"
#include "cif/cifblocks.h"
#include "cif/ciflines.h"
#include "colors/colormanager.h"
#include "commands/cmdchangeinstanceorder.h"
#include "commands/cmdchangeinstanceposition.h"
#include "commands/cmdentitynamechange.h"
#include "commands/cmdinstancekindchange.h"
#include "mscchart.h"
#include "msccommandsstack.h"
#include "mscinstance.h"
#include "systemchecks.h"
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
#include <QTimer>
#include <QtDebug>
#include <functional>

namespace msc {

InstanceItem::InstanceItem(
        msc::MscInstance *instance, ChartLayoutManagerBase *chartLayoutManager, MscChart *chart, QGraphicsItem *parent)
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

    connect(m_headSymbol, &InstanceHeadItem::textChanged, this, &InstanceItem::rebuildLayout);
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
    connect(m_headSymbol, &InstanceHeadItem::editingModeOff, this, &InstanceItem::checkforInitialName);

    if (m_chartLayoutManager) {
        updateSystemChecker(m_chartLayoutManager->systemChecker());
        connect(m_chartLayoutManager, &ChartLayoutManagerBase::systemCheckerChanged, this,
                &InstanceItem::updateSystemChecker);
    }

    setDenominatorAndKind(instance->denominatorAndKind());
    setName(instance->name());

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
    Q_ASSERT(height >= 0);
    if (qFuzzyCompare(1. + height, 1. + m_axisHeight)) {
        return;
    }

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
    QTimer::singleShot(1, this, &msc::InstanceItem::checkIVFunction);
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
    const int xOffset = geometryManagedByCif() && boundingRect().isValid()
            ? qRound((boundingRect().width() - m_headSymbol->boundingRect().width()) / 2)
            : 0;

    const qreal endSymbolHeight = m_endSymbol->height();
    prepareGeometryChange();

    QRect headRect = m_headSymbol->boundingRect().toRect();
    QRectF br = boundingRect();
    br.setWidth(headRect.width());
    br.setHeight(headRect.height() + m_axisHeight + endSymbolHeight);
    setBoundingRect(br.toRect());

    // move end symbol to the bottom:
    QRectF footerRect(br.x(), br.bottom() - endSymbolHeight, br.width(), endSymbolHeight);
    m_endSymbol->setRect(footerRect.toRect());

    // line between the head and end symbols:
    const QPointF p1(headRect.center().x(), headRect.bottom());
    const QPointF p2 = m_endSymbol->isStop() ? footerRect.center() : QPointF(footerRect.center().x(), footerRect.top());
    m_axisSymbol->setLine(QLineF(p1, p2));

    if (xOffset != 0) {
        moveSilentlyBy(QPointF(xOffset, 0.));
        if (geometryManagedByCif())
            updateCif();
        Q_EMIT needUpdateLayout();
    }
}

/*!
   If the instance is not stopped, the bottom is moved to be at the bottom of the msc chart box
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
    if (!m_instance || !m_instance->explicitStop() || !m_chartLayoutManager || !m_chartLayoutManager->itemForChart()) {
        return sceneBoundingRect();
    }

    const qreal chartbottom = m_chartLayoutManager->itemForChart()->contentRect().bottom();
    QRectF box = sceneBoundingRect();
    box.setBottom(chartbottom);
    return box;
}

InstanceItem *InstanceItem::createDefaultItem(
        ChartLayoutManagerBase *model, MscInstance *instance, MscChart *chart, const QPointF &pos)
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
    m_initialName = m_instance->name();
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
        m_chartLayoutManager->undoStack()->push(new cmd::CmdEntityNameChange(modelEntity(), newName));
        checkIVFunction();
    }
}

void InstanceItem::onKindEdited(const QString &newKind)
{
    m_chartLayoutManager->undoStack()->push(new cmd::CmdInstanceKindChange(m_instance, newKind));
    // Update to have the bold text correct
    m_headSymbol->setKind(m_instance->denominatorAndKind());
}

/*!
   Check if the user did not change the initial name (directly after creating the instance)
 */
void InstanceItem::checkforInitialName()
{
    if (!m_initialName.isEmpty() && m_instance->name() == m_initialName) {
        Q_EMIT initialNameAccepted(m_instance);
        checkIVFunction();
    }
    m_initialName.clear();
}

/*!
   Checks in the iv model, if there is a corresponding function
 */
void InstanceItem::checkIVFunction()
{
    const bool isOk = ivFunctionOk();

    QPen axisPen;
    if (isOk) {
        axisPen = shared::ColorManager::instance()->colorsForItem(shared::ColorManager::InstanceLine).pen();
    } else {
        axisPen = shared::ColorManager::instance()->colorsForItem(shared::ColorManager::InstanceErrorLine).pen();
    }
    m_axisSymbol->setPen(axisPen);
}

void InstanceItem::updateSystemChecker(SystemChecks *checker)
{
    if (!m_chartLayoutManager || !m_chartLayoutManager->systemChecker()) {
        return;
    }

    m_headSymbol->setSystemChecker(m_chartLayoutManager->systemChecker());
    connect(m_chartLayoutManager->systemChecker(), &msc::SystemChecks::ivDataReset, this,
            &msc::InstanceItem::checkIVFunction);
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

shared::ColorManager::HandledColors InstanceItem::handledColorType() const
{
    return ivFunctionOk() ? shared::ColorManager::HandledColors::InstanceLine
                          : shared::ColorManager::HandledColors::InstanceErrorLine;
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

            const QPoint textBoxTopLeft = scenePoints.at(0).toPoint();
            const QPoint textBoxSize = scenePoints.at(1).toPoint();

            m_headSymbol->setTextboxSize(QSizeF(textBoxSize.x(), textBoxSize.y()));
            const QRect currTextBox = m_headSymbol->textBoxSceneRect().toRect();
            const QPoint shift = textBoxTopLeft - currTextBox.topLeft();
            if (shift.x() != 0) {
                moveBy(shift.x(), 0.0); // Only apply the horizontal value
                rebuildLayout();
            }
        }
    }
}

void InstanceItem::updateCif()
{
    using namespace cif;

    if (!geometryManagedByCif()) {
        CifBlockShared emptyCif = CifBlockFactory::createBlock({ CifLineShared(new CifLineInstance()) });
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

void InstanceItem::onManualMoveFinish(shared::ui::GripPoint *, const QPointF &from, const QPointF &to)
{
    QVector<QPoint> points = prepareChangePositionCommand();
    if (!points.isEmpty()) {
        MscCommandsStack *undoStack = m_chartLayoutManager->undoStack();

        undoStack->beginMacro(QStringLiteral("Change Instance geometry"));
        if (!geometryManagedByCif() && m_chartLayoutManager) {
            const QVector<MscInstance *> instances = m_instance->chart()->instances();
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
                undoStack->push(new cmd::CmdChangeInstanceOrder(m_instance, newIdx, m_instance->chart()));
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

        if (!ivFunctionOk()) {
            tt += "\n\n" + tr("This instance does not have a corresponding IV function in the iv model.");
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

bool InstanceItem::ivFunctionOk() const
{
    if (m_chartLayoutManager && m_chartLayoutManager->systemChecker()) {
        return m_chartLayoutManager->systemChecker()->checkInstance(m_instance);
    }
    return true;
}

QRectF InstanceItem::kindBox() const
{
    return m_headSymbol->rectGeometry();
}

InstanceHeadItem *InstanceItem::headerItem() const
{
    return m_headSymbol;
}

InstanceEndItem *InstanceItem::endItem() const
{
    return m_endSymbol;
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
   \note The returned point is in scene coordinates
 */
QPointF InstanceItem::leftCreatorTarget() const
{
    return QPointF(sceneBoundingRect().left(), m_headSymbol->kindSceneRect().center().y());
}

/*!
   Returns the right point where a creator message should point to.
   From the MSC standard: "The arrowhead points to the <instance head symbol> of the created instance."
   \note The returned point is in scene coordinates
 */
QPointF InstanceItem::rightCreatorTarget() const
{
    return QPointF(sceneBoundingRect().right(), m_headSymbol->kindSceneRect().center().y());
}

} // namespace msc

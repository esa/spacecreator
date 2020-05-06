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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "commentitem.h"

#include "ui/grippointshandler.h"
#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/objectslink.h"
#include "cif/cifblockfactory.h"
#include "cif/cifline.h"
#include "cif/ciflines.h"
#include "commands/cmdcommentitemchangegeometry.h"
#include "commands/common/commandsstack.h"
#include "mscchart.h"
#include "msccomment.h"
#include "mscentity.h"
#include "objectslinkitem.h"
#include "textitem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

static const qreal kBorderWidth = 1;
static const qreal kMargins = 6 + kBorderWidth;
static const qreal kLineLength = 20;

namespace msc {

/*!
 * \class msc::CommentItem
 *
 * Comment box item.
 */

CommentItem::CommentItem(MscChart *chart, QGraphicsItem *parent)
    : InteractiveObject(nullptr, parent)
    , m_textItem(new TextItem(this))
    , m_linkItem(new QGraphicsPathItem)
    , m_chart(chart)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_textItem->setFramed(false);
    m_textItem->setEditable(true);
    m_textItem->setBackgroundColor(Qt::transparent);
    m_textItem->setTextAllignment(Qt::AlignCenter);
    m_textItem->setTextWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    connect(m_textItem, &TextItem::edited, this, &CommentItem::textEdited);
    connect(m_textItem, &TextItem::textChanged, this, [this]() {
        prepareGeometryChange();
        setBoundingRect(m_textItem->boundingRect());
        updateGripPoints();
    });

    m_linkItem->setPen(QPen(Qt::black, kBorderWidth, Qt::DotLine));

    setHighlightable(false);
}

CommentItem::~CommentItem()
{
    if (scene())
        scene()->removeItem(m_linkItem);

    delete m_linkItem;
    m_linkItem = nullptr;
}

void CommentItem::setText(const QString &text)
{
    if (m_textItem->toPlainText() == text)
        return;

    m_textItem->setTextWidth(150);
    m_textItem->setPlainText(text);
    m_textItem->setTextWidth(m_textItem->idealWidth());

    instantLayoutUpdate();
}

QString CommentItem::text() const
{
    return m_textItem->toPlainText();
}

/*!
 * \brief CommentItem::attachTo Place this comment on an object
 * \param iObj
 */
void CommentItem::attachTo(InteractiveObject *iObj)
{
    if (m_iObj == iObj)
        return;

    if (m_iObj)
        m_iObj->disconnect(this);

    m_iObj = iObj;

    if (!m_iObj)
        return;

    m_entity = m_iObj->modelEntity()->comment();

    if (!isGlobal()) {
        connect(m_iObj, &InteractiveObject::moved, this, &CommentItem::scheduleLayoutUpdate, Qt::UniqueConnection);
        connect(m_iObj, &InteractiveObject::relocated, this, &CommentItem::scheduleLayoutUpdate, Qt::UniqueConnection);
    }

    if (auto entity = commentEntity()) {
        connect(entity, &MscComment::dataChanged, this, &CommentItem::needUpdateLayout, Qt::UniqueConnection);
        setText(entity->text());
    }

    rebuildLayout();
}

InteractiveObject *CommentItem::object() const
{
    return m_iObj;
}

bool CommentItem::geometryManagedByCif() const
{
    if (auto entity = commentEntity())
        return !entity->rect().isNull();

    return false;
}

bool CommentItem::isGlobal() const
{
    if (auto entity = commentEntity())
        return entity->isGlobal();

    return false;
}

void CommentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen = painter->pen();
    pen.setWidthF(kBorderWidth);
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);
    const QRectF br = mapRectFromItem(m_textItem, m_textItem->boundingRect());
    if ((m_iObj && isGlobal()) || (!m_iObj && m_isGlobalPreview)) {
        painter->setBrush(QColor(0xf9e29c));
        painter->drawPolygon(QVector<QPointF> { br.topRight() - QPointF(kMargins, 0), br.topLeft(), br.bottomLeft(),
                                                br.bottomRight(), br.topRight() + QPointF(0, kMargins),
                                                br.topRight() - QPointF(kMargins, 0) });
        painter->drawPolyline(QVector<QPointF> { br.topRight() + QPointF(0, kMargins),
                                                 br.topRight() - QPointF(kMargins, -kMargins),
                                                 br.topRight() - QPointF(kMargins, 0) });
    } else {
        painter->fillRect(br, QColor(0xf9e29c));
        if (m_inverseLayout)
            painter->drawPolyline(QVector<QPointF> { br.topLeft(), br.topRight(), br.bottomRight(), br.bottomLeft() });
        else
            painter->drawPolyline(QVector<QPointF> { br.topRight(), br.topLeft(), br.bottomLeft(), br.bottomRight() });
    }
    painter->restore();

    InteractiveObject::paint(painter, option, widget);
}

void CommentItem::initGripPoints()
{
    InteractiveObjectBase::initGripPoints();
    gripPointsHandler()->setUsedPoints(
            isGlobal() ? shared::ui::GripPoint::Locations { shared::ui::GripPoint::Location::Top, shared::ui::GripPoint::Location::Left,
                                                shared::ui::GripPoint::Location::Bottom, shared::ui::GripPoint::Location::Right,
                                                shared::ui::GripPoint::Location::TopLeft, shared::ui::GripPoint::Location::BottomLeft,
                                                shared::ui::GripPoint::Location::TopRight, shared::ui::GripPoint::Location::BottomRight,
                                                shared::ui::GripPoint::Location::Center }
                       : shared::ui::GripPoint::Locations { shared::ui::GripPoint::Location::Center });
}

cif::CifLine::CifType CommentItem::mainCifType() const
{
    if (auto entity = commentEntity())
        return entity->mainCifType();

    return cif::CifLine::CifType::Unknown;
}

void CommentItem::rebuildLayout()
{
    const QPair<QPointF, bool> commentData = m_iObj ? m_iObj->commentPoint() : qMakePair(QPointF(0, 0), false);
    const QPointF commentPosition = commentData.first;
    m_inverseLayout = commentData.second; // inverse layouting for comment

    if (!m_linkItem->scene() && scene() != m_linkItem->scene())
        scene()->addItem(m_linkItem);

    QRectF br = m_textItem->boundingRect();
    if (geometryManagedByCif() || isGlobal()) {
        applyCif();
        br = m_textItem->sceneBoundingRect();
    } else {
        br.moveCenter(commentPosition);
        if (m_inverseLayout)
            br.moveRight(commentPosition.x() - br.width() / 2 - kLineLength);
        else
            br.moveLeft(commentPosition.x() + br.width() / 2 + kLineLength);

        setPos(br.topLeft());
        m_textItem->setExplicitSize(br.size());
    }

    QPolygonF link;
    static const qreal tolerance = 5.;
    const bool directLink = qAbs(br.center().y() - commentPosition.y()) < tolerance;
    if (directLink) {
        if (m_inverseLayout)
            link = QPolygonF(QVector<QPointF> { QPointF(br.right(), br.center().y()), commentPosition });
        else
            link = QPolygonF(QVector<QPointF> { QPointF(br.left(), br.center().y()), commentPosition });
    } else if (!isGlobal()) {
        if (m_inverseLayout) {
            if (br.left() - commentPosition.x() > kLineLength) {
                link = QPolygonF(QVector<QPointF> {
                        QPointF(br.left(), br.center().y()), QPointF(br.left() - kLineLength / 2, br.center().y()),
                        QPointF(br.left() - kLineLength / 2, commentPosition.y()), commentPosition });
                m_inverseLayout = false;
            } else {
                link = QPolygonF(QVector<QPointF> {
                        QPointF(br.right(), br.center().y()),
                        QPointF((br.right() + commentPosition.x()) / 2, br.center().y()),
                        QPointF((br.right() + commentPosition.x()) / 2, commentPosition.y()), commentPosition });
            }
        } else {
            if (br.left() - commentPosition.x() > kLineLength) {
                link = QPolygonF(QVector<QPointF> { QPointF(br.left(), br.center().y()),
                                                    QPointF((br.left() + commentPosition.x()) / 2, br.center().y()),
                                                    QPointF((br.left() + commentPosition.x()) / 2, commentPosition.y()),
                                                    commentPosition });
            } else {
                link = QPolygonF(QVector<QPointF> {
                        QPointF(br.right(), br.center().y()), QPointF(br.right() + kLineLength / 2, br.center().y()),
                        QPointF(br.right() + kLineLength / 2, commentPosition.y()), commentPosition });
                m_inverseLayout = true;
            }
        }
    }

    prepareGeometryChange();
    setBoundingRect(m_textItem->boundingRect());

    m_linkItem->setVisible(isVisible() && !isGlobal());
    QPainterPath pp;
    pp.addPolygon(link);
    m_linkItem->setPath(pp);
}

void CommentItem::applyCif()
{
    if (!m_iObj)
        return;

    const QRect storedCifRect = commentEntity()->rect();
    if (storedCifRect.isNull())
        return;

    QRectF rect;
    if (!utils::CoordinatesConverter::cifToScene(storedCifRect, rect))
        qWarning() << "ChartItem: Coordinates conversion (mm->scene) failed" << storedCifRect;

    if (!rect.isNull() && rect != m_textItem->sceneBoundingRect()) {
        setPos(rect.topLeft());
        prepareGeometryChange();
        m_textItem->setExplicitSize(rect.size());
    }
}

void CommentItem::updateCif()
{
    if (!m_iObj)
        return;

    const QRect storedCifRect = commentEntity()->rect();
    const QRectF textItemRect = m_textItem->sceneBoundingRect();

    QRect cifRect;
    if (!utils::CoordinatesConverter::sceneToCif(textItemRect, cifRect))
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << cifRect;
    else if (storedCifRect == textItemRect)
        return;

    commentEntity()->setRect(cifRect);
}

void CommentItem::setGlobalPreview(bool isGlobalPreview)
{
    m_isGlobalPreview = isGlobalPreview;
    update();
}

void CommentItem::onManualMoveProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to)
{
    if (gp->location() != shared::ui::GripPoint::Location::Center)
        return;

    const QRectF contentRect = utils::CoordinatesConverter::currentChartItem()->contentRect();
    QPointF newPos = pos() + (to - from);
    if (newPos.x() < contentRect.left())
        newPos.setX(contentRect.left());
    else if ((newPos.x() + boundingRect().width()) > contentRect.right())
        newPos.setX(contentRect.right() - boundingRect().width());
    if (newPos.y() < contentRect.top())
        newPos.setY(contentRect.top());
    else if ((newPos.y() + boundingRect().height()) > contentRect.bottom())
        newPos.setY(contentRect.bottom() - boundingRect().height());

    QRect oldRect;
    if (geometryManagedByCif()) {
        oldRect = commentEntity()->rect();
    } else if (!utils::CoordinatesConverter::sceneToCif(m_textItem->sceneBoundingRect(), oldRect)) {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << oldRect;
        return;
    }

    QRectF rect { newPos, boundingRect().size() };
    QRect newRect;
    if (utils::CoordinatesConverter::sceneToCif(rect, newRect)) {
        msc::cmd::CommandsStack::push(msc::cmd::ChangeCommentGeometry,
                                      { QVariant::fromValue<MscChart *>(m_chart), oldRect, newRect,
                                        QVariant::fromValue<MscEntity *>(m_iObj->modelEntity()) });

        rebuildLayout();
        updateGripPoints();

        Q_EMIT needUpdateLayout();
    } else {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << rect;
    }

    Q_EMIT needUpdateLayout();
}

void CommentItem::onManualResizeProgress(shared::ui::GripPoint *gp, const QPointF &from, const QPointF &to)
{
    const QPoint shift = QPointF(to - from).toPoint();
    QRect rect = m_textItem->sceneBoundingRect().toRect();
    switch (gp->location()) {
    case shared::ui::GripPoint::Left:
        rect.setLeft(rect.left() + shift.x());
        break;
    case shared::ui::GripPoint::Top:
        rect.setTop(rect.top() + shift.y());
        break;
    case shared::ui::GripPoint::Right:
        rect.setRight(rect.right() + shift.x());
        break;
    case shared::ui::GripPoint::Bottom:
        rect.setBottom(rect.bottom() + shift.y());
        break;
    case shared::ui::GripPoint::TopLeft:
        rect.setTopLeft(rect.topLeft() + shift);
        break;
    case shared::ui::GripPoint::TopRight:
        rect.setTopRight(rect.topRight() + shift);
        break;
    case shared::ui::GripPoint::BottomLeft:
        rect.setBottomLeft(rect.bottomLeft() + shift);
        break;
    case shared::ui::GripPoint::BottomRight:
        rect.setBottomRight(rect.bottomRight() + shift);
        break;
    default:
        qWarning() << "Update grip point handling";
        break;
    }

    QRect oldRect;
    if (geometryManagedByCif()) {
        oldRect = commentEntity()->rect();
    } else if (!utils::CoordinatesConverter::sceneToCif(m_textItem->sceneBoundingRect(), oldRect)) {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << oldRect;
        return;
    }

    QRect newRect;
    if (utils::CoordinatesConverter::sceneToCif(rect, newRect)) {
        msc::cmd::CommandsStack::push(msc::cmd::ChangeCommentGeometry,
                                      { QVariant::fromValue<MscChart *>(m_chart), oldRect, newRect,
                                        QVariant::fromValue<MscEntity *>(m_iObj->modelEntity()) });

        rebuildLayout();
        updateGripPoints();

        Q_EMIT needUpdateLayout();
    } else {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << rect;
    }
}

/*!
 * \brief CommentItem::textEdited Update the text in the comment
 * \param text
 */
void CommentItem::textEdited(const QString &text)
{
    const QRect oldRect = commentEntity() ? commentEntity()->rect() : QRect();
    const QString oldText = commentEntity() ? commentEntity()->text() : QString();
    QRect newRect;
    if (utils::CoordinatesConverter::sceneToCif(m_textItem->sceneBoundingRect(), newRect)) {
        if (oldRect != newRect || oldText != text) {
            msc::cmd::CommandsStack::current()->beginMacro(tr("Change comment"));
            msc::cmd::CommandsStack::push(msc::cmd::ChangeCommentGeometry,
                                          { QVariant::fromValue<MscChart *>(m_chart), oldRect, newRect,
                                            QVariant::fromValue<MscEntity *>(m_iObj->modelEntity()) });
            msc::cmd::CommandsStack::push(msc::cmd::Id::ChangeComment,
                                          { QVariant::fromValue<MscChart *>(m_chart),
                                            QVariant::fromValue<msc::MscEntity *>(m_iObj->modelEntity()), text });
            msc::cmd::CommandsStack::current()->endMacro();
        }
    }
    rebuildLayout();
    updateGripPoints();

    Q_EMIT needUpdateLayout();
}

MscComment *CommentItem::commentEntity() const
{
    return qobject_cast<MscComment *>(m_entity);
}

void CommentItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    m_textItem->enableEditMode();
    InteractiveObject::mouseDoubleClickEvent(event);
}

QPainterPath CommentItem::shape() const
{
    QPainterPath path;
    path.addRect(m_textItem->boundingRect());
    path.addPath(m_linkItem->shape());
    return path;
}

} // ns msc

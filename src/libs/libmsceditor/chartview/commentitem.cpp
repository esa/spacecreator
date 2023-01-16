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

#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/textitem.h"
#include "chartlayoutmanagerbase.h"
#include "cif/cifline.h"
#include "colors/colormanager.h"
#include "commands/cmdcommentitemchangegeometry.h"
#include "commands/cmdentitycommentchange.h"
#include "mscchart.h"
#include "mscchartviewconstants.h"
#include "msccommandsstack.h"
#include "msccomment.h"
#include "mscentity.h"
#include "ui/grippointshandler.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QtDebug>

namespace msc {

/*!
 * \class msc::CommentItem
 *
 * Comment box item.
 */

CommentItem::CommentItem(MscChart *chart, ChartLayoutManagerBase *chartLayoutManager, QGraphicsItem *parent)
    : InteractiveObject(nullptr, chartLayoutManager, parent)
    , m_textItem(new TextItem(this))
    , m_linkItem(new QGraphicsPathItem)
    , m_chart(chart)
{
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_textItem->setMscValidationTest(QString("msc c1;instance i1 comment '%1';endinstance;endmsc;"));
    m_textItem->setFramed(false);
    m_textItem->setEditable(true);
    m_textItem->setBackground(Qt::transparent);
    m_textItem->setTextAlignment(Qt::AlignLeft);
    m_textItem->setTextWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    m_textItem->setMultilineEnabled(true);
    connect(m_textItem, &TextItem::edited, this, &CommentItem::textEdited);
    connect(m_textItem, &TextItem::textChanged, this, [this]() {
        setBoundingRect(m_textItem->boundingRect());
        updateGripPoints();
    });

    shared::ColorHandler color = shared::ColorManager::instance()->colorsForItem(shared::ColorManager::CommentMsc);
    QPen pen = color.pen();
    pen.setStyle(Qt::DotLine);
    m_linkItem->setPen(pen);

    setHighlightable(false);

    connect(this, &InteractiveObject::relocated, this, [&]() { updateCif(); });
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

    m_textItem->setExplicitSize(QSizeF());
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
        connect(m_iObj, &InteractiveObject::relocated, this, &CommentItem::onLinkedItemMoved, Qt::UniqueConnection);
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

    shared::ColorHandler color = shared::ColorManager::instance()->colorsForItem(shared::ColorManager::CommentMsc);
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen = color.pen();
    pen.setCapStyle(Qt::FlatCap);
    painter->setPen(pen);
    const QRectF br = mapRectFromItem(m_textItem, m_textItem->boundingRect());
    if ((m_iObj && isGlobal()) || (!m_iObj && m_isGlobalPreview)) {
        painter->setBrush(color.brush());
        painter->drawPolygon(QVector<QPointF> { br.topRight() - QPointF(COMMENT_MARGIN, 0), br.topLeft(),
                br.bottomLeft(), br.bottomRight(), br.topRight() + QPointF(0, COMMENT_MARGIN),
                br.topRight() - QPointF(COMMENT_MARGIN, 0) });
        painter->drawPolyline(QVector<QPointF> { br.topRight() + QPointF(0, COMMENT_MARGIN),
                br.topRight() - QPointF(COMMENT_MARGIN, -COMMENT_MARGIN), br.topRight() - QPointF(COMMENT_MARGIN, 0) });
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
    gripPointsHandler()->setUsedPoints(isGlobal()
                    ? shared::ui::GripPoint::Locations { shared::ui::GripPoint::Location::Top,
                            shared::ui::GripPoint::Location::Left, shared::ui::GripPoint::Location::Bottom,
                            shared::ui::GripPoint::Location::Right, shared::ui::GripPoint::Location::TopLeft,
                            shared::ui::GripPoint::Location::BottomLeft, shared::ui::GripPoint::Location::TopRight,
                            shared::ui::GripPoint::Location::BottomRight, shared::ui::GripPoint::Location::Center }
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

    if (geometryManagedByCif() || isGlobal()) {
        applyCif();
    } else {
        QRectF br = m_textItem->boundingRect();
        br.moveCenter(commentPosition);
        if (m_inverseLayout)
            br.moveRight(commentPosition.x() - br.width() / 2 - COMMENT_LINE_LENGTH);
        else
            br.moveLeft(commentPosition.x() + br.width() / 2 + COMMENT_LINE_LENGTH);

        m_textItem->setExplicitSize(br.size());
        setPos(br.topLeft());
    }

    setBoundingRect(m_textItem->boundingRect());

    updateLinkPath();
}

void CommentItem::updateLinkPath()
{
    const QPair<QPointF, bool> commentData = m_iObj ? m_iObj->commentPoint() : qMakePair(QPointF(0, 0), false);
    const QPointF commentPosition = commentData.first;
    m_inverseLayout = commentData.second; // inverse layouting for comment

    QRectF br = m_textItem->boundingRect();
    br.moveTopLeft(pos());

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
            if (br.left() - commentPosition.x() > COMMENT_LINE_LENGTH) {
                link = QPolygonF(QVector<QPointF> { QPointF(br.left(), br.center().y()),
                        QPointF(br.left() - COMMENT_LINE_LENGTH / 2, br.center().y()),
                        QPointF(br.left() - COMMENT_LINE_LENGTH / 2, commentPosition.y()), commentPosition });
                m_inverseLayout = false;
            } else {
                link = QPolygonF(QVector<QPointF> { QPointF(br.right(), br.center().y()),
                        QPointF((br.right() + commentPosition.x()) / 2, br.center().y()),
                        QPointF((br.right() + commentPosition.x()) / 2, commentPosition.y()), commentPosition });
            }
        } else {
            if (br.left() - commentPosition.x() > COMMENT_LINE_LENGTH) {
                link = QPolygonF(QVector<QPointF> { QPointF(br.left(), br.center().y()),
                        QPointF((br.left() + commentPosition.x()) / 2, br.center().y()),
                        QPointF((br.left() + commentPosition.x()) / 2, commentPosition.y()), commentPosition });
            } else {
                link = QPolygonF(QVector<QPointF> { QPointF(br.right(), br.center().y()),
                        QPointF(br.right() + COMMENT_LINE_LENGTH / 2, br.center().y()),
                        QPointF(br.right() + COMMENT_LINE_LENGTH / 2, commentPosition.y()), commentPosition });
                m_inverseLayout = true;
            }
        }
    }

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
    if (!CoordinatesConverter::cifToScene(storedCifRect, rect))
        qWarning() << "ChartItem: Coordinates conversion (mm->scene) failed" << storedCifRect;

    if (!rect.isNull() && rect != m_textItem->sceneBoundingRect()) {
        m_textItem->setExplicitSize(rect.size());
        setPos(rect.topLeft());
    }
}

void CommentItem::updateCif()
{
    if (!m_iObj)
        return;

    const QRect storedCifRect = commentEntity()->rect();
    const QRectF textItemRect = m_textItem->sceneBoundingRect();

    QRect cifRect;
    if (!CoordinatesConverter::sceneToCif(textItemRect, cifRect))
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

    const QRectF contentRect = CoordinatesConverter::currentChartItem()->contentRect();
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
    } else if (!CoordinatesConverter::sceneToCif(m_textItem->sceneBoundingRect(), oldRect)) {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << oldRect;
        return;
    }

    QRectF rect { newPos, boundingRect().size() };
    QRect newRect;
    if (CoordinatesConverter::sceneToCif(rect, newRect)) {
        m_chartLayoutManager->undoStack()->push(
                new cmd::CmdCommentItemChangeGeometry(oldRect, newRect, m_iObj->modelEntity(), m_chart));

        rebuildLayout();
        updateGripPoints();
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
    } else if (!CoordinatesConverter::sceneToCif(m_textItem->sceneBoundingRect(), oldRect)) {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << oldRect;
        return;
    }

    QRect newRect;
    if (CoordinatesConverter::sceneToCif(rect, newRect)) {
        m_chartLayoutManager->undoStack()->push(
                new cmd::CmdCommentItemChangeGeometry(oldRect, newRect, m_iObj->modelEntity(), m_chart));

        rebuildLayout();
        updateGripPoints();

        Q_EMIT needUpdateLayout();
    } else {
        qWarning() << "ChartItem: Coordinates conversion (scene->mm) failed" << rect;
    }
}

void CommentItem::onLinkedItemMoved(const QPointF &from, const QPointF &to)
{
    moveBy(to.x() - from.x(), to.y() - from.y());
    updateLinkPath();
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
    if (CoordinatesConverter::sceneToCif(m_textItem->sceneBoundingRect(), newRect)) {
        if (oldRect != newRect || oldText != text) {
            MscCommandsStack *undoStack = m_chartLayoutManager->undoStack();
            undoStack->beginMacro(tr("Change comment"));
            undoStack->push(new cmd::CmdCommentItemChangeGeometry(oldRect, newRect, m_iObj->modelEntity(), m_chart));
            undoStack->push(new cmd::CmdEntityCommentChange(m_iObj->modelEntity(), text, m_chart));
            undoStack->endMacro();
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

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

#include "instanceheaditem.h"

#include "baseitems/common/coordinatesconverter.h"
#include "baseitems/common/mscutils.h"
#include "baseitems/textitem.h"
#include "colors/colormanager.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "systemchecks.h"

#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace msc {

/*!
  \class msc:: InstanceHeadItem

  Item showing the header of an instance
  */

InstanceHeadItem::InstanceHeadItem(MscChart *chart, QGraphicsItem *parent)
    : QGraphicsObject(parent)
    , m_textItemName(new NameItem(this))
    , m_rectItem(new QGraphicsRectItem(this))
    , m_textItemKind(new TextItem(this))
    , m_chart(chart)
{
    m_textItemKind->setBackground(Qt::transparent);
    m_textItemKind->setEditable(true);
    m_textItemKind->setTextWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    m_textItemName->setBackground(Qt::transparent);
    m_textItemName->setEditable(true);
    m_textItemName->setTextWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    connect(m_textItemName, &TextItem::edited, this, [this](const QString &txt) {
        m_explicitTextBox = QRectF();
        m_textItemName->adjustSize();
        updateLayout();
        Q_EMIT nameEdited(txt);
    });

    connect(m_textItemKind, &TextItem::edited, this, [this](const QString &txt) {
        m_explicitTextBox = QRectF();
        m_textItemKind->adjustSize();
        updateLayout();
        Q_EMIT kindEdited(txt);
    });

    connect(m_textItemName, &shared::ui::TextItem::textChanged, this, &InstanceHeadItem::updateGeometryOnTextChange);
    connect(m_textItemKind, &shared::ui::TextItem::textChanged, this, &InstanceHeadItem::updateGeometryOnTextChange);

    m_rectItem->setCursor(Qt::SizeAllCursor);
    m_textItemKind->setCursor(Qt::SizeAllCursor); // TODO: restore regular cursor for editing
}

QString InstanceHeadItem::name() const
{
    return m_textItemName->toPlainText();
}

QString InstanceHeadItem::kind() const
{
    return m_textItemKind->toPlainText();
}

void InstanceHeadItem::setName(const QString &name)
{
    if (name == this->name() || m_textItemName->isEditing()) {
        return;
    }

    m_explicitTextBox = QRectF();
    m_textItemName->setPlainText(name);
    m_textItemName->adjustSize();

    updateLayout();
}

void InstanceHeadItem::setKind(const QString &kind)
{
    if (kind == m_textItemKind->toPlainText() || m_textItemKind->isEditing()) {
        return;
    }

    QString denominator;
    QString kindText;
    MscInstance::splitDenominatorKind(kind, denominator, kindText);

    m_explicitTextBox = QRectF();
    if (denominator.isEmpty())
        m_textItemKind->setPlainText(kindText);
    else
        m_textItemKind->setHtml(QString("<b>%1</b> %2").arg(denominator, kindText));
    m_textItemKind->adjustSize();

    updateLayout();
}

void InstanceHeadItem::updateLayout()
{
    prepareGeometryChange();

    const QSizeF defaultSizeScene = defaultSize();
    // prepare symbol's rect:
    const qreal width =
            qMax(defaultSizeScene.width(), qMax(m_textItemName->idealWidth(), m_textItemKind->idealWidth()));

    if (!qFuzzyIsNull(m_textItemKind->textWidth())
            && (m_textItemKind->textWidth() - width) / m_textItemKind->textWidth() > 0.1) {
        m_textItemName->setTextWidth(width);
    }
    if (!qFuzzyIsNull(m_textItemKind->textWidth())
            && (m_textItemKind->textWidth() - width) / m_textItemKind->textWidth() > 0.1) {
        m_textItemKind->setTextWidth(width);
    }
    QRectF nameRect = m_textItemName->boundingRect();
    QRectF kindRect = m_textItemKind->boundingRect();

    QRectF symbolRect(m_explicitTextBox);
    if (symbolRect.isEmpty()) {
        symbolRect = { 0., 0., width, qMax(kindRect.height(), defaultSizeScene.height()) };

        static const qreal padding = 5.;
        symbolRect.adjust(-padding, -padding, padding, padding);

        symbolRect.moveTopLeft({ 0., 0. });
        m_explicitTextBox = symbolRect;
    }

    // center name horizontally:
    nameRect.moveCenter(symbolRect.center());
    nameRect.moveTop(0.);

    // shift box downward:
    symbolRect.moveTop(nameRect.bottom());
    m_rectItem->setRect(symbolRect);

    // center kind in the box:
    kindRect.moveCenter(symbolRect.center());

    // place items:
    auto moveItem = [](QGraphicsItem *item, const QPointF &newCenter) {
        const QRectF &currBounds = item->boundingRect().translated(item->pos());
        const QPointF &currCenter = currBounds.center();
        const QPointF &shift = newCenter - currCenter;
        item->moveBy(shift.x(), shift.y());
    };

    moveItem(m_textItemName, nameRect.center());
    moveItem(m_textItemKind, kindRect.center());
    moveItem(m_rectItem, symbolRect.center());

    // update head gradient:
    shared::ColorHandler color = shared::ColorManager::instance()->colorsForItem(shared::ColorManager::InstanceHead);
    m_rectItem->setPen(color.pen());
    m_rectItem->setBrush(color.brush());

    Q_EMIT layoutUpdated();
}

void InstanceHeadItem::updateGeometryOnTextChange()
{
    m_explicitTextBox = QRectF();
    m_textItemName->adjustSize();
    m_textItemKind->adjustSize();
    updateLayout();
    Q_EMIT textChanged();
}

QRectF InstanceHeadItem::boundingRect() const
{
    return m_textItemName->boundingRect() | m_rectItem->rect();
}

QRectF InstanceHeadItem::rectGeometry() const
{
    return m_rectItem->sceneBoundingRect();
}

void InstanceHeadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void InstanceHeadItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_manualMovementFrom =
                (!m_textItemKind->isEditing() && m_rectItem->contains(event->pos())) ? event->scenePos() : QPointF();

        m_manualMovementTo = m_manualMovementFrom;

        event->accept();

        if (QGraphicsItem *parent = parentItem()) {
            if (event->modifiers() == Qt::NoModifier)
                scene()->clearSelection();

            parent->setSelected(!parent->isSelected());
        }
    } else
        event->ignore();
}

void InstanceHeadItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    const bool posDiffers = m_manualMovementFrom != m_manualMovementTo;
    if (posDiffers)
        Q_EMIT manualMoveFinished(m_manualMovementFrom, event->scenePos());

    m_manualMovementFrom = m_manualMovementTo = QPointF();
}

void InstanceHeadItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // Check for click in for the name
    if (m_textItemName->contains(m_textItemName->mapFromScene(event->scenePos()))) {
        m_textItemName->enableEditMode();
        showCompleter();
        m_manualMovementFrom = m_manualMovementTo = QPointF();
    }

    // Check for click in for the kind
    if (m_rectItem->contains(m_rectItem->mapFromScene(event->scenePos()))) {
        m_textItemKind->enableEditMode();
        m_manualMovementFrom = m_manualMovementTo = QPointF();
    }
}

void InstanceHeadItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_manualMovementFrom.isNull() && event->lastScenePos() != event->scenePos()) {
        m_manualMovementTo = event->scenePos();
        Q_EMIT manualMoveRequested(event->lastScenePos(), m_manualMovementTo);
    }
}

void InstanceHeadItem::setTextboxSize(const QSizeF &size)
{
    const QRectF newTextBox { QPointF(0., 0.), size };
    if (newTextBox != m_explicitTextBox) {
        m_explicitTextBox = newTextBox;
        QSignalBlocker keepSilent(this);
        m_textItemKind->adjustSize();
        m_textItemName->adjustSize();
        updateLayout();
    }
}

QRectF InstanceHeadItem::textBoxSceneRect() const
{
    return m_rectItem->mapRectToScene(m_rectItem->rect());
}

/*!
   Returns the bounding box of the kind (yellow box) in scene coordinates
 */
QRectF InstanceHeadItem::kindSceneRect() const
{
    return m_rectItem->sceneBoundingRect();
}

QSizeF InstanceHeadItem::defaultSize()
{
    static QSizeF sizeScene { 0., 0. };
    if (sizeScene.isEmpty()) {
        const QSize sizeCIF { 100, 50 };
        const QPointF sz = CoordinatesConverter::vector2DInScene(sizeCIF.width(), sizeCIF.height());
        sizeScene = { sz.x(), sz.y() };
    }

    return sizeScene;
}

/*!
   Set the aadl checker to verify data with the aadl model
 */
void InstanceHeadItem::setSystemChecker(SystemChecks *checker)
{
    m_checker = checker;
}

/*!
   Updates and show the name completer with all function names
 */
void InstanceHeadItem::showCompleter()
{
    if (m_checker) {
        QStringList functionNames = m_checker->functionsNames();
        if (!m_chart.isNull()) {
            QStringList instanceNames;
            for (MscInstance *instance : m_chart->instances()) {
                instanceNames.append(instance->name());
            }
            int i = 0;
            while (i < functionNames.size()) {
                if (instanceNames.contains(functionNames[i], m_checker->stringSensitivity())) {
                    functionNames.removeAt(i);
                } else {
                    ++i;
                }
            }
        }
        m_textItemName->updateCompleter(functionNames);
    }
}

/*!
   Removes the completer if it is used.
 */
void InstanceHeadItem::removeCompleter()
{
    m_textItemName->removeCompleter();
}

} // ns msc

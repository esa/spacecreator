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

#include "aadlcommentgraphicsitem.h"

#include "aadlfunctiongraphicsitem.h"
#include "aadlobjectcomment.h"
#include "baseitems/common/aadlutils.h"
#include "baseitems/textgraphicsitem.h"
#include "colors/colormanager.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "commandsstack.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <QTextLayout>
#include <QTextLine>
#include <QtDebug>

static const qreal kBorderWidth = 2;
static const qreal kMargins = 14 + kBorderWidth;

namespace aadlinterface {

AADLCommentGraphicsItem::AADLCommentGraphicsItem(aadl::AADLObjectComment *comment, QGraphicsItem *parent)
    : AADLRectGraphicsItem(comment, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFont(qApp->font());
    setZValue(ZOrder.Comment);

    applyColorScheme();

    connect(comment, &aadl::AADLObject::titleChanged, this, &InteractiveObject::updateGraphicsItem);
}

void AADLCommentGraphicsItem::updateFromEntity()
{
    AADLRectGraphicsItem::updateFromEntity();
    setText(entity()->title());
}

void AADLCommentGraphicsItem::setText(const QString &text)
{
    if (m_text == text)
        return;

    m_text = text;

    instantLayoutUpdate();
}

QString AADLCommentGraphicsItem::text() const
{
    return m_text;
}

aadl::AADLObjectComment *AADLCommentGraphicsItem::entity() const
{
    return qobject_cast<aadl::AADLObjectComment *>(aadlObject());
}

void AADLCommentGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());

    const QRectF br = boundingRect();
    auto preparePolygon = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() - QPointF(kMargins, 0), rect.topLeft(), rect.bottomLeft(),
            rect.bottomRight(), rect.topRight() + QPointF(0, kMargins), rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolygon(preparePolygon(br));

    auto preparePolyline = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() + QPointF(0, kMargins),
            rect.topRight() - QPointF(kMargins, -kMargins), rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolyline(preparePolyline(br));

    qreal y = kMargins;
    const qreal lineWidth = boundingRect().width() - 2 * kMargins;
    for (auto line : m_text.split("\n")) {
        QTextLayout textLayout(line);
        textLayout.setFont(font());
        textLayout.beginLayout();
        while (true) {
            QTextLine textLine = textLayout.createLine();
            if (!textLine.isValid()) {
                break;
            }

            textLine.setPosition(QPointF(kMargins, y));
            textLine.setLineWidth(lineWidth);
            y += textLine.height();
        }
        textLayout.endLayout();
        textLayout.draw(painter, QPointF(0, 0));
    }

    painter->restore();
    AADLRectGraphicsItem::paint(painter, option, widget);
}

void AADLCommentGraphicsItem::rebuildLayout()
{
    AADLRectGraphicsItem::rebuildLayout();
    update();
}

QSizeF AADLCommentGraphicsItem::minimalSize() const
{
    return DefaultGraphicsItemSize;
}

ColorManager::HandledColors AADLCommentGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::Comment;
}

void AADLCommentGraphicsItem::applyColorScheme()
{
    const ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

bool AADLCommentGraphicsItem::allowGeometryChange(const QPointF & /*from*/, const QPointF & /*to*/)
{
    return true;
}

}

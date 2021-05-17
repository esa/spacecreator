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

#include "ivcommentgraphicsitem.h"

#include "baseitems/common/ivutils.h"
#include "colors/colormanager.h"
#include "graphicsitemhelpers.h"
#include "graphicsviewutils.h"
#include "ivcomment.h"
#include "ivfunctiongraphicsitem.h"
#include "ui/textitem.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <QTextLayout>
#include <QTextLine>
#include <QtDebug>

static const qreal kBorderWidth = 2;
static const qreal kMargins = 14 + kBorderWidth;

namespace ive {

IVCommentGraphicsItem::IVCommentGraphicsItem(ivm::IVComment *comment, QGraphicsItem *parent)
    : shared::ui::VERectGraphicsItem(comment, parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFont(qApp->font());
    setZValue(ZOrder.Comment);
}

void IVCommentGraphicsItem::init()
{
    shared::ui::VERectGraphicsItem::init();
    connect(entity(), &ivm::IVObject::titleChanged, this, &shared::ui::VEInteractiveObject::updateGraphicsItem);
}

void IVCommentGraphicsItem::updateFromEntity()
{
    shared::ui::VERectGraphicsItem::updateFromEntity();
    setText(entity()->titleUI());
}

int IVCommentGraphicsItem::itemLevel(bool isSelected) const
{
    return gi::itemLevel(entity(), isSelected);
}

void IVCommentGraphicsItem::setText(const QString &text)
{
    if (m_text == text)
        return;

    m_text = text;

    instantLayoutUpdate();
}

QString IVCommentGraphicsItem::text() const
{
    return m_text;
}

ivm::IVComment *IVCommentGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVComment *>(m_dataObject);
}

void IVCommentGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->setFont(font());

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
    const qreal maxY = boundingRect().height() - kMargins;
    const QFontMetricsF fm(font());
    bool complete = false;
    for (auto line : m_text.split(QLatin1Char('\n'))) {
        QTextLayout textLayout(line);
        textLayout.setFont(font());
        textLayout.beginLayout();
        while (true) {
            QTextLine textLine = textLayout.createLine();
            if (!textLine.isValid()) {
                break;
            }

            textLine.setLineWidth(lineWidth);
            if (maxY < y + textLine.height()) {
                const QString lastLine = line.mid(textLine.textStart());
                const QString elidedLastLine = fm.elidedText(lastLine, Qt::ElideRight, lineWidth);
                painter->drawText(QPointF(kMargins, y + fm.ascent()), elidedLastLine);
                complete = true;
                break;
            }

            textLine.draw(painter, QPointF(kMargins, y));
            y += textLine.height();
        }
        textLayout.endLayout();
        if (complete)
            break;
    }

    painter->restore();
    shared::ui::VERectGraphicsItem::paint(painter, option, widget);
}

void IVCommentGraphicsItem::rebuildLayout()
{
    shared::ui::VERectGraphicsItem::rebuildLayout();
    setVisible(entity() && (gi::nestingLevel(entity()) >= gi::kNestingVisibilityLevel || entity()->isRootObject())
            && entity()->isVisible());
    update();
}

QSizeF IVCommentGraphicsItem::minimalSize() const
{
    return shared::graphicsviewutils::kDefaultGraphicsItemSize;
}

shared::ColorManager::HandledColors IVCommentGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Comment;
}

void IVCommentGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    QPen pen = h.pen();
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    setPen(pen);
    setBrush(h.brush());
    update();
}

}

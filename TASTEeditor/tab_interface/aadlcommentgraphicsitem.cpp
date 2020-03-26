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
#include "baseitems/common/utils.h"
#include "baseitems/textgraphicsitem.h"
#include "colors/colormanager.h"
#include "commands/commandids.h"
#include "commands/commandsfactory.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QPainter>
#include <QTextLayout>
#include <QTextLine>
#include <QtDebug>
#include <app/commandsstack.h>
#include <baseitems/grippointshandler.h>

static const qreal kBorderWidth = 2;
static const qreal kMargins = 14 + kBorderWidth;

namespace taste3 {
namespace aadl {

AADLCommentGraphicsItem::AADLCommentGraphicsItem(AADLObjectComment *comment, QGraphicsItem *parent)
    : AADLRectGraphicsItem(comment, parent)
    , m_textLayout(new QTextLayout)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFont(QFont(qApp->font()));
    setZValue(utils::kCommentZLevel);

    applyColorScheme();
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
    updateTextLayout(text);

    instantLayoutUpdate();
}

QString AADLCommentGraphicsItem::text() const
{
    return m_text;
}

AADLObjectComment *AADLCommentGraphicsItem::entity() const
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
        return QVector<QPointF> { rect.topRight() - QPointF(kMargins, 0),
                                  rect.topLeft(),
                                  rect.bottomLeft(),
                                  rect.bottomRight(),
                                  rect.topRight() + QPointF(0, kMargins),
                                  rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolygon(preparePolygon(br));

    auto preparePolyline = [](const QRectF &rect) {
        return QVector<QPointF> { rect.topRight() + QPointF(0, kMargins),
                                  rect.topRight() - QPointF(kMargins, -kMargins),
                                  rect.topRight() - QPointF(kMargins, 0) };
    };
    painter->drawPolyline(preparePolyline(br));

    m_textLayout->draw(painter, QPointF(0, 0));

    painter->restore();
    AADLRectGraphicsItem::paint(painter, option, widget);
}

void AADLCommentGraphicsItem::rebuildLayout()
{
    AADLRectGraphicsItem::rebuildLayout();
    updateTextLayout(m_text);
}

QSizeF AADLCommentGraphicsItem::minimalSize() const
{
    return utils::DefaultGraphicsItemSize;
}

ColorManager::HandledColors AADLCommentGraphicsItem::handledColorType() const
{
    return ColorManager::HandledColors::Comment;
}

void AADLCommentGraphicsItem::updateTextLayout(const QString &text)
{
    static const QString ellipsis { QLatin1String("...") };
    if (text.size() < ellipsis.size())
        return;

    m_textLayout->setText(text);
    m_textLayout->setFont(font());
    m_textLayout->beginLayout();
    qreal y = kMargins;

    QTextLine line = m_textLayout->createLine();
    while (line.isValid()) {
        line.setPosition(QPointF(kMargins, y));
        line.setLineWidth(m_boundingRect.width() - 2 * kMargins);
        y += line.height();
        if (y > (m_boundingRect.height() - 2 * kMargins)) {
            const QString sf(text.constData(), line.textStart() + line.textLength() - 2 * ellipsis.size());
            m_textLayout->endLayout();
            updateTextLayout(sf + ellipsis);
            return;
        }
        line = m_textLayout->createLine();
    }
    m_textLayout->endLayout();
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

} // namespace aadl
} // namespace taste3

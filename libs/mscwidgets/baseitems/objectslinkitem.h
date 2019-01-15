/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "interactiveobject.h"

#include <QGraphicsItem>
#include <QBrush>
#include <QPen>

namespace msc {

class ObjectsLink;
class ObjectsLinkItem : public QGraphicsObject
{
    Q_OBJECT
public:
    static QPainterPath hoverableLine(const QLineF &line);

    ObjectsLinkItem(QGraphicsItem *parent = nullptr);

    ObjectsLink *link() const;

    void setBodyBrush(const QBrush &brush);
    QBrush bodyBrush() const;
    void setBodyPen(const QPen &pen);
    QPen bodyPen() const;

    void setSourceBrush(const QBrush &brush);
    QBrush sourceBrush() const;
    void setSourcePen(const QPen &pen);
    QPen sourcePen() const;

    void setTargetBrush(const QBrush &brush);
    QBrush targetBrush() const;
    void setTargetPen(const QPen &pen);
    QPen targetPen() const;

    void setStartSignShown(bool shown);
    bool startSignShown() const;

    void setEndSignShown(bool shown);
    bool endSignShown() const;

    QPointF anchorPointSource() const;
    QPointF anchorPointTarget() const;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

Q_SIGNALS:
    void geometryChanged(const QRectF &geom);

protected:
    template<class T>
    struct PairOf {
        PairOf(const T &a, const T &b)
            : Source(a)
            , Target(b)
        {
        }

        T Source, Target;
    };

    ObjectsLink *m_link = nullptr;
    QBrush m_bodyBrush = QBrush(Qt::black);
    PairOf<QBrush> m_itemsBrush = { QBrush(Qt::black), QBrush(Qt::black) };
    QPen m_bodyPen = QPen(Qt::black);
    PairOf<QPen> m_itemsPen = { QPen(Qt::black), QPen(Qt::black) };
    PairOf<bool> m_symbolShown = { false, true };
    PairOf<QPainterPath> m_symbols = { QPainterPath(), QPainterPath() };

    virtual QPainterPath bodyPath() const = 0;
    Q_INVOKABLE virtual void buildLayout() = 0;
    virtual void drawBody(QPainter *painter);
    virtual void drawStartSign(QPainter *painter);
    virtual void drawEndSign(QPainter *painter);
    void drawPath(QPainter *painter, const QBrush &brush, const QPen &pen, const QPainterPath &path);

public Q_SLOTS:
    virtual void updateLayout();
    virtual void rebuildLayout();

protected Q_SLOTS:
    void onSourceAnchorChanged(const QPointF &scenePos);
    void onTargetAnchorChanged(const QPointF &scenePos);

private:
    bool m_dirty = false;

    template<class T>
    void updateDrawingTool(T &existent, const T &requested);
};

} // namespace msc

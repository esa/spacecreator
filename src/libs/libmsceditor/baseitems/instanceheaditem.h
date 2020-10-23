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

#pragma once

#include <QGraphicsObject>
#include <QLinearGradient>
#include <QPointer>

namespace msc {
class AadlSystemChecks;
class MscChart;
class TextItem;

class InstanceHeadItem : public QGraphicsObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameEdited)
    Q_PROPERTY(QString kind READ kind WRITE setKind NOTIFY kindEdited)

public:
    InstanceHeadItem(MscChart *chart, QGraphicsItem *parent = nullptr);

    QString name() const;
    QString kind() const;

    virtual QRectF boundingRect() const override;

    QRectF rectGeometry() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setTextboxSize(const QSizeF &size);
    QRectF textBoxSceneRect() const;
    QRectF kindSceneRect() const;

    static QSizeF defaultSize();

    void setAadlChecker(AadlSystemChecks *checker);
    void showCompleter();
    void removeCompleter();

public Q_SLOTS:
    void setName(const QString &name);
    void setKind(const QString &kind);

Q_SIGNALS:
    void nameEdited(const QString &name);
    void kindEdited(const QString &kind);
    void manualMoveRequested(const QPointF &sceneFrom, const QPointF &sceneTo);
    void manualMoveFinished(const QPointF &sceneFrom, const QPointF &sceneTo);
    void layoutUpdated();

protected Q_SLOTS:
    void updateLayout();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    static QLinearGradient createGradientForKind(const QGraphicsItem *itemKind);
    static QLinearGradient createGradientForName(const QGraphicsItem *itemName);

    TextItem *m_textItemName = nullptr;
    QGraphicsRectItem *m_rectItem = nullptr;
    TextItem *m_textItemKind = nullptr;
    QPointF m_manualMovementFrom;
    QPointF m_manualMovementTo;
    QRectF m_explicitTextBox;
    AadlSystemChecks *m_aadlChecker = nullptr;
    QPointer<MscChart> m_chart;
};

} // ns msc

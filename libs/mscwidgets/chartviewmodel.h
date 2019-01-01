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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#ifndef CHARTVIEWMODEL_H
#define CHARTVIEWMODEL_H

#include <QObject>
#include <QPointF>

#include <memory>

class QGraphicsScene;
class QGraphicsObject;

namespace msc {

class MscChart;
class ConditionItem;
class InstanceItem;
class MessageItem;
class MscCondition;
class MscInstance;
class MscMessage;

struct ChartViewModelPrivate;
class ChartViewModel : public QObject
{
    Q_OBJECT
public:
    explicit ChartViewModel(QObject *parent = nullptr);
    ~ChartViewModel();

    QGraphicsScene *graphicsScene() const;

    msc::MscChart *currentChart() const;

    void clearScene();

    msc::InstanceItem *createDefaultInstanceItem(msc::MscInstance *orphanInstance = nullptr,
                                                 const QPointF &pos = QPointF());
    bool removeInstanceItem(msc::InstanceItem *item);
    msc::MessageItem *createDefaultMessageItem(msc::MscMessage *orphanMessage = nullptr,
                                               const QPointF &pos = QPointF());
    bool removeMessageItem(msc::MessageItem *item);

    InstanceItem *itemForInstance(msc::MscInstance *instance) const;
    MessageItem *itemForMessage(MscMessage *message) const;
    ConditionItem *itemForCondition(MscCondition *condition) const;

public Q_SLOTS:
    void fillView(msc::MscChart *chart);
    void relayout();
    void rearrangeInstances();

Q_SIGNALS:
    void currentChartChagend(msc::MscChart *chart);

private:
    std::unique_ptr<ChartViewModelPrivate> const d;

    QVector<QGraphicsObject *> instanceEventItems(MscInstance *instance) const;

    void actualizeInstancesHeights() const;
    void updateStoppedInstanceHeight(InstanceItem *instanceItem) const;
    void updateCreatedInstanceHeight(InstanceItem *instanceItem) const;
};

} // namespace msc

#endif // CHARTVIEWMODEL_H

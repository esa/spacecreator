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

#include "chartviewmodel.h"
#include "instanceitem.h"
#include "messageitem.h"
#include "baseitems/common/utils.h"

#include <mscchart.h>
#include <mscinstance.h>
#include <mscmessage.h>

#include <QGraphicsScene>
#include <QVector>
#include <QPointer>
#include <QDebug>

namespace msc {

/*!
   \class  ChartViewModel is the model containing the scene graph of the currently selected/visible
   MSC chart (showing instances, messages, ...)
 */

struct ChartViewModelPrivate {
    ChartViewModelPrivate(ChartViewModel *parent)
        : m_currentChart(new msc::MscChart(parent))
    {
    }

    QGraphicsScene m_scene;
    QVector<msc::InstanceItem *> m_instanceItems;
    QVector<msc::MessageItem *> m_messageItems;
    QPointer<msc::MscChart> m_currentChart = nullptr;
    static constexpr qreal InterMessageSpan = 40.;
    static constexpr qreal InterInstanceSpan = 100.;

    qreal instanceAxisHeight() const
    {
        static constexpr qreal oneMessageHeight = 50.;
        const qreal messagesCount = m_currentChart ? qMax(1, m_currentChart->messages().size()) : 1.;
        return messagesCount * (oneMessageHeight + InterMessageSpan);
    }
};

ChartViewModel::ChartViewModel(QObject *parent)
    : QObject(parent)
    , d(new ChartViewModelPrivate(this))
{
}

ChartViewModel::~ChartViewModel()
{
    clearScene();
}

QGraphicsScene *ChartViewModel::graphicsScene() const
{
    return &(d->m_scene);
}

MscChart *ChartViewModel::currentChart() const
{
    return d->m_currentChart;
}

void ChartViewModel::clearScene()
{
    qDeleteAll(d->m_messageItems);
    d->m_messageItems.clear();
    qDeleteAll(d->m_instanceItems);
    d->m_instanceItems.clear();
    d->m_scene.clear();
}

void ChartViewModel::fillView(MscChart *chart)
{
    if (chart == d->m_currentChart) {
        return;
    }

    d->m_currentChart = chart;

    clearScene();

    if (d->m_currentChart)
        relayout();

    Q_EMIT currentChartChagend(d->m_currentChart);
}

void ChartViewModel::relayout()
{
    const qreal axisHeight = d->instanceAxisHeight();
    double x = .0;
    qreal bottom = 0.;

    QRectF totalRect;
    for (MscInstance *instance : d->m_currentChart->instances()) {
        InstanceItem *item = itemForInstance(instance);
        if (!item) {
            item = new InstanceItem(instance);
            connect(item, &InstanceItem::needRelayout, this, &ChartViewModel::relayout);
            d->m_scene.addItem(item);
            d->m_instanceItems.append(item);
        }

        item->setKind(instance->kind());
        item->setAxisHeight(axisHeight);
        item->setX(x);
        item->buildLayout(); // messages layout calculation is based on

        x += d->InterInstanceSpan + item->boundingRect().width();

        // align instances bottoms:
        QRectF bounds = item->boundingRect().translated(item->pos());
        bounds.moveBottom(bottom);
        item->setY(bounds.top());
        bottom = bounds.bottom();

        totalRect = totalRect.united(item->boundingRect().translated(item->pos()));
    }

    qreal y(d->InterMessageSpan);
    for (MscMessage *message : d->m_currentChart->messages()) {
        InstanceItem *sourceInstance(nullptr);
        qreal instanceVertiacalOffset(0);
        if (message->sourceInstance()) {
            sourceInstance = itemForInstance(message->sourceInstance());
            instanceVertiacalOffset = sourceInstance->axis().p1().y();
        }
        InstanceItem *targetInstance(nullptr);
        if (message->targetInstance()) {
            targetInstance = itemForInstance(message->targetInstance());
            if (qFuzzyIsNull(instanceVertiacalOffset))
                instanceVertiacalOffset = targetInstance->axis().p1().y();
        }

        MessageItem *item = itemForMessage(message);
        if (!item) {
            item = new MessageItem(message);

            d->m_scene.addItem(item);
            d->m_messageItems.append(item);
        }
        item->connectObjects(sourceInstance, targetInstance, y + instanceVertiacalOffset);
        y += item->boundingRect().height() + d->InterMessageSpan;

        totalRect = totalRect.united(item->boundingRect().translated(item->pos()));
    }

    // actualize scene's rect to avoid flickering on first show:
    static constexpr qreal margin(50.);
    totalRect.adjust(-margin, -margin, margin, margin);
    d->m_scene.setSceneRect(totalRect);
}

InstanceItem *ChartViewModel::itemForInstance(msc::MscInstance *instance) const
{
    if (instance)
        for (QGraphicsItem *item : utils::toplevelItems(&d->m_scene))
            if (InstanceItem *instanceItem = dynamic_cast<InstanceItem *>(item))
                if (instanceItem->modelItem()->internalId() == instance->internalId())
                    return instanceItem;
    return nullptr;
}

msc::MessageItem *ChartViewModel::itemForMessage(msc::MscMessage *message) const
{
    if (message)
        for (QGraphicsItem *item : utils::toplevelItems(&d->m_scene))
            if (MessageItem *messageItem = dynamic_cast<MessageItem *>(item))
                if (messageItem->modelItem()->internalId() == message->internalId())
                    return messageItem;
    return nullptr;
}

InstanceItem *ChartViewModel::createDefaultInstanceItem(MscInstance *orphanInstance, const QPointF &pos)
{
    if (currentChart()) {
        if (!orphanInstance) {
            orphanInstance = new MscInstance(tr("Instance #%1").arg(currentChart()->instances().size()));
            currentChart()->addInstance(orphanInstance);
        }

        InstanceItem *instanceItem = InstanceItem::createDefaultItem(orphanInstance, pos);
        if (!qFuzzyIsNull(d->instanceAxisHeight()))
            instanceItem->setAxisHeight(d->instanceAxisHeight());
        return instanceItem;
    }
    return nullptr;
}

bool ChartViewModel::removeInstanceItem(msc::InstanceItem *item)
{
    if (item && utils::removeSceneItem(item)) {
        if (MscInstance *instance = item->modelItem()) {
            currentChart()->removeInstance(instance);
            delete instance;
        }
        delete item;

        return true;
    }

    return false;
}

msc::MessageItem *ChartViewModel::createDefaultMessageItem(msc::MscMessage *orphanMessage, const QPointF &pos)
{
    if (currentChart()) {
        if (!orphanMessage) {
            orphanMessage = new MscMessage(tr("Message #%1").arg(currentChart()->messages().size()));
            currentChart()->addMessage(orphanMessage);
        }

        return MessageItem::createDefaultItem(orphanMessage, pos);
    }
    return nullptr;
}

bool ChartViewModel::removeMessageItem(msc::MessageItem *item)
{
    if (item && utils::removeSceneItem(item)) {
        if (MscMessage *message = item->modelItem()) {
            currentChart()->removeMessage(message);
            delete message;
        }
        delete item;

        return true;
    }

    return false;
}

} // namespace msc

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
#include "mscchart.h"
#include "mscinstance.h"
#include "mscmessage.h"

#include <QGraphicsScene>
#include <QVector>

namespace msc {

/*!
   \class  ChartViewModel is the model containing the scene graph of the currently selected/visible
   MSC chart (showing instances, messages, ...)
 */

struct ChartViewModelPrivate {
    QGraphicsScene m_scene;
    QVector<msc::InstanceItem *> m_instanceItems;
    QVector<msc::MessageItem *> m_messageItems;
    msc::MscChart *m_currentChart = nullptr;
    qreal m_instanceAxisHeight = 0.;
};

ChartViewModel::ChartViewModel(QObject *parent)
    : QObject(parent)
    , d(new ChartViewModelPrivate)
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

    if (d->m_currentChart == nullptr) {
        Q_EMIT currentChartChagend(d->m_currentChart);
        return;
    }

    static qreal constexpr InterInstanceSpan(100.);
    double x = .0;
    for (MscInstance *instance : d->m_currentChart->instances()) {
        auto *item = new InstanceItem(instance);
        item->setKind(instance->kind());
        item->setX(x);

        item->buildLayout(); // messages layout calculation is based on

        d->m_scene.addItem(item);
        d->m_instanceItems.append(item);

        x += InterInstanceSpan + item->boundingRect().width();
    }

    static qreal constexpr InterMessageSpan(40);
    d->m_instanceAxisHeight = InterMessageSpan;
    for (MscMessage *message : d->m_currentChart->messages()) {
        InstanceItem *sourceInstance(nullptr);
        qreal instanceVertiacalOffset(0);
        if (message->sourceInstance()) {
            sourceInstance = instanceItem(message->sourceInstance()->name());
            instanceVertiacalOffset = sourceInstance->axis().p1().y();
        }
        InstanceItem *targetInstance(nullptr);
        if (message->targetInstance()) {
            targetInstance = instanceItem(message->targetInstance()->name());
            if (qFuzzyIsNull(instanceVertiacalOffset))
                instanceVertiacalOffset = targetInstance->axis().p1().y();
        }

        auto *item = new MessageItem(message);
        d->m_scene.addItem(item);
        const qreal y(d->m_instanceAxisHeight + instanceVertiacalOffset);
        item->connectObjects(sourceInstance, targetInstance, y);

        d->m_messageItems.append(item);
        d->m_instanceAxisHeight += item->boundingRect().height() + InterMessageSpan;
    }

    Q_EMIT currentChartChagend(d->m_currentChart);
    layoutItems();
}

void ChartViewModel::layoutItems()
{
    qreal x = 0., bottom = 0.;

    // align instances bottoms:
    for (InstanceItem *item : d->m_instanceItems) {
        QRectF bounds = item->boundingRect().translated(item->pos());

        item->setAxisHeight(d->m_instanceAxisHeight);
        x += bounds.width();

        bounds.moveBottom(bottom);
        item->setY(bounds.top());
        bottom = bounds.bottom();
    }

    // actualize scene's rect to avoid flickering on first show:
    QRectF r;
    for (QGraphicsItem *gi : d->m_scene.items()) {
        const QRectF itemRect = gi->boundingRect().translated(gi->pos());
        if (r.isEmpty())
            r = itemRect;
        else
            r = r.united(itemRect);
    }

    static constexpr qreal margin(50.);
    r.adjust(-margin, -margin, margin, margin);
    d->m_scene.setSceneRect(r);
}

InstanceItem *ChartViewModel::instanceItem(const QString &name) const
{
    if (!name.isEmpty())
        for (QGraphicsItem *item : d->m_scene.items()) {
            if (InstanceItem *instance = dynamic_cast<InstanceItem *>(item)) {
                if (instance->name() == name) {
                    return instance;
                }
            }
        }
    return nullptr;
}

} // namespace msc

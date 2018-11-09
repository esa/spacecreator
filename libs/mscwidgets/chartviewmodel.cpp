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

    double x = 100.0;
    for (MscInstance *instance : d->m_currentChart->instances()) {
        auto *item = new InstanceItem(instance);
        item->setKind(instance->kind());
        item->setX(x);
        d->m_scene.addItem(item);
        d->m_instanceItems.append(item);
        x += 100.0;
    }

    double y = 50.0;
    for (MscMessage *message : d->m_currentChart->messages()) {
        auto *item = new MessageItem(message);
        item->setY(y);

        if (message->sourceInstance() != nullptr)
            if (InstanceItem *instItem = instanceItem(message->sourceInstance()->name()))
                item->setSourceInstanceItem(instItem);

        if (message->targetInstance() != nullptr)
            if (InstanceItem *instItem = instanceItem(message->targetInstance()->name()))
                item->setTargetInstanceItem(instItem);

        d->m_scene.addItem(item);
        d->m_messageItems.append(item);
        y += 40.0;
    }

    for (InstanceItem *item : d->m_instanceItems) {
        item->setAxisHeight(y);
    }

    Q_EMIT currentChartChagend(d->m_currentChart);
    QMetaObject::invokeMethod(this, "layoutItems", Qt::QueuedConnection);
}

void ChartViewModel::layoutItems()
{
    qreal x = 0.;
    qreal bottom = 0.;
    for (InstanceItem *item : d->m_instanceItems) {
        QRectF bounds = item->boundingRect().translated(item->pos());

        item->setX(item->x() + x);
        x += bounds.width();

        bounds.moveBottom(bottom);
        item->setY(bounds.top());
        bottom = bounds.bottom();
    }
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

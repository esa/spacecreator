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

#include "mainmodel.h"

#include "mscchart.h"
#include "mscdocument.h"
#include "mscfile.h"
#include "mscinstance.h"
#include "mscmessage.h"
#include "mscmodel.h"

#include "instanceitem.h"
#include "messageitem.h"

#include <QGraphicsScene>
#include <documentitemmodel.h>

using namespace msc;

struct MainModelPrivate {
    explicit MainModelPrivate(MainModel *q)
        : m_documentItemModel(new DocumentItemModel(q))
    {
    }
    ~MainModelPrivate()
    {
        delete m_documentItemModel;
    }

    msc::MscModel *m_mscModel = nullptr;
    DocumentItemModel *m_documentItemModel = nullptr;
    QGraphicsScene m_scene;
    QVector<msc::InstanceItem *> m_instanceItems;
    QVector<msc::MessageItem *> m_messageItems;
    msc::MscChart *m_currentChart = nullptr;
    QStringList m_errorMessages;
};

MainModel::MainModel(QObject *parent)
    : QObject(parent)
    , d(new MainModelPrivate(this))
{
}

MainModel::~MainModel()
{
    clearScene();
    clearMscModel();
    delete d;
}

QGraphicsScene *MainModel::graphicsScene() const
{
    return &(d->m_scene);
}

msc::DocumentItemModel *MainModel::documentItemModel() const
{
    return d->m_documentItemModel;
}

MscChart *MainModel::currentChart() const
{
    return d->m_currentChart;
}

QStringList MainModel::errorMessages() const
{
    return d->m_errorMessages;
}

void MainModel::showFirstChart()
{
    fillView(firstChart());
}

void MainModel::fillView(msc::MscChart *chart)
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

void MainModel::layoutItems()
{
    qreal x = 0., bottom = 0.;
    for (InstanceItem *item : d->m_instanceItems) {
        QRectF bounds = item->boundingRect().translated(item->pos());

        item->setX(item->x() + x);
        x += bounds.width();

        bounds.moveBottom(bottom);
        item->setY(bounds.top());
        bottom = bounds.bottom();
    }
}

bool MainModel::loadFile(const QString &filename)
{
    clearMscModel();

    msc::MscFile file;

    try {
        d->m_errorMessages.clear();
        d->m_mscModel = file.parseFile(filename, &d->m_errorMessages);
    } catch (...) {
        //print error message
        return false;
    }

    d->m_documentItemModel->setMscModel(d->m_mscModel);

    connect(d->m_mscModel, &msc::MscModel::documentAdded, this, &MainModel::showFirstChart);
    connect(d->m_mscModel, &msc::MscModel::chartAdded, this, &MainModel::showFirstChart);
    connect(d->m_mscModel, &msc::MscModel::cleared, this, &MainModel::showFirstChart);

    showFirstChart();
    return true;
}

MscChart *MainModel::firstChart() const
{
    if (d->m_mscModel == nullptr) {
        return nullptr;
    }

    if (!d->m_mscModel->charts().empty()) {
        return d->m_mscModel->charts().at(0);
    }

    return firstChart(d->m_mscModel->documents());
}

MscChart *MainModel::firstChart(const QVector<MscDocument *> docs) const
{
    for (MscDocument *doc : docs) {
        if (!doc->charts().isEmpty()) {
            return doc->charts().at(0);
        }
        auto ret = firstChart(doc->documents());
        if (ret != nullptr) {
            return ret;
        }
    }
    return nullptr;
}

InstanceItem *MainModel::instanceItem(const QString &name) const
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

void MainModel::clearMscModel()
{
    delete d->m_mscModel;
    d->m_mscModel = nullptr;
}

void MainModel::clearScene()
{
    d->m_scene.clear();
    d->m_instanceItems.clear();
    d->m_messageItems.clear();
}

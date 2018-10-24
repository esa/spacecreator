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

void MainModel::showFirstChart()
{
    fillView(firstChart());
}

void MainModel::fillView(msc::MscChart *chart)
{
    clearScene();

    if (chart == nullptr) {
        return;
    }

    double x = 100.0;
    for (MscInstance *instance : chart->instances()) {
        auto *item = new InstanceItem(instance);
        item->setKind(instance->kind());
        item->setX(x);
        d->m_scene.addItem(item);
        d->m_instanceItems.append(item);
        x += 100.0;
    }

    double y = 50.0;
    for (MscMessage *message : chart->messages()) {
        auto *item = new MessageItem(message);
        item->setY(y);

        if (message->sourceInstance() != nullptr) {
            QString sourceName = message->sourceInstance()->name();
            InstanceItem *instItem = instanceItem(sourceName);
            if (instItem != nullptr) {
                item->setSourceInstanceItem(instItem);
            }
        }
        if (message->targetInstance() != nullptr) {
            QString targetName = message->targetInstance()->name();
            InstanceItem *instItem = instanceItem(targetName);
            if (instItem != nullptr) {
                item->setTargetInstanceItem(instItem);
            }
        }

        d->m_scene.addItem(item);
        d->m_messageItems.append(item);
        y += 40.0;
    }

    for (InstanceItem *item : d->m_instanceItems) {
        item->setAxisHeight(y);
    }
}

void MainModel::loadFile(const QString &filename)
{
    clearMscModel();

    msc::MscFile file;
    try {
        d->m_mscModel = file.parseFile(filename);
    } catch (...) {
        //print error message
        return;
    }

    d->m_documentItemModel->setMscModel(d->m_mscModel);

    connect(d->m_mscModel, &msc::MscModel::documentAdded, this, &MainModel::showFirstChart);
    connect(d->m_mscModel, &msc::MscModel::chartAdded, this, &MainModel::showFirstChart);
    connect(d->m_mscModel, &msc::MscModel::cleared, this, &MainModel::showFirstChart);

    showFirstChart();
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
    for (QGraphicsItem *item : d->m_scene.items()) {
        InstanceItem *instance = dynamic_cast<InstanceItem *>(item);
        if (instance != nullptr) {
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

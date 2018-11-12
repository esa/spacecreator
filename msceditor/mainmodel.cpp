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

#include "chartviewmodel.h"
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

    MscModel *m_mscModel = nullptr;
    ChartViewModel m_chartModel;
    DocumentItemModel *m_documentItemModel = nullptr;
    QStringList m_errorMessages;
    qreal m_instanceAxisHeight = 0.;
};

MainModel::MainModel(QObject *parent)
    : QObject(parent)
    , d(new MainModelPrivate(this))
{
}

MainModel::~MainModel()
{
    clearMscModel();
}

QGraphicsScene *MainModel::graphicsScene() const
{
    return d->m_chartModel.graphicsScene();
}

msc::DocumentItemModel *MainModel::documentItemModel() const
{
    return d->m_documentItemModel;
}

QStringList MainModel::errorMessages() const
{
    return d->m_errorMessages;
}

ChartViewModel &MainModel::chartViewModel() const
{
    return d->m_chartModel;
}

void MainModel::showFirstChart()
{
    d->m_chartModel.fillView(firstChart());
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

void MainModel::clearMscModel()
{
    delete d->m_mscModel;
    d->m_mscModel = nullptr;
    d->m_chartModel.clearScene();
}

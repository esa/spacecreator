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

#include "basetool.h"
#include "chartviewmodel.h"
#include "mscchart.h"
#include "mscentity.h"

#include <QPointer>

namespace msc {

class MscChart;

class BaseCreatorTool : public BaseTool
{
    Q_OBJECT
public:
    BaseCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent = nullptr);

public Q_SLOTS:
    void setModel(ChartViewModel *model);

Q_SIGNALS:
    void created();

protected Q_SLOTS:
    virtual void onCurrentChartChagend(msc::MscChart *);
    virtual void onModelLayoutComplete();

protected:
    QPointer<ChartViewModel> m_model = nullptr;
    QPointer<MscEntity> m_previewEntity = nullptr;
    QPointer<MscChart> m_activeChart = nullptr;
    QPointer<MscEntity> m_addedEntity = nullptr;
    QMetaObject::Connection m_modelUpdateFinishedListener = QMetaObject::Connection();
    virtual void startWaitForModelLayoutComplete(MscEntity *addedEntity);

private:
    void dropModelLayoutUpdateConnection();
};

} // ns msc

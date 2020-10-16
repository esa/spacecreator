/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "chartlayouttestbase.h"

#include "baseitems/common/coordinatesconverter.h"
#include "commands/common/commandsfactory.h"
#include "commands/common/commandsstack.h"
#include "mscdocument.h"

#include <QDebug>
#include <QGraphicsItem>
#include <QTest>

namespace msc {

void ChartLayoutTestBase::initBase()
{
    m_undoStack = std::make_unique<QUndoStack>();
    m_chartModel.reset(new ChartLayoutManager(m_undoStack.get()));
    cmd::CommandsStack::instance()->factory()->setChartLayoutManager(m_chartModel.get());
    cmd::CommandsStack::setCurrent(m_undoStack.get());
    m_view = std::make_unique<QGraphicsView>();
    m_view->setScene(m_chartModel->graphicsScene());
    m_reader = std::make_unique<MscReader>();

    CoordinatesConverter::instance()->setScene(m_chartModel->graphicsScene());
    const QPointF dpi1to1(CoordinatesConverter::Dpi1To1, CoordinatesConverter::Dpi1To1);
    CoordinatesConverter::setDPI(dpi1to1, dpi1to1); // results in cif <-> pixel as 1:1
}

void ChartLayoutTestBase::cleanupBase()
{
    m_reader.reset();
    m_view.reset();
    m_chartModel.reset();
    m_model.reset();
    m_undoStack.reset();
}

void ChartLayoutTestBase::parseMsc(const QString &mscDoc)
{
    m_model.reset(m_reader->parseText(mscDoc));

    if (m_model->charts().isEmpty()) {
        MscDocument *doc = m_model->documents().at(0);
        while (doc->charts().isEmpty()) {
            doc = doc->documents().at(0);
        }
        m_chart = doc->charts().at(0);
    } else {
        m_chart = m_model->charts().at(0);
    }
    Q_ASSERT(m_chart);
    m_chartModel->setCurrentChart(m_chart);
    waitForLayoutUpdate();
}

void ChartLayoutTestBase::waitForLayoutUpdate()
{
    QApplication::processEvents();
    QTest::qWait(2);
    QApplication::processEvents();
}

}

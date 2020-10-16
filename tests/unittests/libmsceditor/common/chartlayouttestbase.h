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

#include "chartlayoutmanager.h"
#include "mscchart.h"
#include "mscmodel.h"
#include "mscreader.h"

#include <QGraphicsView>
#include <QPointer>
#include <QUndoStack>
#include <memory>

namespace msc {

/*!
   \brief The ChartViewLayoutTestBase class is a base class for setting up chart layout related unit tests
 */
class ChartLayoutTestBase : public QObject
{
    Q_OBJECT

protected:
    void initBase();
    void cleanupBase();

    virtual void parseMsc(const QString &mscDoc);

    void waitForLayoutUpdate();

    std::unique_ptr<MscReader> m_reader;
    std::unique_ptr<ChartLayoutManager> m_chartModel;
    QPointer<msc::MscChart> m_chart;
    std::unique_ptr<MscModel> m_model;
    std::unique_ptr<QGraphicsView> m_view;
    std::unique_ptr<QUndoStack> m_undoStack;
};

}

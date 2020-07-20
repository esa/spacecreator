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

#include "chartlayoutmanager.h"
#include "mscchart.h"
#include "mscmodel.h"
#include "mscreader.h"

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPointer>
#include <QScopedPointer>

namespace msc {

/*!
   \brief The ChartViewTestBase class is a base class for setting up chart view related integration tests
 */
class ChartViewTestBase : public QObject
{
    Q_OBJECT

protected:
    void initTestCaseBase();
    void initBase();
    void cleanupBase();

    void loadView(const QString &mscDoc);

    QPoint center(const QGraphicsItem *item) const;
    QPoint topCenter(const QGraphicsItem *item) const;
    QPoint bottomCenter(const QGraphicsItem *item) const;
    bool isInCoregion(const msc::CoregionItem *coregion, MscInstanceEvent *event) const;

    QScopedPointer<ChartLayoutManager> m_chartModel;
    QScopedPointer<QGraphicsView> m_view;
    QScopedPointer<MscReader> m_reader;
    QScopedPointer<MscModel> m_model;
    QPointer<msc::MscChart> m_chart;

    // This could be usefull during local development,
    // but fails the test in CI environment:
    const bool m_isLocalBuild = false;
};

}

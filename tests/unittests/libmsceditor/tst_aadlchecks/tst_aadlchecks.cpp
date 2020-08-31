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

#include "aadlchecks.h"
#include "aadlobjectfunction.h"
#include "aadlobjectsmodel.h"
#include "baseitems/common/coordinatesconverter.h"
#include "chartitem.h"
#include "commandsstack.h"
#include "interface/interfacedocument.h"
#include "iveditorplugin.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "mscplugin.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QUndoCommand>
#include <QtTest>

using namespace msc;

class tst_AadlChecks : public QObject
{
    Q_OBJECT
public:
    tst_AadlChecks()
        : m_chartItem(nullptr, nullptr)
    {
    }

private Q_SLOTS:
    void initTestCase();

    void testCheckInstanceNames();

private:
    QGraphicsView m_view;
    msc::ChartItem m_chartItem;
    QUndoStack m_stack;
};

void tst_AadlChecks::initTestCase()
{
    auto converter = msc::CoordinatesConverter::instance();
    converter->setDPI(QPointF(109., 109.), QPointF(96., 96.));
    aadlinterface::cmd::CommandsStack::setCurrent(&m_stack);
}

void tst_AadlChecks::testCheckInstanceNames()
{
    msc::AadlChecks checker;
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> result = checker.checkInstances();
    QCOMPARE(result.size(), 0);

    msc::MSCPlugin mscPlugin;
    mscPlugin.mainModel()->initialModel();
    checker.setMscPlugin(&mscPlugin);
    result = checker.checkInstances();
    msc::MscChart *chart = mscPlugin.mainModel()->mscModel()->documents().at(0)->documents().at(0)->charts().at(0);
    QVERIFY(chart != nullptr);

    aadlinterface::IVEditorPlugin ivPlugin;
    checker.setIvPlugin(&ivPlugin);
    result = checker.checkInstances();
    QCOMPARE(result.size(), 0);

    // add instance
    auto instance = new msc::MscInstance("Dummy", chart);
    chart->addInstance(instance);
    result = checker.checkInstances();
    QCOMPARE(result.size(), 1);

    // Add function with different name
    aadlinterface::InterfaceDocument *doc = ivPlugin.document();
    aadl::AADLObjectsModel *aadlModel = doc->objectsModel();
    aadlModel->addObject(new aadl::AADLObjectFunction("init"));
    QCOMPARE(result.size(), 1);

    // set instance name to the function name
    instance->setName("init");
    result = checker.checkInstances();
    QCOMPARE(result.size(), 0);
}

QTEST_MAIN(tst_AadlChecks)

#include "tst_aadlchecks.moc"

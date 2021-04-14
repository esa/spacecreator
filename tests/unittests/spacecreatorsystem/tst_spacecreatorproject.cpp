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

#include "interface/interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "msceditorcore.h"
#include "sharedlibrary.h"
#include "spacecreatorproject.h"

#include <QGraphicsScene>
#include <QUndoCommand>
#include <QtTest>
#include <memory>

using namespace msc;

class tst_SpaceCreatorProject : public QObject
{
    Q_OBJECT
public:
    tst_SpaceCreatorProject() { }

private Q_SLOTS:
    void initTestCase();
    void init();

    void testIvCores();
    void testMscCores();

private:
    std::unique_ptr<scs::SpaceCreatorProject> m_project;
};

void tst_SpaceCreatorProject::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    ive::initIVEditor();
    shared::initSharedLibrary();
}

void tst_SpaceCreatorProject::init()
{
    m_project = std::make_unique<scs::SpaceCreatorProject>();
}

void tst_SpaceCreatorProject::testIvCores()
{
    QVERIFY(m_project->ivCore().isNull());

    const QString ivFileName = QFINDTESTDATA("/interfaceview.xml");
    QSharedPointer<ive::IVEditorCore> ivData = m_project->ivData(ivFileName);

    QVERIFY(!m_project->ivCore().isNull());
    QCOMPARE(m_project->ivCore(), ivData);
}

void tst_SpaceCreatorProject::testMscCores()
{
    QVERIFY(m_project->allMscCores().isEmpty());

    const QString mscFileName = QFINDTESTDATA("/Taste07.msc");
    QSharedPointer<msc::MSCEditorCore> mscData = m_project->mscData(mscFileName);

    QCOMPARE(m_project->allMscCores().size(), 1);
}

QTEST_MAIN(tst_SpaceCreatorProject)

#include "tst_spacecreatorproject.moc"

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

#include "baseitems/common/coordinatesconverter.h"
#include "msceditor.h"
#include "mscsystemchecks.h"
#include "sharedlibrary.h"
#include "spacecreatorproject.h"

#include <QtTest>

using namespace msc;

class tst_MscSystemChecks : public QObject
{
    Q_OBJECT
public:
    tst_MscSystemChecks() { }

private Q_SLOTS:
    void initTestCase();
    void init();

    void testInstanceExists();
    void testMessagesExists();

private:
    std::unique_ptr<scs::MscSystemChecks> m_checker;
    std::unique_ptr<scs::SpaceCreatorProject> m_project;
};

void tst_MscSystemChecks::initTestCase()
{
    QStandardPaths::setTestModeEnabled(true);
    msc::initMscEditor();
    shared::initSharedLibrary();
    auto converter = msc::CoordinatesConverter::instance();
    converter->setDPI(QPointF(109., 109.), QPointF(96., 96.));
}

void tst_MscSystemChecks::init()
{
    m_checker = std::make_unique<scs::MscSystemChecks>();
    m_project = std::make_unique<scs::SpaceCreatorProject>();
    m_checker->setStorage(m_project.get());
}

void tst_MscSystemChecks::testInstanceExists()
{
    const QString mscFileName = QFINDTESTDATA("/Taste07.msc");
    m_project->mscData(mscFileName);

    QCOMPARE(m_checker->ivFunctionUsed("foo"), false);
    QCOMPARE(m_checker->ivFunctionUsed("Function_1"), true);
}

void tst_MscSystemChecks::testMessagesExists()
{
    const QString mscFileName = QFINDTESTDATA("/Taste07.msc");
    m_project->mscData(mscFileName);

    QCOMPARE(m_checker->mscMessagesExist("foo", "bar", "nope"), false);
    QCOMPARE(m_checker->mscMessagesExist("notch", "Function_1", "Function_B"), true);
}

QTEST_MAIN(tst_MscSystemChecks)

#include "tst_mscsystemchecks.moc"

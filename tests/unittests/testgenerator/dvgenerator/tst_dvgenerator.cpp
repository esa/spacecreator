/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "dvtools.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <dvcore/dvmodel.h>
#include <dvcore/dvobject.h>
#include <harness/dvgenerator/dvgenerator.h>
#include <ivcore/ivfunction.h>
#include <libdveditor/dvexporter.h>
#include <memory>
#include <qobjectdefs.h>
#include <testgenerator/testgenerator.h>

using testgenerator::DvGenerator;

namespace tests::testgenerator {

class tst_dvgenerator final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testNominal();

private:
    const QString dvDir = "resources";
    const QString dvPath =
            QString("%1%2%3.dv.xml").arg(dvDir).arg(QDir::separator()); // for example resources/deploymentview.dv.xml
    const QString dvConfig = QString("%1%2config.xml").arg(dvDir).arg(QDir::separator());

    std::unique_ptr<dve::DVExporter> m_exporter;
};

void tst_dvgenerator::initTestCase()
{
    //
}

void tst_dvgenerator::testNominal()
{
    // generate deplyment view
    std::vector<ivm::IVFunction *> functionsToBind;
    auto generatedXml = DvGenerator::generate(functionsToBind);

    const auto generatedDvObjects = std::make_unique<QVector<dvm::DVObject *>>();
    for (const auto &obj : generatedXml->objects()) {
        generatedDvObjects->append(static_cast<dvm::DVObject *>(obj));
    }

    // read expected DeploymentView
    QFile file(dvPath.arg("deploymentview"));
    file.open(QIODevice::ReadOnly);
    QByteArray expectedXml(file.readAll());
    file.close();
    const std::unique_ptr<QVector<dvm::DVObject *>> expectedDvObjects = dvtools::getDvObjectsFromXml(expectedXml);
    QVERIFY(expectedDvObjects != nullptr);

    // TODO: compare expected and generated dvobjects
    QCOMPARE(generatedDvObjects->size(), expectedDvObjects->size());

    QFAIL("this shall happen");
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_dvgenerator)

#include "tst_dvgenerator.moc"

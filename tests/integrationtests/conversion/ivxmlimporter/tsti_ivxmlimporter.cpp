/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <QFileInfo>
#include <QObject>
#include <QtTest>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/options.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/IvXmlImporter/importer.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>

using conversion::Options;
using conversion::iv::IvOptions;
using conversion::iv::importer::IvXmlImporter;
using ivm::IVModel;

namespace conversion::iv::test {

class tsti_IvXmlImporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testValid();
};

void tsti_IvXmlImporter::testValid()
{
    Options options;
    options.add(IvOptions::inputFilepath, "interfaceview.xml");
    options.add(IvOptions::configFilepath, "config.xml");

    IvXmlImporter ivImporter;

    try {
        const auto model = ivImporter.importModel(options);
        QVERIFY(model);
        const auto *ivModel = dynamic_cast<IVModel *>(model.get());
        QVERIFY(ivModel);

        QCOMPARE(ivModel->objects().size(), 8);

        auto *pingerFunction = ivModel->getFunction("Pinger", Qt::CaseSensitive);
        QVERIFY(pingerFunction);
        QCOMPARE(pingerFunction->ris().size(), 1);
        QCOMPARE(pingerFunction->pis().size(), 1);

        auto *pongerFunction = ivModel->getFunction("Ponger", Qt::CaseSensitive);
        QVERIFY(pongerFunction);
        QCOMPARE(pongerFunction->ris().size(), 1);
        QCOMPARE(pongerFunction->pis().size(), 1);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

} // namespace conversion::iv::test

QTEST_MAIN(conversion::iv::test::tsti_IvXmlImporter)

#include "tsti_ivxmlimporter.moc"

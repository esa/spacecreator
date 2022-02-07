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

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <exception>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <ivtools.h>
#include <modelloader.h>
#include <qdebug.h>
#include <qobjectdefs.h>
#include <shared/sharedlibrary.h>
#include <stdexcept>
#include <testgenerator/testgenerator.h>

using plugincommon::IvTools;
using plugincommon::ModelLoader;
using testgenerator::IvGenerator;

namespace tests::testgenerator {

class tst_ivgenerator final : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testFail();

private:
    const QString interfaceviewFilepath = "resources/interfaceview.xml";
    const QString ivConfig = "resources/config.xml";
    const QString interfaceUnderTestName = "InterfaceUnderTest";
    const QString functionUnderTestName = "FunctionUnderTest";
    const QString functionUnderTestLanguage = "C";
};

void tst_ivgenerator::initTestCase()
{
    shared::initSharedLibrary();
}

void tst_ivgenerator::testFail()
{
    const auto ivModelGenerated =
            IvGenerator::generate(interfaceUnderTestName, functionUnderTestName, functionUnderTestLanguage);

    if (ivModelGenerated == nullptr) {
        QFAIL("IV model was not generated");
    }

    const auto ivModelLoadedRaw = ModelLoader::loadIvModel(ivConfig, interfaceviewFilepath);
    const auto ivModelLoaded = dynamic_cast<ivm::IVModel *>(ivModelLoadedRaw.get());
    if (ivModelLoaded == nullptr) {
        throw std::runtime_error(QString("%1 file could not be read as IV").arg(interfaceviewFilepath).toStdString());
    }

    // compare ivModels
    const std::vector<ivm::IVFunction *> loadedFunctions = IvTools::getFunctions(ivModelLoaded);
    const std::vector<ivm::IVFunction *> generatedFunctions = IvTools::getFunctions(ivModelGenerated.get());
    (void)generatedFunctions;
    QCOMPARE(generatedFunctions.size(), loadedFunctions.size());
    qDebug() << "loaded:";
    for (const auto &function : loadedFunctions) {
        qDebug() << function->title();
    }
    qDebug() << "generated:";
    for (const auto &function : generatedFunctions) {
        qDebug() << function->title();
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_ivgenerator)

#include "tst_ivgenerator.moc"

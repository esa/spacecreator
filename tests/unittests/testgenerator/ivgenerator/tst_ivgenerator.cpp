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

#include "ivinterface.h"

#include <QDebug>
#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <exception>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <ivtools.h>
#include <memory>
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
    void testNominal();

private:
    const QString interfaceviewFilepath = "resources/interfaceview.xml";
    const QString ivConfig = "resources/config.xml";
    const QString interfaceUnderTestName = "InterfaceUnderTest";
    const QString functionUnderTestName = "FunctionUnderTest";
};

static void compareModels(ivm::IVModel *loadedModel, ivm::IVModel *generatedModel);
static void compareFunctions(ivm::IVFunction *loadedFunction, ivm::IVFunction *generatedFunction);
static void compareInterfaces(ivm::IVInterface *loadedInterface, ivm::IVInterface *generatedInterface);

template<typename T>
QVector<int> createQVectorToQVectorMap(T source, T destination);

void tst_ivgenerator::initTestCase()
{
    shared::initSharedLibrary();
}

void tst_ivgenerator::testNominal()
{
    const auto functionUnderTest = std::make_unique<ivm::IVFunction>();
    functionUnderTest->setTitle(functionUnderTestName);

    ivm::IVInterface::CreationInfo ci;
    ci.name = interfaceUnderTestName;
    ci.kind = ivm::IVInterface::OperationKind::Protected;
    ci.type = ivm::IVInterface::InterfaceType::Provided;
    ci.function = functionUnderTest.get();
    ivm::IVInterface *const interfaceUnderTest = ivm::IVInterface::createIface(ci);

    const auto ivModelGenerated = IvGenerator::generate(interfaceUnderTest);

    if (ivModelGenerated == nullptr) {
        QFAIL("IV model was not generated");
    }

    const auto ivModelLoadedRaw = ModelLoader::loadIvModel(ivConfig, interfaceviewFilepath);
    const auto ivModelLoaded = dynamic_cast<ivm::IVModel *>(ivModelLoadedRaw.get());
    if (ivModelLoaded == nullptr) {
        throw std::runtime_error(QString("%1 file could not be read as IV").arg(interfaceviewFilepath).toStdString());
    }

    compareModels(ivModelLoaded, ivModelGenerated.get());
}

static void compareModels(ivm::IVModel *const loadedModel, ivm::IVModel *const generatedModel)
{
    const QVector<ivm::IVFunction *> loadedFunctions =
            QVector<ivm::IVFunction *>::fromStdVector(IvTools::getFunctions(loadedModel));
    const int loadedFunctionsSize = loadedFunctions.size();

    const QVector<ivm::IVFunction *> generatedFunctions =
            QVector<ivm::IVFunction *>::fromStdVector(IvTools::getFunctions(generatedModel));
    const int generatedFunctionsSize = generatedFunctions.size();
    QCOMPARE(generatedFunctionsSize, loadedFunctionsSize);

    QVector<int> loadedToGeneratedFunctionMap = createQVectorToQVectorMap(loadedFunctions, generatedFunctions);
    if (std::any_of(loadedToGeneratedFunctionMap.begin(), loadedToGeneratedFunctionMap.end(),
                [&loadedFunctionsSize](const auto &el) { return el == loadedFunctionsSize; })) {
        QFAIL(QString("Function not found in generated model").toStdString().c_str());
    }

    for (int i = 0; i < generatedFunctionsSize; i++) {
        const auto &loadedFunction = loadedFunctions.at(loadedToGeneratedFunctionMap.at(i));
        const auto &generatedFunction = generatedFunctions.at(i);

        compareFunctions(loadedFunction, generatedFunction);
    }
}

static void compareFunctions(ivm::IVFunction *const loadedFunction, ivm::IVFunction *const generatedFunction)
{
    const auto &loadedInterfaces = loadedFunction->interfaces();
    const auto &generatedInterfaces = generatedFunction->interfaces();

    const int loadedInterfacesSize = loadedInterfaces.size();
    const int generatedInterfacesSize = generatedInterfaces.size();
    QCOMPARE(generatedInterfacesSize, loadedInterfacesSize);

    QVector<int> loadedToGeneratedInterfaceMap = createQVectorToQVectorMap(loadedInterfaces, generatedInterfaces);

    for (int j = 0; j < generatedInterfacesSize; j++) {
        const auto &generatedInterface = generatedInterfaces.at(j);
        const auto &loadedInterface = loadedInterfaces.at(loadedToGeneratedInterfaceMap.at(j));

        compareInterfaces(loadedInterface, generatedInterface);
    }
}

static void compareInterfaces(ivm::IVInterface *loadedInterface, ivm::IVInterface *generatedInterface)
{
    QCOMPARE(generatedInterface->title(), loadedInterface->title());
}

// T could be at least QVector<IVInterface*> or QVector<IVFunction*>
template<typename T>
QVector<int> createQVectorToQVectorMap(const T source, const T destination)
{
    const int sourceSize = source.size();
    const int destinationSize = destination.size();
    assert(sourceSize == destinationSize);

    QVector<int> map(sourceSize, sourceSize);

    for (int j = 0; j < destinationSize; j++) {
        const auto &dst = destination.at(j);
        for (int k = 0; k < sourceSize; k++) {
            const auto &src = destination.at(k);
            if (src->title().compare(dst->title()) == 0) {
                map[j] = k;
            }
        }
    }

    return map;
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_ivgenerator)

#include "tst_ivgenerator.moc"

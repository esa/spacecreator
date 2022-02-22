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

#include "../common.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <dvcore/dvcommonprops.h>
#include <exception>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <ivtools.h>
#include <memory>
#include <modelloader.h>
#include <qobjectdefs.h>
#include <shared/parameter.h>
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
    const QString ivDir = "resources";
    const QString ivPath =
            QString("%1%2%3.xml").arg(ivDir).arg(QDir::separator()); // for example directory/interfaceview.xml
    const QString ivConfig = QString("%1%2config.xml").arg(ivDir).arg(QDir::separator());
};

static auto makeFunctionUnderTest(const QString &name) -> std::unique_ptr<ivm::IVFunction>;
static auto makeInterfaceUnderTest(const QString &interfaceName, ivm::IVFunction *function,
        ivm::IVInterface::OperationKind operationKind) -> ivm::IVInterface *;
static auto createInterfaceUnderTestCreationInfo(const QString &ifName, ivm::IVFunction *function,
        ivm::IVInterface::OperationKind kind) -> ivm::IVInterface::CreationInfo;

static auto compareModels(ivm::IVModel *loaded, ivm::IVModel *generated) -> void;
static auto compareFunctions(ivm::IVFunction *loaded, ivm::IVFunction *generated) -> void;
static auto compareInterfaces(ivm::IVInterface *loaded, ivm::IVInterface *generated) -> void;
static auto compareParameters(const shared::InterfaceParameter &loaded, const shared::InterfaceParameter &generated)
        -> void;
static auto compareConnectionVectors(
        const QVector<ivm::IVConnection *> &loaded, const QVector<ivm::IVConnection *> &generated) -> void;
static auto compareConnections(ivm::IVConnection *loaded, ivm::IVConnection *generated) -> void;

static auto checkEntityAttributesEqual(ivm::IVObject *expected, ivm::IVObject *actual) -> void;
static auto checkAllElementsMapped(const QVector<int> &map, int size) -> void;

static auto isAnyElementEqual(const QVector<int> &vector, int number) -> bool;

void tst_ivgenerator::initTestCase()
{
    shared::initSharedLibrary();
}

void tst_ivgenerator::testNominal()
{
    const auto functionUnderTest = makeFunctionUnderTest("FunctionUnderTest");

    ivm::IVInterface *const interfaceUnderTest = makeInterfaceUnderTest(
            "InterfaceUnderTest", functionUnderTest.get(), ivm::IVInterface::OperationKind::Protected);

    functionUnderTest->addChild(interfaceUnderTest);

    const auto ivModelGenerated = IvGenerator::generate(interfaceUnderTest);

    if (ivModelGenerated == nullptr) {
        QFAIL("IV model was not generated");
    }

    const auto ivModelLoadedUniquePtr = ModelLoader::loadIvModel(ivConfig, ivPath.arg("interfaceview"));
    const auto ivModelLoadedRaw = dynamic_cast<ivm::IVModel *>(ivModelLoadedUniquePtr.get());
    if (ivModelLoadedRaw == nullptr) {
        throw std::runtime_error(QString("%1 file could not be read as IV").arg(ivDir).toStdString());
    }

    compareModels(ivModelLoadedRaw, ivModelGenerated.get());
}

static ivm::IVInterface *makeInterfaceUnderTest(const QString &interfaceName, ivm::IVFunction *const function,
        const ivm::IVInterface::OperationKind operationKind)
{
    ivm::IVInterface::CreationInfo ci = createInterfaceUnderTestCreationInfo(interfaceName, function, operationKind);

    QVector<shared::InterfaceParameter> parameters = {
        shared::InterfaceParameter("p1", shared::BasicParameter::Type::Other, "MyInteger", "ACN",
                shared::InterfaceParameter::Direction::IN),
        shared::InterfaceParameter("p2", shared::BasicParameter::Type::Other, "MyInteger", "ACN",
                shared::InterfaceParameter::Direction::OUT),
    };
    ci.parameters = std::move(parameters);

    ivm::IVInterface *const iface = ivm::IVInterface::createIface(ci);

    iface->setEntityAttribute("wcet", "0");
    const auto layerToken = ivm::meta::Props::token(ivm::meta::Props::Token::layer);
    iface->setEntityAttribute(layerToken, "default");
    const auto autonamedToken = ivm::meta::Props::token(ivm::meta::Props::Token::Autonamed);
    iface->setEntityAttribute(autonamedToken, "true");

    return iface;
}

static std::unique_ptr<ivm::IVFunction> makeFunctionUnderTest(const QString &name)
{
    auto function = std::make_unique<ivm::IVFunction>();
    function->setTitle(name);
    const auto languageToken = ivm::meta::Props::token(ivm::meta::Props::Token::language);
    function->setEntityAttribute(languageToken, "SDL");
    const auto isTypeToken = ivm::meta::Props::token(ivm::meta::Props::Token::is_type);
    function->setEntityAttribute(isTypeToken, "NO");

    return function;
}

static ivm::IVInterface::CreationInfo createInterfaceUnderTestCreationInfo(
        const QString &ifName, ivm::IVFunction *const function, ivm::IVInterface::OperationKind kind)
{
    ivm::IVInterface::CreationInfo ci;
    ci.name = ifName;
    ci.kind = kind;
    ci.type = ivm::IVInterface::InterfaceType::Provided;
    ci.function = function;

    return ci;
}

static void compareModels(ivm::IVModel *const loaded, ivm::IVModel *const generated)
{
    const QVector<ivm::IVFunction *> loadedFunctions =
            QVector<ivm::IVFunction *>::fromStdVector(IvTools::getFunctions(loaded));

    const QVector<ivm::IVFunction *> generatedFunctions =
            QVector<ivm::IVFunction *>::fromStdVector(IvTools::getFunctions(generated));

    const int loadedFunctionsSize = loadedFunctions.size();
    const int generatedFunctionsSize = generatedFunctions.size();
    QCOMPARE(generatedFunctionsSize, loadedFunctionsSize);

    const QVector<int> loadedToGeneratedFunctionMap =
            createQVectorToQVectorMap<ivm::IVFunction *>(loadedFunctions, generatedFunctions, elementsEqualByTitle);
    checkAllElementsMapped(loadedToGeneratedFunctionMap, loadedFunctionsSize);

    for (int i = 0; i < generatedFunctionsSize; i++) {
        const auto &loadedFunction = loadedFunctions.at(loadedToGeneratedFunctionMap.at(i));
        const auto &generatedFunction = generatedFunctions.at(i);

        compareFunctions(loadedFunction, generatedFunction);
    }

    for (int i = 0; i < generatedFunctionsSize; i++) {
        const auto &loadedFunction = loadedFunctions.at(loadedToGeneratedFunctionMap.at(i));
        const auto &generatedFunction = generatedFunctions.at(i);

        const auto &loadedConnections = loaded->getConnectionsForFunction(loadedFunction->id());
        const auto &generatedConnections = generated->getConnectionsForFunction(generatedFunction->id());

        compareConnectionVectors(loadedConnections, generatedConnections);
    }
}

static void compareFunctions(ivm::IVFunction *const loaded, ivm::IVFunction *const generated)
{
    for (const auto &entityAttribute : loaded->entityAttributes()) {
        const auto &generatedValue = generated->entityAttributeValue(entityAttribute.name()).toString();
        const auto &loadedValue = entityAttribute.value().toString();

        QCOMPARE(generatedValue, loadedValue);
    }
    QCOMPARE(generated->defaultImplementation(), loaded->defaultImplementation());

    const auto &loadedInterfaces = loaded->interfaces();
    const auto &generatedInterfaces = generated->interfaces();

    const int loadedInterfacesSize = loadedInterfaces.size();
    const int generatedInterfacesSize = generatedInterfaces.size();
    QCOMPARE(generatedInterfacesSize, loadedInterfacesSize);

    const QVector<int> loadedToGeneratedInterfaceMap =
            createQVectorToQVectorMap<ivm::IVInterface *>(loadedInterfaces, generatedInterfaces, elementsEqualByTitle);
    checkAllElementsMapped(loadedToGeneratedInterfaceMap, loadedInterfacesSize);

    for (int i = 0; i < generatedInterfacesSize; i++) {
        const auto &generatedInterface = generatedInterfaces.at(i);
        const auto &loadedInterface = loadedInterfaces.at(loadedToGeneratedInterfaceMap.at(i));

        compareInterfaces(loadedInterface, generatedInterface);
    }
}

static void compareInterfaces(ivm::IVInterface *const loaded, ivm::IVInterface *const generated)
{
    QCOMPARE(generated->title(), loaded->title());
    QCOMPARE(generated->kind(), loaded->kind());
    QCOMPARE(generated->type(), loaded->type());

    checkEntityAttributesEqual(loaded, generated);

    const auto &loadedParams = loaded->params();
    const auto &generatedParams = generated->params();

    const int generatedParametersSize = generatedParams.size();
    const int loadedParametersSize = loadedParams.size();
    QCOMPARE(generatedParametersSize, loadedParametersSize);

    const QVector<int> loadedToGeneratedParameterMap =
            createQVectorToQVectorMap<shared::InterfaceParameter>(loadedParams, generatedParams, elementsEqualByName);
    checkAllElementsMapped(loadedToGeneratedParameterMap, loadedParametersSize);

    const int generatedParamsSize = generatedParams.size();
    const int loadedParamsSize = loadedParams.size();
    QCOMPARE(generatedParamsSize, loadedParamsSize);

    for (int i = 0; i < loadedParamsSize; i++) {
        const auto &generatedParam = generatedParams.at(i);
        const auto &loadedParam = loadedParams.at(loadedToGeneratedParameterMap.at(i));

        compareParameters(loadedParam, generatedParam);
    }
}

static void compareParameters(const shared::InterfaceParameter &loaded, const shared::InterfaceParameter &generated)
{
    QCOMPARE(generated.paramType(), loaded.paramType());
    QCOMPARE(generated.paramTypeName(), loaded.paramTypeName());
    QCOMPARE(generated.direction(), loaded.direction());
    QCOMPARE(generated.encoding(), loaded.encoding());
    QCOMPARE(generated.name(), loaded.name());
}

static void compareConnectionVectors(
        const QVector<ivm::IVConnection *> &loadedConnections, const QVector<ivm::IVConnection *> &generatedConnections)
{
    const auto &loadedConnectionsSize = loadedConnections.size();
    const auto &generatedConnectionsSize = generatedConnections.size();

    QCOMPARE(generatedConnectionsSize, loadedConnectionsSize);

    for (int j = 0; j < loadedConnectionsSize; j++) {
        const auto &loadedConnection = loadedConnections.at(j);
        const auto &generatedConnection = generatedConnections.at(j);

        compareConnections(loadedConnection, generatedConnection);
    }
}

static void compareConnections(ivm::IVConnection *const loaded, ivm::IVConnection *const generated)
{
    checkEntityAttributesEqual(loaded, generated);
}

static void checkEntityAttributesEqual(ivm::IVObject *const expected, ivm::IVObject *const actual)
{
    for (const auto &entityAttribute : expected->entityAttributes()) {
        const auto &expectedValue = entityAttribute.value().toString();
        const auto &actualValue = actual->entityAttributeValue(entityAttribute.name()).toString();

        QCOMPARE(actualValue, expectedValue);
    }
}

static void checkAllElementsMapped(const QVector<int> &map, int size)
{
    if (isAnyElementEqual(map, size)) {
        QFAIL(QString("Element not found in generated entity").toStdString().c_str());
    }
}

static bool isAnyElementEqual(const QVector<int> &vector, int number)
{
    return std::any_of(vector.begin(), vector.end(), [&number](const auto &el) { return el == number; });
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_ivgenerator)

#include "tst_ivgenerator.moc"

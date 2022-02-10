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

static std::unique_ptr<ivm::IVFunction> makeFunctionUnderTest(const QString &name);
static ivm::IVInterface *makeInterfaceUnderTest(
        const QString &interfaceName, ivm::IVFunction *function, ivm::IVInterface::OperationKind operationKind);
static ivm::IVInterface::CreationInfo createInterfaceUnderTestCreationInfo(
        const QString &ifName, ivm::IVFunction *function, ivm::IVInterface::OperationKind kind);

static void compareModels(ivm::IVModel *loaded, ivm::IVModel *generated);
static void compareFunctions(ivm::IVFunction *loaded, ivm::IVFunction *generated);
static void compareInterfaces(ivm::IVInterface *loaded, ivm::IVInterface *generated);
static void compareParameters(const shared::InterfaceParameter &loaded, const shared::InterfaceParameter &generated);
static void compareConnectionVectors(
        const QVector<ivm::IVConnection *> &loaded, const QVector<ivm::IVConnection *> &generated);
static void compareConnections(ivm::IVConnection *loaded, ivm::IVConnection *generated);

static void checkEntityAttributesEqual(ivm::IVObject *expected, ivm::IVObject *actual);
static void checkAnyOfElementsIsSize(QVector<int> map, int size);

static auto elementsEqualByTitle = [](const auto &srcVector, int i, const auto &dstVector, int j) -> bool {
    return srcVector.at(i)->title().compare(dstVector.at(j)->title()) == 0;
};

static auto elementsEqualByName = [](const auto &srcVector, int i, const auto &dstVector, int j) -> bool {
    return srcVector.at(i).name().compare(dstVector.at(j).name()) == 0;
};

template<typename T>
QVector<int> createQVectorToQVectorMap(const T &source, const T &destination,
        std::function<bool(const T &source, int i, const T &destination, int j)> elementsEqual);

template<typename entityType, typename containedType, typename elementType>
void compareEntities(entityType loaded, entityType generated,
        std::function<void(entityType, entityType)> additionalChecks,
        std::function<containedType(entityType)> accessElements,
        std::function<void(elementType, elementType)> checkElements,
        std::function<bool(const containedType &, int, const containedType &, int)> elementsEqualBy);

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

    const auto ivModelLoadedRaw = ModelLoader::loadIvModel(ivConfig, ivPath.arg("interfaceview"));
    const auto ivModelLoaded = dynamic_cast<ivm::IVModel *>(ivModelLoadedRaw.get());
    if (ivModelLoaded == nullptr) {
        throw std::runtime_error(QString("%1 file could not be read as IV").arg(ivDir).toStdString());
    }

    compareModels(ivModelLoaded, ivModelGenerated.get());
}

static ivm::IVInterface *makeInterfaceUnderTest(const QString &interfaceName, ivm::IVFunction *const function,
        const ivm::IVInterface::OperationKind operationKind)
{
    const ivm::IVInterface::CreationInfo ci =
            createInterfaceUnderTestCreationInfo(interfaceName, function, operationKind);
    ivm::IVInterface *const iface = ivm::IVInterface::createIface(ci);
    iface->setEntityAttribute("wcet", "0");
    iface->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::layer), "default");
    iface->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::Autonamed), "true");

    return iface;
}

static std::unique_ptr<ivm::IVFunction> makeFunctionUnderTest(const QString &name)
{
    auto function = std::make_unique<ivm::IVFunction>();
    function->setTitle(name);
    function->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::language), "SDL");
    function->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::is_type), "NO");

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

    const QVector<int> loadedToGeneratedFunctionMap = createQVectorToQVectorMap<QVector<ivm::IVFunction *>>(
            loadedFunctions, generatedFunctions, elementsEqualByTitle);
    // if yes, vectors do differ in at least one element
    checkAnyOfElementsIsSize(loadedToGeneratedFunctionMap, loadedFunctionsSize);

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
    const auto additionalChecks = [](ivm::IVFunction *const expected, ivm::IVFunction *const actual) {
        QCOMPARE(actual->defaultImplementation(), expected->defaultImplementation());
    };
    const auto accessElements = [](ivm::IVFunction *const function) { return function->interfaces(); };

    compareEntities<ivm::IVFunction *, QVector<ivm::IVInterface *>, ivm::IVInterface *>(
            loaded, generated, additionalChecks, accessElements, compareInterfaces, elementsEqualByTitle);
}

static void compareInterfaces(ivm::IVInterface *const loaded, ivm::IVInterface *const generated)
{
    const auto additionalChecks = [](ivm::IVInterface *const expected, ivm::IVInterface *const actual) {
        QCOMPARE(expected->title(), actual->title());
        QCOMPARE(expected->kind(), actual->kind());
        QCOMPARE(expected->type(), actual->type());
    };
    const auto accessElements = [](ivm::IVInterface *const interface) { return interface->params(); };

    checkEntityAttributesEqual(loaded, generated);

    compareEntities<ivm::IVInterface *, QVector<shared::InterfaceParameter>, shared::InterfaceParameter>(
            loaded, generated, additionalChecks, accessElements, compareParameters, elementsEqualByName);
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

static void checkAnyOfElementsIsSize(QVector<int> map, int size)
{
    if (std::any_of(map.begin(), map.end(), [&size](const auto &el) { return el == size; })) {
        QFAIL(QString("Element not found in generated entity").toStdString().c_str());
    }
}

template<typename QVectorT>
QVector<int> createQVectorToQVectorMap(const QVectorT &source, const QVectorT &destination,
        std::function<bool(const QVectorT &source, int i, const QVectorT &destination, int j)> elementsEqual)
{
    assert(source.size() == destination.size());

    const int size = source.size();

    QVector<int> map(size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (elementsEqual(source, i, destination, j)) {
                map[i] = j;
            }
        }
    }

    return map;
}

template<typename entityType, typename containedType, typename elementType>
void compareEntities(entityType loaded, entityType generated,
        std::function<void(entityType, entityType)> additionalChecks,
        std::function<containedType(entityType)> accessElements,
        std::function<void(elementType, elementType)> checkElements,
        std::function<bool(const containedType &, int, const containedType &, int)> elementsEqualBy)
{
    additionalChecks(loaded, generated);

    checkEntityAttributesEqual(loaded, generated);

    const auto &loadedElements = accessElements(loaded);
    const auto &generatedElements = accessElements(generated);

    const int loadedElementsSize = loadedElements.size();
    const int generatedElementsSize = generatedElements.size();
    QCOMPARE(generatedElementsSize, loadedElementsSize);

    const QVector<int> loadedToGeneratedElementsMap =
            createQVectorToQVectorMap<containedType>(loadedElements, generatedElements, elementsEqualBy);
    checkAnyOfElementsIsSize(loadedToGeneratedElementsMap, loadedElementsSize);

    for (int i = 0; i < generatedElementsSize; i++) {
        const auto &generatedElement = generatedElements.at(i);
        const auto &loadedElement = loadedElements.at(loadedToGeneratedElementsMap.at(i));

        checkElements(loadedElement, generatedElement);
    }
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_ivgenerator)

#include "tst_ivgenerator.moc"

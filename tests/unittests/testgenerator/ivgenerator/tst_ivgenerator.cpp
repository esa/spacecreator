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

#include "conversion/iv/IvOptions/options.h"
#include "conversion/iv/IvXmlExporter/exporter.h"
#include "options.h"

#include <QObject>
#include <QTest>
#include <QtTest/qtestcase.h>
#include <algorithm>
#include <dvcore/dvcommonprops.h>
#include <exception>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <ivtools.h>
#include <libiveditor/iveditor.h>
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
    const QString ivPath = QString("%1%2%3.xml").arg(ivDir).arg(QDir::separator());
    const QString ivConfig = QString("%1%2config.xml").arg(ivDir).arg(QDir::separator());
};

static std::unique_ptr<ivm::IVFunction> makeFunctionUnderTest(const QString &name);
static ivm::IVInterface::CreationInfo createInterfaceUnderTestCreationInfo(
        const QString &ifName, ivm::IVFunction *function, ivm::IVInterface::OperationKind kind);

static void compareModels(ivm::IVModel *loaded, ivm::IVModel *generated);
static void compareFunctions(ivm::IVFunction *loaded, ivm::IVFunction *generated);
static void compareInterfaces(ivm::IVInterface *loaded, ivm::IVInterface *generated);
static void compareParameters(const shared::InterfaceParameter &loaded, const shared::InterfaceParameter &generated);

template<typename T>
QVector<int> createQVectorToQVectorMapByTitle(T source, T destination);
QVector<int> createQVectorToQVectorMapByName(
        const QVector<shared::InterfaceParameter> &source, const QVector<shared::InterfaceParameter> &destination);

void tst_ivgenerator::initTestCase()
{
    shared::initSharedLibrary();
    ivm::initIVLibrary();
    ive::initIVEditor();
}

void tst_ivgenerator::testNominal()
{
    const auto functionUnderTest = makeFunctionUnderTest("FunctionUnderTest");

    const ivm::IVInterface::CreationInfo ci = createInterfaceUnderTestCreationInfo(
            "InterfaceUnderTest", functionUnderTest.get(), ivm::IVInterface::OperationKind::Protected);
    ivm::IVInterface *const interfaceUnderTest = ivm::IVInterface::createIface(ci);
    interfaceUnderTest->setEntityAttribute("wcet", "0");
    interfaceUnderTest->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::layer), "default");
    interfaceUnderTest->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::Autonamed), "true");
    // <Property name="Taste::coordinates" value="12300 18400"/>
    functionUnderTest->addChild(interfaceUnderTest);

    const auto ivModelGenerated = IvGenerator::generate(interfaceUnderTest);
    ivModelGenerated->setProperty(
            dvm::meta::Props::token(dvm::meta::Props::Token::asn1file).toStdString().c_str(), "testDriver.acn");
    qDebug() << ivModelGenerated
                        ->property(dvm::meta::Props::token(dvm::meta::Props::Token::asn1file).toStdString().c_str())
                        .toString();

    if (ivModelGenerated == nullptr) {
        QFAIL("IV model was not generated");
    }

    const auto ivModelLoadedRaw = ModelLoader::loadIvModel(ivConfig, ivPath.arg("interfaceview"));
    const auto ivModelLoaded = dynamic_cast<ivm::IVModel *>(ivModelLoadedRaw.get());
    if (ivModelLoaded == nullptr) {
        throw std::runtime_error(QString("%1 file could not be read as IV").arg(ivDir).toStdString());
    }

    conversion::iv::exporter::IvXmlExporter exporter;
    conversion::Options options;
    options.add(conversion::iv::IvOptions::outputFilepath, "out_iv.xml");

    exporter.exportModel(ivModelGenerated.get(), options);

    compareModels(ivModelLoaded, ivModelGenerated.get());
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
    const int loadedFunctionsSize = loadedFunctions.size();

    const QVector<ivm::IVFunction *> generatedFunctions =
            QVector<ivm::IVFunction *>::fromStdVector(IvTools::getFunctions(generated));
    const int generatedFunctionsSize = generatedFunctions.size();
    QCOMPARE(generatedFunctionsSize, loadedFunctionsSize);

    QVector<int> loadedToGeneratedFunctionMap = createQVectorToQVectorMapByTitle(loadedFunctions, generatedFunctions);
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

static void compareFunctions(ivm::IVFunction *const loaded, ivm::IVFunction *const generated)
{
    for (const auto &entityAttribute : loaded->entityAttributes()) {
        QCOMPARE(
                generated->entityAttributeValue(entityAttribute.name()).toString(), entityAttribute.value().toString());
    }
    QCOMPARE(generated->defaultImplementation(), loaded->defaultImplementation());

    const auto &loadedInterfaces = loaded->interfaces();
    const auto &generatedInterfaces = generated->interfaces();

    const int loadedInterfacesSize = loadedInterfaces.size();
    const int generatedInterfacesSize = generatedInterfaces.size();
    QCOMPARE(generatedInterfacesSize, loadedInterfacesSize);

    QVector<int> loadedToGeneratedInterfaceMap =
            createQVectorToQVectorMapByTitle(loadedInterfaces, generatedInterfaces);

    if (std::any_of(loadedToGeneratedInterfaceMap.begin(), loadedToGeneratedInterfaceMap.end(),
                [&loadedInterfacesSize](const auto &el) { return el == loadedInterfacesSize; })) {
        QFAIL(QString("Interface not found in generated function").toStdString().c_str());
    }

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

    for (const auto &entityAttribute : loaded->entityAttributes()) {
        if (generated->entityAttributeValue(entityAttribute.name())
                        .toString()
                        .compare(entityAttribute.value().toString())
                != 0) {
            qDebug() << "fun: " << generated->function()->title();
            qDebug() << "ifc: " << generated->title();
            qDebug() << "att: " << entityAttribute.name();
        }

        QCOMPARE(
                generated->entityAttributeValue(entityAttribute.name()).toString(), entityAttribute.value().toString());
    }

    const auto &loadedParams = loaded->params();
    const auto &generatedParams = generated->params();

    const int generatedParametersSize = generatedParams.size();
    const int loadedParametersSize = loadedParams.size();
    QCOMPARE(generatedParametersSize, loadedParametersSize);

    QVector<int> loadedToGeneratedParameterMap = createQVectorToQVectorMapByName(loadedParams, generatedParams);

    if (std::any_of(loadedToGeneratedParameterMap.begin(), loadedToGeneratedParameterMap.end(),
                [&loadedParametersSize](const auto &el) { return el == loadedParametersSize; })) {
        QFAIL(QString("Parameter not found in generated interface").toStdString().c_str());
    }

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

// T could be at least QVector<IVInterface*> or QVector<IVFunction*>
template<typename T>
QVector<int> createQVectorToQVectorMapByTitle(const T source, const T destination)
{
    assert(source.size() == destination.size());

    const int size = source.size();

    QVector<int> map(size, size);

    for (int j = 0; j < size; j++) {
        const auto &dst = destination.at(j);
        for (int k = 0; k < size; k++) {
            const auto &src = source.at(k);
            if (src->title().compare(dst->title()) == 0) {
                map[j] = k;
            }
        }
    }

    return map;
}

QVector<int> createQVectorToQVectorMapByName(
        const QVector<shared::InterfaceParameter> &source, const QVector<shared::InterfaceParameter> &destination)
{
    assert(source.size() == destination.size());

    const int size = source.size();

    QVector<int> map(size, size);

    for (int j = 0; j < size; j++) {
        const auto &dst = destination.at(j);
        for (int k = 0; k < size; k++) {
            const auto &src = source.at(k);
            if (src.name().compare(dst.name()) == 0) {
                map[j] = k;
            }
        }
    }

    return map;
}

} // namespace tests::testgenerator

QTEST_MAIN(tests::testgenerator::tst_ivgenerator)

#include "tst_ivgenerator.moc"

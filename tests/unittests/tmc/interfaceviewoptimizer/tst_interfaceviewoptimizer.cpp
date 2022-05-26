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

#include <QObject>
#include <QString>
#include <QtTest>
#include <ivcore/ivfunction.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <ivcore/ivxmlreader.h>
#include <memory>
#include <tmc/TmcInterfaceViewOptimizer/interfaceviewoptimizer.h>

namespace tmc::test {

class tst_InterfaceViewOptimizer : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testEnvironmentSelection();

private:
    bool hasFunction(const QString &functionName, ivm::IVModel *ivModel);
    bool hasSdlFunction(const QString &functionName, ivm::IVModel *ivModel);
    bool hasEnvFunction(const QString &functionName, ivm::IVModel *ivModel);

    std::unique_ptr<ivm::IVModel> importIvModel(const QString &filepath);

private:
    ivm::IVPropertyTemplateConfig *m_dynPropConfig;
};

void tst_InterfaceViewOptimizer::initTestCase()
{
    ivm::initIVLibrary();
    m_dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    m_dynPropConfig->init(QLatin1String("default_attributes.xml"));
}

void tst_InterfaceViewOptimizer::cleanupTestCase() {}

void tst_InterfaceViewOptimizer::testEnvironmentSelection()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("resources/environmentselection.xml");
    QVERIFY(ivModel);

    InterfaceViewOptimizer::optimizeModel(ivModel.get(), { "Pinger" });

    QVERIFY(hasSdlFunction("Ponger", ivModel.get()));
    QVERIFY(hasEnvFunction("Pinger", ivModel.get()));
    QVERIFY(!hasFunction("Clock", ivModel.get()));
}

bool tst_InterfaceViewOptimizer::hasFunction(const QString &functionName, ivm::IVModel *ivModel)
{
    auto function = ivModel->getFunction(functionName, Qt::CaseSensitive);
    return function != nullptr;
}

bool tst_InterfaceViewOptimizer::hasSdlFunction(const QString &functionName, ivm::IVModel *ivModel)
{
    auto function = ivModel->getFunction(functionName, Qt::CaseSensitive);
    if (function == nullptr) {
        return false;
    }

    const QString defaultImplementation = function->defaultImplementation();

    for (const auto &impl : function->implementations()) { // NOLINT(readability-use-anyofallof)
        if (impl.name() == defaultImplementation)
            return impl.value().toString().toLower() == "sdl";
    }

    return false;
}

bool tst_InterfaceViewOptimizer::hasEnvFunction(const QString &functionName, ivm::IVModel *ivModel)
{
    auto function = ivModel->getFunction(functionName, Qt::CaseSensitive);
    if (function == nullptr) {
        return false;
    }

    const QString defaultImplementation = function->defaultImplementation();

    for (const auto &impl : function->implementations()) { // NOLINT(readability-use-anyofallof)
        if (impl.name() == defaultImplementation)
            return impl.value().toString().toLower() != "sdl";
    }

    return false;
}

std::unique_ptr<ivm::IVModel> tst_InterfaceViewOptimizer::importIvModel(const QString &filepath)
{
    ivm::IVXMLReader reader;

    const bool ok = reader.readFile(filepath);
    if (!ok) {
        return nullptr;
    }

    auto model = std::make_unique<ivm::IVModel>(m_dynPropConfig);
    model->initFromObjects(reader.parsedObjects());
    return model;
}

} // namespace tmc::test

QTEST_MAIN(tmc::test::tst_InterfaceViewOptimizer)

#include "tst_interfaceviewoptimizer.moc"

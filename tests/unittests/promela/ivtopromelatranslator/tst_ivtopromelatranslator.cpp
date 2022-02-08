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
#include <QtTest>
#include <conversion/common/modeltype.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <ivcore/ivxmlreader.h>
#include <memory>
#include <promela/IvToPromelaTranslator/translator.h>
#include <promela/PromelaModel/proctypeelement.h>
#include <promela/PromelaModel/promelamodel.h>

using promela::model::BasicType;
using promela::model::ChannelSend;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::InlineDef;
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Utype;
using promela::translator::IvToPromelaTranslator;

namespace tmc::test {
class tst_IvToPromelaTranslator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testSimple();
    void testParameters();

private:
    std::unique_ptr<ivm::IVModel> importIvModel(const QString &filepath);
    std::unique_ptr<PromelaModel> translateIvToPromela(std::unique_ptr<ivm::IVModel> ivModel);

    const Declaration *findDeclaration(const QList<Declaration> &list, const QString &name);
    const InlineDef *findInline(const std::list<std::unique_ptr<InlineDef>> &list, const QString &name);
    const Proctype *findProctype(const std::list<std::unique_ptr<Proctype>> &list, const QString &name);

private:
    ivm::IVPropertyTemplateConfig *m_dynPropConfig;
};

void tst_IvToPromelaTranslator::initTestCase()
{
    ivm::initIVLibrary();
    m_dynPropConfig = ivm::IVPropertyTemplateConfig::instance();
    m_dynPropConfig->init(QLatin1String("default_attributes.xml"));
}

void tst_IvToPromelaTranslator::cleanupTestCase() {}

void tst_IvToPromelaTranslator::testSimple()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("parameterless.xml");
    QVERIFY(ivModel);
    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel));
    QVERIFY(promelaModel);

    QCOMPARE(promelaModel->getIncludes().size(), 4);
    QVERIFY(promelaModel->getIncludes().indexOf("dataview.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("controller.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("actuator.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("env_inlines.pml") >= 0);

    QCOMPARE(promelaModel->getUtypes().size(), 1);

    const auto &systemState = promelaModel->getUtypes().first();
    QCOMPARE(systemState.getName(), "system_state");
    QCOMPARE(systemState.isUnionType(), false);
    QCOMPARE(systemState.getFields().size(), 2);
    {
        const Declaration *controller = findDeclaration(systemState.getFields(), "controller");
        QVERIFY(controller != nullptr);
        QVERIFY(controller->getType().isUtypeReference());
        QCOMPARE(controller->getType().getUtypeReference().getName(), "Controller_Context");
        QCOMPARE(controller->getVisibility(), Declaration::Visibility::NORMAL);
    }
    {
        const Declaration *actuator = findDeclaration(systemState.getFields(), "actuator");
        QVERIFY(actuator != nullptr);
        QVERIFY(actuator->getType().isUtypeReference());
        QCOMPARE(actuator->getType().getUtypeReference().getName(), "Actuator_Context");
        QCOMPARE(actuator->getVisibility(), Declaration::Visibility::NORMAL);
    }

    QCOMPARE(promelaModel->getDeclarations().size(), 5);

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "global_state");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "system_state");
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "inited");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::INT);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "actuator_test_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "controller_success_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "controller_fail_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    QVERIFY(promelaModel->hasInit());

    QCOMPARE(promelaModel->getProctypes().size(), 3);

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "actuator_test");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "controller_success");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "controller_fail");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);
    }

    QCOMPARE(promelaModel->getInlineDefs().size(), 3);

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_test");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_RI_0_success");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_RI_0_fail");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }
}

void tst_IvToPromelaTranslator::testParameters()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("interface_params.xml");
    QVERIFY(ivModel);
    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel));
    QVERIFY(promelaModel);

    QCOMPARE(promelaModel->getIncludes().size(), 4);
    QVERIFY(promelaModel->getIncludes().indexOf("dataview.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("controller.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("actuator.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("env_inlines.pml") >= 0);

    QCOMPARE(promelaModel->getUtypes().size(), 1);

    const auto &systemState = promelaModel->getUtypes().first();
    QCOMPARE(systemState.getName(), "system_state");
    QCOMPARE(systemState.isUnionType(), false);
    QCOMPARE(systemState.getFields().size(), 2);
    {
        const Declaration *controller = findDeclaration(systemState.getFields(), "controller");
        QVERIFY(controller != nullptr);
        QVERIFY(controller->getType().isUtypeReference());
        QCOMPARE(controller->getType().getUtypeReference().getName(), "Controller_Context");
        QCOMPARE(controller->getVisibility(), Declaration::Visibility::NORMAL);
    }
    {
        const Declaration *actuator = findDeclaration(systemState.getFields(), "actuator");
        QVERIFY(actuator != nullptr);
        QVERIFY(actuator->getType().isUtypeReference());
        QCOMPARE(actuator->getType().getUtypeReference().getName(), "Actuator_Context");
        QCOMPARE(actuator->getVisibility(), Declaration::Visibility::NORMAL);
    }

    QCOMPARE(promelaModel->getDeclarations().size(), 5);

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "global_state");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "system_state");
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "inited");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::INT);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "actuator_work_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "controller_result_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "controller_error_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    QVERIFY(promelaModel->hasInit());

    QCOMPARE(promelaModel->getProctypes().size(), 3);

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "actuator_work");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "controller_result");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "controller_error");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);
    }

    QCOMPARE(promelaModel->getInlineDefs().size(), 3);

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_work");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_RI_0_result");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_RI_0_error");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }
}

std::unique_ptr<ivm::IVModel> tst_IvToPromelaTranslator::importIvModel(const QString &filepath)
{
    ivm::IVXMLReader reader;

    const bool ok = reader.readFile(filepath);
    if (ok) {
        auto model = std::make_unique<ivm::IVModel>(m_dynPropConfig);
        model->initFromObjects(reader.parsedObjects());
        return model;
    } else {
        qWarning() << "Cannot read file " << filepath;
    }

    return {};
}

std::unique_ptr<PromelaModel> tst_IvToPromelaTranslator::translateIvToPromela(std::unique_ptr<ivm::IVModel> ivModel)
{
    IvToPromelaTranslator translator;
    conversion::Options options;
    std::vector<conversion::Model *> inputs;
    inputs.push_back(ivModel.get());
    auto result = translator.translateModels(std::move(inputs), options);

    if (result.empty()) {
        return {};
    }
    if (result.front()->modelType() != conversion::ModelType::Promela) {
        return {};
    }

    auto promelaModel = std::unique_ptr<PromelaModel>(dynamic_cast<PromelaModel *>(result.front().release()));
    return promelaModel;
}

const Declaration *tst_IvToPromelaTranslator::findDeclaration(const QList<Declaration> &list, const QString &name)
{
    auto iter = std::find_if(list.begin(), list.end(), [&](const Declaration &decl) { return decl.getName() == name; });

    if (iter == list.end()) {
        return nullptr;
    }
    return &(*iter);
}

const InlineDef *tst_IvToPromelaTranslator::findInline(
        const std::list<std::unique_ptr<InlineDef>> &list, const QString &name)
{
    auto iter = std::find_if(list.begin(), list.end(),
            [&](const std::unique_ptr<InlineDef> &inlineDef) { return inlineDef->getName() == name; });

    if (iter == list.end()) {
        return nullptr;
    }
    return iter->get();
}
const Proctype *tst_IvToPromelaTranslator::findProctype(
        const std::list<std::unique_ptr<Proctype>> &list, const QString &name)
{
    auto iter = std::find_if(list.begin(), list.end(),
            [&](const std::unique_ptr<Proctype> &proctype) { return proctype->getName() == name; });

    if (iter == list.end()) {
        return nullptr;
    }
    return iter->get();
}
}
QTEST_MAIN(tmc::test::tst_IvToPromelaTranslator)

#include "tst_ivtopromelatranslator.moc"

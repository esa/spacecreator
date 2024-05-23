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
#include <asn1library/asn1/asn1model.h>
#include <conversion/common/modeltype.h>
#include <ivcore/ivlibrary.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <ivcore/ivxmlreader.h>
#include <memory>
#include <promela/IvToPromelaTranslator/translator.h>
#include <promela/PromelaModel/proctypeelement.h>
#include <promela/PromelaModel/promelamodel.h>
#include <promela/PromelaOptions/options.h>

using conversion::promela::PromelaOptions;
using promela::model::Assignment;
using promela::model::BasicType;
using promela::model::ChannelRecv;
using promela::model::ChannelSend;
using promela::model::Conditional;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::DoLoop;
using promela::model::Expression;
using promela::model::GoTo;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::Label;
using promela::model::PrintfStatement;
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Sequence;
using promela::model::Skip;
using promela::model::StringConstant;
using promela::model::Utype;
using promela::model::VariableRef;
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
    void testFunctionTypes();
    void testProctypePriority();
    void testSimpleObservers();
    void testOutputObservers();
    void testUnhandledInputObservers();
    void testSynchronousInterfaces();
    void testChannelNames();
    void testEnvironmentCallsSynchronousInterface();

private:
    template<typename T>
    const T *findProctypeElement(const Sequence &sequence, size_t index)
    {
        auto iter = sequence.getContent().begin();
        std::advance(iter, index);
        if (iter == sequence.getContent().end()) {
            return nullptr;
        }
        if (std::holds_alternative<T>((*iter)->getValue())) {
            return &(std::get<T>((*iter)->getValue()));
        }
        return nullptr;
    }

    void verifyProctypeSimple(const Proctype *proctype, const QString &functionName, const QString &interfaceName,
            size_t expectedParameters);

private:
    std::unique_ptr<ivm::IVModel> importIvModel(const QString &filepath);
    std::unique_ptr<PromelaModel> translateIvToPromela(
            std::unique_ptr<ivm::IVModel> ivModel, const conversion::Options &options);

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

void tst_IvToPromelaTranslator::cleanupTestCase() { }

void tst_IvToPromelaTranslator::testSimple()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("parameterless.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "actuator");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
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
    QCOMPARE(systemState.getFields().size(), 3);
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
    {
        const Declaration *timers = findDeclaration(systemState.getFields(), "timers");
        QVERIFY(timers != nullptr);
        QVERIFY(timers->getType().isUtypeReference());
        QCOMPARE(timers->getType().getUtypeReference().getName(), "AggregateTimerData");
        QCOMPARE(timers->getVisibility(), Declaration::Visibility::NORMAL);
    }

    QCOMPARE(promelaModel->getDeclarations().size(), 7);

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
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Actuator_test_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_success_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_fail_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_lock");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Actuator_lock");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    QVERIFY(promelaModel->hasInit());

    QCOMPARE(promelaModel->getProctypes().size(), 3);

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Actuator_test");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Actuator", "test", 0);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Controller_success");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Controller", "success", 0);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Controller_fail");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Controller", "fail", 0);
    }

    QCOMPARE(promelaModel->getInlineDefs().size(), 10);

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

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_check_queue");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Expression>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_check_queue");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Expression>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_RI_0_get_sender");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_get_sender");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_success_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_fail_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_PI_0_test_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
}

void tst_IvToPromelaTranslator::testParameters()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("interface_params.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "actuator");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
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
    QCOMPARE(systemState.getFields().size(), 3);
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
    {
        const Declaration *timers = findDeclaration(systemState.getFields(), "timers");
        QVERIFY(timers != nullptr);
        QVERIFY(timers->getType().isUtypeReference());
        QCOMPARE(timers->getType().getUtypeReference().getName(), "AggregateTimerData");
        QCOMPARE(timers->getVisibility(), Declaration::Visibility::NORMAL);
    }

    QCOMPARE(promelaModel->getDeclarations().size(), 11);

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
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Actuator_work_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_result_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_error_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_lock");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Actuator_lock");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Actuator_work_signal_parameter");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "TestParam");
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Actuator_work_channel_used");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::BOOLEAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_result_signal_parameter");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "TestInteger");
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_result_channel_used");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::BOOLEAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    QVERIFY(promelaModel->hasInit());

    QCOMPARE(promelaModel->getProctypes().size(), 3);

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Actuator_work");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Actuator", "work", 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Controller_result");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Controller", "result", 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Controller_error");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Controller", "error", 0);
    }

    QCOMPARE(promelaModel->getInlineDefs().size(), 10);

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

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_check_queue");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Expression>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_check_queue");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Expression>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_RI_0_get_sender");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_get_sender");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_result_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_error_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_PI_0_work_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
}

void tst_IvToPromelaTranslator::testFunctionTypes()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("function_types.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "up");
    options.add(PromelaOptions::modelFunctionName, "down");
    options.add(PromelaOptions::environmentFunctionName, "env");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
    QVERIFY(promelaModel);

    QCOMPARE(promelaModel->getIncludes().size(), 5);
    QVERIFY(promelaModel->getIncludes().indexOf("dataview.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("controller.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("up.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("down.pml") >= 0);
    QVERIFY(promelaModel->getIncludes().indexOf("env_inlines.pml") >= 0);

    QCOMPARE(promelaModel->getUtypes().size(), 1);

    const auto &systemState = promelaModel->getUtypes().first();
    QCOMPARE(systemState.getName(), "system_state");
    QCOMPARE(systemState.isUnionType(), false);
    QCOMPARE(systemState.getFields().size(), 4);
    {
        const Declaration *controller = findDeclaration(systemState.getFields(), "controller");
        QVERIFY(controller != nullptr);
        QVERIFY(controller->getType().isUtypeReference());
        QCOMPARE(controller->getType().getUtypeReference().getName(), "Controller_Context");
        QCOMPARE(controller->getVisibility(), Declaration::Visibility::NORMAL);
    }
    {
        const Declaration *actuator = findDeclaration(systemState.getFields(), "down");
        QVERIFY(actuator != nullptr);
        QVERIFY(actuator->getType().isUtypeReference());
        QCOMPARE(actuator->getType().getUtypeReference().getName(), "Actuator_Context");
        QCOMPARE(actuator->getVisibility(), Declaration::Visibility::NORMAL);
    }
    {
        const Declaration *actuator = findDeclaration(systemState.getFields(), "up");
        QVERIFY(actuator != nullptr);
        QVERIFY(actuator->getType().isUtypeReference());
        QCOMPARE(actuator->getType().getUtypeReference().getName(), "Actuator_Context");
        QCOMPARE(actuator->getVisibility(), Declaration::Visibility::NORMAL);
    }
    {
        const Declaration *timers = findDeclaration(systemState.getFields(), "timers");
        QVERIFY(timers != nullptr);
        QVERIFY(timers->getType().isUtypeReference());
        QCOMPARE(timers->getType().getUtypeReference().getName(), "AggregateTimerData");
        QCOMPARE(timers->getVisibility(), Declaration::Visibility::NORMAL);
    }

    QCOMPARE(promelaModel->getDeclarations().size(), 20);

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
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_test_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_test_signal_parameter");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "MyTestInteger");
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_test_channel_used");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::BOOLEAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_up_result_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_up_result_signal_parameter");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "MyInteger");
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_up_result_channel_used");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::BOOLEAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_down_result_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_down_result_signal_parameter");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "MyInteger");

        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Controller_down_result_channel_used");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::BOOLEAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Up_check_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Up_check_signal_parameter");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "MyInteger");

        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Up_check_channel_used");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::BOOLEAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Down_check_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Down_check_signal_parameter");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isUtypeReference());
        QCOMPARE(declaration->getType().getUtypeReference().getName(), "MyInteger");

        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Down_check_channel_used");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::BOOLEAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_lock");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Up_lock");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Down_lock");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    QVERIFY(promelaModel->hasInit());

    QCOMPARE(promelaModel->getProctypes().size(), 6);

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Env_test");
        QVERIFY(proctype != nullptr);
        // environment proctype
    }
    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Controller_test");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Controller", "test", 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Controller_up_result");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Controller", "up_result", 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Controller_down_result");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Controller", "down_result", 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Up_check");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Up", "check", 1);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Down_check");
        QVERIFY(proctype != nullptr);
        verifyProctypeSimple(proctype, "Down", "check", 1);
    }

    QCOMPARE(promelaModel->getInlineDefs().size(), 16);

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Env_0_RI_0_test");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_up_check");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_down_check");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Up_0_RI_0_result");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Down_0_RI_0_result");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_check_queue");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Expression>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Up_check_queue");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Expression>(content.back()->getValue()));
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Down_check_queue");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 0);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Expression>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Up_0_RI_0_get_sender");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Down_0_RI_0_get_sender");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_get_sender");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_test_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_up_result_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_down_result_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Up_0_PI_0_check_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Down_0_PI_0_check_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<Skip>(content.back()->getValue()));
    }
}

void tst_IvToPromelaTranslator::testProctypePriority()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("proctype_priority.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "Receiver");
    options.add(PromelaOptions::environmentFunctionName, "Pinger");
    options.add(PromelaOptions::processesBasePriority, "3");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
    QVERIFY(promelaModel);

    QCOMPARE(promelaModel->getProctypes().size(), 2);

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Receiver_IntegerInterface");
        QVERIFY(proctype != nullptr);
        QCOMPARE(proctype->getPriority(), 6);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Pinger_IntegerInterface");
        QVERIFY(proctype != nullptr);
        QCOMPARE(proctype->getPriority(), 1);
    }
}

void tst_IvToPromelaTranslator::testSimpleObservers()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("interface_params.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "actuator");
    options.add(PromelaOptions::observerAttachment,
            "observer:ObservedSignalKind.OUTPUT:work_in:work:<controller:>actuator");
    options.add(PromelaOptions::observerFunctionName, "observer");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
    QVERIFY(promelaModel);

    QVERIFY(promelaModel->getIncludes().indexOf("observer.pml") >= 0);

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Observer_0_RI_0_work_in");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const std::list<std::unique_ptr<ProctypeElement>> &content = inlineDef->getSequence().getContent();
        QVERIFY(std::holds_alternative<ChannelSend>(content.back()->getValue()));
    }

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Actuator_observer_work_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Actuator_work");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);

        const Sequence &main = proctype->getSequence();

        const DoLoop *mainLoop = findProctypeElement<DoLoop>(main, 1);
        QVERIFY(mainLoop);

        QVERIFY(mainLoop->getSequences().size() > 0);

        const Sequence &mainSequence = *mainLoop->getSequences().front();

        const Expression *queueCheckExpression = findProctypeElement<Expression>(mainSequence, 0);
        QVERIFY(queueCheckExpression);

        const ChannelRecv *functionLockStatement = findProctypeElement<ChannelRecv>(mainSequence, 1);
        QVERIFY(functionLockStatement);

        const Label *loopLabel = findProctypeElement<Label>(mainSequence, 2);
        QVERIFY(loopLabel);

        const Conditional *sdlProcessingBlock = findProctypeElement<Conditional>(mainSequence, 3);
        QVERIFY(sdlProcessingBlock);

        const Conditional *observerProcessingBlock = findProctypeElement<Conditional>(mainSequence, 4);
        QVERIFY(observerProcessingBlock);

        const ChannelSend *functionUnlockStatement = findProctypeElement<ChannelSend>(mainSequence, 5);
        QVERIFY(functionUnlockStatement);
    }
}

void tst_IvToPromelaTranslator::testOutputObservers()
{
    // this test check if in case of multiple output observers attached to the one interface
    // the generated code is valid, i.e.
    // First observer with higher priority is executed using message from main proctype channel
    // then observer with lower priority is executed using message from previous observer
    // finally, the process is executed using message from observer with lower priority
    // in the generated code the order is reversed, i.e.
    // first the channel from observer with lower priority is checked and eventually the process is executed
    // then the channel from observer with higher priority is checked and eventually the observer with lower priority is
    // executed in the last step, the main proctype channel is checked and eventually the observer with higher priority
    // is executed.
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("output_observers.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "actuator");
    options.add(PromelaOptions::environmentFunctionName, "environ");

    options.add(PromelaOptions::observerAttachment,
            "change_observer:ObservedSignalKind.OUTPUT:f2_in:f2:<controller:>actuator:p1");
    options.add(PromelaOptions::observerAttachment,
            "change_observer:ObservedSignalKind.OUTPUT:f1_in:f1:<controller:>actuator:p1");
    options.add(PromelaOptions::observerAttachment,
            "zero_observer:ObservedSignalKind.OUTPUT:f2_in:f2:<controller:>actuator:p2");
    options.add(PromelaOptions::observerAttachment,
            "zero_observer:ObservedSignalKind.OUTPUT:f1_in:f1:<controller:>actuator:p2");
    options.add(PromelaOptions::observerFunctionName, "Change_observer");
    options.add(PromelaOptions::observerFunctionName, "Zero_observer");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
    QVERIFY(promelaModel);

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Actuator_f1");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);

        const Sequence &main = proctype->getSequence();

        const DoLoop *mainLoop = findProctypeElement<DoLoop>(main, 1);
        QVERIFY(mainLoop);

        QVERIFY(mainLoop->getSequences().size() > 0);

        const Sequence &mainSequence = *mainLoop->getSequences().front();

        const Expression *queueCheckExpression = findProctypeElement<Expression>(mainSequence, 0);
        QVERIFY(queueCheckExpression);

        const ChannelRecv *functionLockStatement = findProctypeElement<ChannelRecv>(mainSequence, 1);
        QVERIFY(functionLockStatement);

        const Label *loopLabel = findProctypeElement<Label>(mainSequence, 2);
        QVERIFY(loopLabel);

        {
            const Conditional *sdlProcessingBlock = findProctypeElement<Conditional>(mainSequence, 3);
            QVERIFY(sdlProcessingBlock);
            QCOMPARE(sdlProcessingBlock->getAlternatives().size(), 2);
            const InlineCall *queueCheck =
                    findProctypeElement<InlineCall>(*sdlProcessingBlock->getAlternatives().front(), 0);
            QVERIFY(queueCheck);
            QCOMPARE(queueCheck->getName(), "nempty");
            QCOMPARE(queueCheck->getArguments().size(), 1);
            QVERIFY(std::holds_alternative<VariableRef>(queueCheck->getArguments().front()));
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().size(), 1);
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().front().m_name,
                    "Actuator_change_observer_f1_channel");
        }

        {
            const Conditional *changeObserverProcessingBlock = findProctypeElement<Conditional>(mainSequence, 4);
            QVERIFY(changeObserverProcessingBlock);
            QCOMPARE(changeObserverProcessingBlock->getAlternatives().size(), 2);
            const InlineCall *queueCheck =
                    findProctypeElement<InlineCall>(*changeObserverProcessingBlock->getAlternatives().front(), 0);
            QVERIFY(queueCheck);
            QCOMPARE(queueCheck->getName(), "nempty");
            QCOMPARE(queueCheck->getArguments().size(), 1);
            QVERIFY(std::holds_alternative<VariableRef>(queueCheck->getArguments().front()));
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().size(), 1);
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().front().m_name,
                    "Actuator_zero_observer_f1_channel");
        }

        {
            const Conditional *zeroObserverProcessingBlock = findProctypeElement<Conditional>(mainSequence, 5);
            QVERIFY(zeroObserverProcessingBlock);
            QCOMPARE(zeroObserverProcessingBlock->getAlternatives().size(), 2);
            const InlineCall *queueCheck =
                    findProctypeElement<InlineCall>(*zeroObserverProcessingBlock->getAlternatives().front(), 0);
            QVERIFY(queueCheck);
            QCOMPARE(queueCheck->getName(), "nempty");
            QCOMPARE(queueCheck->getArguments().size(), 1);
            QVERIFY(std::holds_alternative<VariableRef>(queueCheck->getArguments().front()));
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().size(), 1);
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().front().m_name,
                    "Actuator_f1_channel");
        }

        const ChannelSend *functionUnlockStatement = findProctypeElement<ChannelSend>(mainSequence, 6);
        QVERIFY(functionUnlockStatement);
    }

    {
        const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Actuator_f2");
        QVERIFY(proctype != nullptr);
        QVERIFY(proctype->isActive());
        QCOMPARE(proctype->getInstancesCount(), 1);

        const Sequence &main = proctype->getSequence();

        const DoLoop *mainLoop = findProctypeElement<DoLoop>(main, 1);
        QVERIFY(mainLoop);

        QVERIFY(mainLoop->getSequences().size() > 0);

        const Sequence &mainSequence = *mainLoop->getSequences().front();

        const Expression *queueCheckExpression = findProctypeElement<Expression>(mainSequence, 0);
        QVERIFY(queueCheckExpression);

        const ChannelRecv *functionLockStatement = findProctypeElement<ChannelRecv>(mainSequence, 1);
        QVERIFY(functionLockStatement);

        const Label *loopLabel = findProctypeElement<Label>(mainSequence, 2);
        QVERIFY(loopLabel);

        {
            const Conditional *sdlProcessingBlock = findProctypeElement<Conditional>(mainSequence, 3);
            QVERIFY(sdlProcessingBlock);
            QCOMPARE(sdlProcessingBlock->getAlternatives().size(), 2);
            const InlineCall *queueCheck =
                    findProctypeElement<InlineCall>(*sdlProcessingBlock->getAlternatives().front(), 0);
            QVERIFY(queueCheck);
            QCOMPARE(queueCheck->getName(), "nempty");
            QCOMPARE(queueCheck->getArguments().size(), 1);
            QVERIFY(std::holds_alternative<VariableRef>(queueCheck->getArguments().front()));
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().size(), 1);
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().front().m_name,
                    "Actuator_change_observer_f2_channel");
        }

        {
            const Conditional *changeObserverProcessingBlock = findProctypeElement<Conditional>(mainSequence, 4);
            QVERIFY(changeObserverProcessingBlock);
            QCOMPARE(changeObserverProcessingBlock->getAlternatives().size(), 2);
            const InlineCall *queueCheck =
                    findProctypeElement<InlineCall>(*changeObserverProcessingBlock->getAlternatives().front(), 0);
            QVERIFY(queueCheck);
            QCOMPARE(queueCheck->getName(), "nempty");
            QCOMPARE(queueCheck->getArguments().size(), 1);
            QVERIFY(std::holds_alternative<VariableRef>(queueCheck->getArguments().front()));
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().size(), 1);
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().front().m_name,
                    "Actuator_zero_observer_f2_channel");
        }

        {
            const Conditional *zeroObserverProcessingBlock = findProctypeElement<Conditional>(mainSequence, 5);
            QVERIFY(zeroObserverProcessingBlock);
            QCOMPARE(zeroObserverProcessingBlock->getAlternatives().size(), 2);
            const InlineCall *queueCheck =
                    findProctypeElement<InlineCall>(*zeroObserverProcessingBlock->getAlternatives().front(), 0);
            QVERIFY(queueCheck);
            QCOMPARE(queueCheck->getName(), "nempty");
            QCOMPARE(queueCheck->getArguments().size(), 1);
            QVERIFY(std::holds_alternative<VariableRef>(queueCheck->getArguments().front()));
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().size(), 1);
            QCOMPARE(std::get<VariableRef>(queueCheck->getArguments().front()).getElements().front().m_name,
                    "Actuator_f2_channel");
        }

        const ChannelSend *functionUnlockStatement = findProctypeElement<ChannelSend>(mainSequence, 6);
        QVERIFY(functionUnlockStatement);
    }
}

void tst_IvToPromelaTranslator::testUnhandledInputObservers()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("unhandled_input_observers.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "actuator");
    options.add(PromelaOptions::environmentFunctionName, "environ");
    options.add(PromelaOptions::observerAttachment, "second_observer:ObservedSignalKind.UNHANDLED_INPUT:uh_global::p1");
    options.add(PromelaOptions::observerAttachment,
            "first_observer:ObservedSignalKind.UNHANDLED_INPUT:uh_controller::>controller:p2");
    options.add(PromelaOptions::observerAttachment,
            "first_observer:ObservedSignalKind.UNHANDLED_INPUT:uh_ping:ping:>actuator:p2");
    options.add(PromelaOptions::observerFunctionName, "first_observer");
    options.add(PromelaOptions::observerFunctionName, "second_observer");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
    QVERIFY(promelaModel);

    // verify only inlines related to unhandled input
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_test_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const Sequence &content = inlineDef->getSequence();

        QCOMPARE(content.getContent().size(), 7);

        const PrintfStatement *printfStatement = findProctypeElement<PrintfStatement>(content, 0);
        QVERIFY(printfStatement);
        QCOMPARE(printfStatement->getArguments().size(), 1);
        printfStatement->getArguments().front().getContent();
        QVERIFY(std::holds_alternative<StringConstant>(printfStatement->getArguments().front().getContent()));
        QCOMPARE(std::get<StringConstant>(printfStatement->getArguments().front().getContent()).getValue(),
                "unhandled_input controller test\\n");

        const ChannelRecv *firstObserverLock = findProctypeElement<ChannelRecv>(content, 1);
        QVERIFY(firstObserverLock);
        QCOMPARE(firstObserverLock->getChannelRef().getElements().size(), 1);
        QCOMPARE(firstObserverLock->getChannelRef().getElements().front().m_name, "First_observer_lock");
        QCOMPARE(firstObserverLock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const InlineCall *firstObserverCall = findProctypeElement<InlineCall>(content, 2);
        QVERIFY(firstObserverCall);
        QCOMPARE(firstObserverCall->getName(), "First_observer_0_PI_0_uh_controller");
        QCOMPARE(firstObserverCall->getArguments().size(), 0);

        const ChannelSend *firstObserverUnlock = findProctypeElement<ChannelSend>(content, 3);
        QVERIFY(firstObserverUnlock);
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().size(), 1);
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().front().m_name, "First_observer_lock");
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const ChannelRecv *secondObserverLock = findProctypeElement<ChannelRecv>(content, 4);
        QVERIFY(secondObserverLock);
        QCOMPARE(secondObserverLock->getChannelRef().getElements().size(), 1);
        QCOMPARE(secondObserverLock->getChannelRef().getElements().front().m_name, "Second_observer_lock");
        QCOMPARE(secondObserverLock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const InlineCall *secondObserverCall = findProctypeElement<InlineCall>(content, 5);
        QVERIFY(secondObserverCall);
        QCOMPARE(secondObserverCall->getName(), "Second_observer_0_PI_0_uh_global");
        QCOMPARE(secondObserverCall->getArguments().size(), 0);

        const ChannelSend *secondObserverUnlock = findProctypeElement<ChannelSend>(content, 6);
        QVERIFY(secondObserverUnlock);
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().size(), 1);
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().front().m_name, "Second_observer_lock");
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().front().m_index.get(), nullptr);
    }
    {
        const InlineDef *inlineDef =
                findInline(promelaModel->getInlineDefs(), "Controller_0_PI_0_pong_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const Sequence &content = inlineDef->getSequence();

        QCOMPARE(content.getContent().size(), 7);

        const PrintfStatement *printfStatement = findProctypeElement<PrintfStatement>(content, 0);
        QVERIFY(printfStatement);
        QCOMPARE(printfStatement->getArguments().size(), 1);
        printfStatement->getArguments().front().getContent();
        QVERIFY(std::holds_alternative<StringConstant>(printfStatement->getArguments().front().getContent()));
        QCOMPARE(std::get<StringConstant>(printfStatement->getArguments().front().getContent()).getValue(),
                "unhandled_input controller pong\\n");

        const ChannelRecv *firstObserverLock = findProctypeElement<ChannelRecv>(content, 1);
        QVERIFY(firstObserverLock);
        QCOMPARE(firstObserverLock->getChannelRef().getElements().size(), 1);
        QCOMPARE(firstObserverLock->getChannelRef().getElements().front().m_name, "First_observer_lock");
        QCOMPARE(firstObserverLock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const InlineCall *firstObserverCall = findProctypeElement<InlineCall>(content, 2);
        QVERIFY(firstObserverCall);
        QCOMPARE(firstObserverCall->getName(), "First_observer_0_PI_0_uh_controller");
        QCOMPARE(firstObserverCall->getArguments().size(), 0);

        const ChannelSend *firstObserverUnlock = findProctypeElement<ChannelSend>(content, 3);
        QVERIFY(firstObserverUnlock);
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().size(), 1);
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().front().m_name, "First_observer_lock");
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const ChannelRecv *secondObserverLock = findProctypeElement<ChannelRecv>(content, 4);
        QVERIFY(secondObserverLock);
        QCOMPARE(secondObserverLock->getChannelRef().getElements().size(), 1);
        QCOMPARE(secondObserverLock->getChannelRef().getElements().front().m_name, "Second_observer_lock");
        QCOMPARE(secondObserverLock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const InlineCall *secondObserverCall = findProctypeElement<InlineCall>(content, 5);
        QVERIFY(secondObserverCall);
        QCOMPARE(secondObserverCall->getName(), "Second_observer_0_PI_0_uh_global");
        QCOMPARE(secondObserverCall->getArguments().size(), 0);

        const ChannelSend *secondObserverUnlock = findProctypeElement<ChannelSend>(content, 6);
        QVERIFY(secondObserverUnlock);
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().size(), 1);
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().front().m_name, "Second_observer_lock");
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().front().m_index.get(), nullptr);
    }
    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Actuator_0_PI_0_ping_unhandled_input");
        QVERIFY(inlineDef != nullptr);
        QCOMPARE(inlineDef->getArguments().size(), 1);
        const Sequence &content = inlineDef->getSequence();

        QCOMPARE(content.getContent().size(), 7);

        const PrintfStatement *printfStatement = findProctypeElement<PrintfStatement>(content, 0);
        QVERIFY(printfStatement);
        QCOMPARE(printfStatement->getArguments().size(), 1);
        printfStatement->getArguments().front().getContent();
        QVERIFY(std::holds_alternative<StringConstant>(printfStatement->getArguments().front().getContent()));
        QCOMPARE(std::get<StringConstant>(printfStatement->getArguments().front().getContent()).getValue(),
                "unhandled_input actuator ping\\n");

        const ChannelRecv *firstObserverLock = findProctypeElement<ChannelRecv>(content, 1);
        QVERIFY(firstObserverLock);
        QCOMPARE(firstObserverLock->getChannelRef().getElements().size(), 1);
        QCOMPARE(firstObserverLock->getChannelRef().getElements().front().m_name, "First_observer_lock");
        QCOMPARE(firstObserverLock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const InlineCall *firstObserverCall = findProctypeElement<InlineCall>(content, 2);
        QVERIFY(firstObserverCall);
        QCOMPARE(firstObserverCall->getName(), "First_observer_0_PI_0_uh_ping");
        QCOMPARE(firstObserverCall->getArguments().size(), 1);

        const ChannelSend *firstObserverUnlock = findProctypeElement<ChannelSend>(content, 3);
        QVERIFY(firstObserverUnlock);
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().size(), 1);
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().front().m_name, "First_observer_lock");
        QCOMPARE(firstObserverUnlock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const ChannelRecv *secondObserverLock = findProctypeElement<ChannelRecv>(content, 4);
        QVERIFY(secondObserverLock);
        QCOMPARE(secondObserverLock->getChannelRef().getElements().size(), 1);
        QCOMPARE(secondObserverLock->getChannelRef().getElements().front().m_name, "Second_observer_lock");
        QCOMPARE(secondObserverLock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const InlineCall *secondObserverCall = findProctypeElement<InlineCall>(content, 5);
        QVERIFY(secondObserverCall);
        QCOMPARE(secondObserverCall->getName(), "Second_observer_0_PI_0_uh_global");
        QCOMPARE(secondObserverCall->getArguments().size(), 0);

        const ChannelSend *secondObserverUnlock = findProctypeElement<ChannelSend>(content, 6);
        QVERIFY(secondObserverUnlock);
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().size(), 1);
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().front().m_name, "Second_observer_lock");
        QCOMPARE(secondObserverUnlock->getChannelRef().getElements().front().m_index.get(), nullptr);
    }
}

void tst_IvToPromelaTranslator::testSynchronousInterfaces()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("synchronous_interfaces.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "actuator");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
    QVERIFY(promelaModel);

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_test_protected");
        QVERIFY(inlineDef);
        QCOMPARE(inlineDef->getArguments().size(), 3);
        const Sequence &content = inlineDef->getSequence();
        QCOMPARE(content.getContent().size(), 3);

        const ChannelRecv *functionLock = findProctypeElement<ChannelRecv>(content, 0);
        QVERIFY(functionLock);
        QCOMPARE(functionLock->getChannelRef().getElements().size(), 1);
        QCOMPARE(functionLock->getChannelRef().getElements().front().m_name, "Actuator_lock");
        QCOMPARE(functionLock->getChannelRef().getElements().front().m_index.get(), nullptr);

        const InlineCall *functionCall = findProctypeElement<InlineCall>(content, 1);
        QVERIFY(functionCall);
        QCOMPARE(functionCall->getName(), "Actuator_0_PI_0_test_protected");
        QCOMPARE(functionCall->getArguments().size(), 3);

        const ChannelSend *functionUnlock = findProctypeElement<ChannelSend>(content, 2);
        QVERIFY(functionUnlock);
        QCOMPARE(functionUnlock->getChannelRef().getElements().size(), 1);
        QCOMPARE(functionUnlock->getChannelRef().getElements().front().m_name, "Actuator_lock");
        QCOMPARE(functionUnlock->getChannelRef().getElements().front().m_index.get(), nullptr);
    }

    {
        const InlineDef *inlineDef = findInline(promelaModel->getInlineDefs(), "Controller_0_RI_0_test_unprotected");
        QVERIFY(inlineDef);
        QCOMPARE(inlineDef->getArguments().size(), 3);
        const Sequence &content = inlineDef->getSequence();
        QCOMPARE(content.getContent().size(), 1);

        const InlineCall *functionCall = findProctypeElement<InlineCall>(content, 0);
        QVERIFY(functionCall);
        QCOMPARE(functionCall->getName(), "Actuator_0_PI_0_test_unprotected");
        QCOMPARE(functionCall->getArguments().size(), 3);
    }
}

void tst_IvToPromelaTranslator::testChannelNames()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("channel_names.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::modelFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "actuator");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
    QVERIFY(promelaModel);

    {
        const Declaration *declaration =
                findDeclaration(promelaModel->getDeclarations(), "Actuator_testsignal_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }
    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Actuator_ping_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }
    {
        const Declaration *declaration = findDeclaration(promelaModel->getDeclarations(), "Controller_pong_channel");
        QVERIFY(declaration != nullptr);
        QVERIFY(declaration->getType().isBasicType());
        QCOMPARE(declaration->getType().getBasicType(), BasicType::CHAN);
        QCOMPARE(declaration->getVisibility(), Declaration::Visibility::NORMAL);
        QVERIFY(declaration->hasInit());
    }
}

void tst_IvToPromelaTranslator::testEnvironmentCallsSynchronousInterface()
{
    std::unique_ptr<ivm::IVModel> ivModel = importIvModel("env_calls_sync.xml");
    QVERIFY(ivModel);

    conversion::Options options;
    options.add(PromelaOptions::environmentFunctionName, "controller");
    options.add(PromelaOptions::modelFunctionName, "actuator");

    std::unique_ptr<PromelaModel> promelaModel = translateIvToPromela(std::move(ivModel), options);
    QVERIFY(promelaModel);

    QCOMPARE(promelaModel->getProctypes().size(), 1);
    const Proctype *proctype = findProctype(promelaModel->getProctypes(), "Controller_set_position");
    QVERIFY(proctype != nullptr);
    QVERIFY(proctype->isActive());
    QCOMPARE(proctype->getInstancesCount(), 1);
    const Sequence &main = proctype->getSequence();

    const Expression *wait_for_init = findProctypeElement<Expression>(main, 0);
    QVERIFY(wait_for_init != nullptr);
    const Declaration *parameterDeclaration = findProctypeElement<Declaration>(main, 1);
    QVERIFY(parameterDeclaration != nullptr);
    QCOMPARE(parameterDeclaration->getName(), "value_1");
    QVERIFY(parameterDeclaration->getType().isUtypeReference());
    QCOMPARE(parameterDeclaration->getType().getUtypeReference().getName(), "MyInteger");

    const DoLoop *mainLoop = findProctypeElement<DoLoop>(main, 2);
    QVERIFY(mainLoop != nullptr);
    QCOMPARE(mainLoop->getSequences().size(), 1);

    const Sequence &loopSequence = *mainLoop->getSequences().front();
    QCOMPARE(loopSequence.getType(), Sequence::Type::ATOMIC);
    QVERIFY(!loopSequence.isEmpty());

    const InlineCall *genValueCall = findProctypeElement<InlineCall>(loopSequence, 0);
    QVERIFY(genValueCall != nullptr);
    QCOMPARE(genValueCall->getName(), "MyInteger_generate_value");
    QCOMPARE(genValueCall->getArguments().size(), 1);
    const ChannelRecv *lockAcquire = findProctypeElement<ChannelRecv>(loopSequence, 1);
    QVERIFY(lockAcquire != nullptr);
    QCOMPARE(lockAcquire->getChannelRef().getElements().size(), 1);
    QCOMPARE(lockAcquire->getChannelRef().getElements().front().m_name, "Actuator_lock");
    const InlineCall *interfaceCall = findProctypeElement<InlineCall>(loopSequence, 2);
    QVERIFY(interfaceCall != nullptr);
    QCOMPARE(interfaceCall->getName(), "Actuator_0_PI_0_set_position");
    QCOMPARE(interfaceCall->getArguments().size(), 1);
    const ChannelSend *lockRelease = findProctypeElement<ChannelSend>(loopSequence, 3);
    QVERIFY(lockRelease != nullptr);
    QCOMPARE(lockRelease->getChannelRef().getElements().size(), 1);
    QCOMPARE(lockRelease->getChannelRef().getElements().front().m_name, "Actuator_lock");
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

std::unique_ptr<PromelaModel> tst_IvToPromelaTranslator::translateIvToPromela(
        std::unique_ptr<ivm::IVModel> ivModel, const conversion::Options &options)
{
    IvToPromelaTranslator translator;
    auto asn1Model = std::make_unique<Asn1Acn::Asn1Model>();
    std::vector<conversion::Model *> inputs;
    inputs.push_back(ivModel.get());
    inputs.push_back(asn1Model.get());
    auto result = translator.translateModels(std::move(inputs), options);

    if (result.empty()) {
        return {};
    }
    if (result.front()->modelType() != conversion::ModelType::PromelaSystem) {
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

void tst_IvToPromelaTranslator::verifyProctypeSimple(
        const Proctype *proctype, const QString &functionName, const QString &interfaceName, size_t expectedParameters)
{
    const QString expectedLoopLabel = QString("%1_%2_loop").arg(functionName).arg(interfaceName);
    const QString expectedChannelName = QString("%1_%2_channel").arg(functionName).arg(interfaceName);
    const QString expectedChannelUsedName = QString("%1_used").arg(expectedChannelName);
    const QString expectedProcessInlineName = QString("%1_0_PI_0_%2").arg(functionName).arg(interfaceName);

    QVERIFY(proctype->isActive());
    QCOMPARE(proctype->getInstancesCount(), 1);

    const Sequence &main = proctype->getSequence();

    const DoLoop *mainLoop = findProctypeElement<DoLoop>(main, 1);
    QVERIFY(mainLoop);

    QVERIFY(mainLoop->getSequences().size() > 0);

    const Sequence &mainSequence = *mainLoop->getSequences().front();

    const Expression *queueCheckExpression = findProctypeElement<Expression>(mainSequence, 0);
    QVERIFY(queueCheckExpression);

    const ChannelRecv *functionLockStatement = findProctypeElement<ChannelRecv>(mainSequence, 1);
    QVERIFY(functionLockStatement);

    const Label *loopLabel = findProctypeElement<Label>(mainSequence, 2);
    QVERIFY(loopLabel);
    QCOMPARE(loopLabel->getName(), expectedLoopLabel);

    const Conditional *sdlProcessingBlock = findProctypeElement<Conditional>(mainSequence, 3);
    QVERIFY(sdlProcessingBlock);

    QCOMPARE(sdlProcessingBlock->getAlternatives().size(), 2);
    const Sequence &processingSequence = *sdlProcessingBlock->getAlternatives().front();

    const InlineCall *processingCheckExpression = findProctypeElement<InlineCall>(processingSequence, 0);
    QVERIFY(processingCheckExpression);
    QCOMPARE(processingCheckExpression->getName(), "nempty");
    const ChannelRecv *processingChannelRecv = findProctypeElement<ChannelRecv>(processingSequence, 1);
    QVERIFY(processingChannelRecv);
    QCOMPARE(processingChannelRecv->getChannelRef().getElements().size(), 1);
    QCOMPARE(processingChannelRecv->getChannelRef().getElements().front().m_name, expectedChannelName);
    QVERIFY(processingChannelRecv->getChannelRef().getElements().front().m_index.get() == nullptr);

    size_t element_index = 2;
    if (expectedParameters > 0) {
        const Assignment *channelUsedAssignment = findProctypeElement<Assignment>(processingSequence, element_index);
        QVERIFY(channelUsedAssignment);

        QCOMPARE(channelUsedAssignment->getVariableRef().getElements().size(), 1);
        QCOMPARE(channelUsedAssignment->getVariableRef().getElements().front().m_name, expectedChannelUsedName);
        QVERIFY(channelUsedAssignment->getVariableRef().getElements().front().m_index.get() == nullptr);
        ++element_index;
    }

    const InlineCall *sdlProcessingCall = findProctypeElement<InlineCall>(processingSequence, element_index);
    QVERIFY(sdlProcessingCall);
    QCOMPARE(sdlProcessingCall->getName(), expectedProcessInlineName);
    QCOMPARE(sdlProcessingCall->getArguments().size(), expectedParameters);
    ++element_index;

    const GoTo *gotoLoop = findProctypeElement<GoTo>(processingSequence, element_index);
    QVERIFY(gotoLoop);
    QCOMPARE(gotoLoop->getLabel(), expectedLoopLabel);

    const ChannelSend *functionUnlockStatement = findProctypeElement<ChannelSend>(mainSequence, 4);
    QVERIFY(functionUnlockStatement);
}
}
QTEST_MAIN(tmc::test::tst_IvToPromelaTranslator)

#include "tst_ivtopromelatranslator.moc"

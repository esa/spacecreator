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
#include <QtTest>
#include <asn1library/asn1/asn1model.h>
#include <asn1modelbuilder/asn1modelbuilder.h>
#include <conversion/asn1/SedsToAsn1Translator/translator.h>
#include <conversion/common/options.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/iv/IvOptions/options.h>
#include <conversion/iv/SedsToIvTranslator/translator.h>
#include <conversion/sdl/SedsToSdlTranslator/specialized/activitytranslator.h>
#include <conversion/sdl/SedsToSdlTranslator/specialized/mathoperationtranslator.h>
#include <conversion/sdl/SedsToSdlTranslator/specialized/statementtranslatorvisitor.h>
#include <conversion/sdl/SedsToSdlTranslator/translator.h>
#include <iostream>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/task.h>
#include <seds/SedsModel/components/activities/mathoperation.h>
#include <sedsmodelbuilder/sedsactivitybuilder.h>
#include <sedsmodelbuilder/sedscommandprimitivebuilder.h>
#include <sedsmodelbuilder/sedscomponentbuilder.h>
#include <sedsmodelbuilder/sedsimplementationbuilder.h>
#include <sedsmodelbuilder/sedsinterfacebuilder.h>
#include <sedsmodelbuilder/sedsinterfacecommandbuilder.h>
#include <sedsmodelbuilder/sedsinterfacedeclarationbuilder.h>
#include <sedsmodelbuilder/sedsinvocationbuilder.h>
#include <sedsmodelbuilder/sedsmodelbuilder.h>
#include <sedsmodelbuilder/sedsstatemachinebuilder.h>
#include <unittests/common/verifyexception.h>

using namespace seds::model;
using namespace sdl;

using conversion::Options;
using conversion::sdl::translator::ActivityTranslator;
using conversion::sdl::translator::MathOperationTranslator;
using conversion::sdl::translator::SedsToSdlTranslator;
using conversion::translator::TranslationException;

using tests::conversion::common::Asn1ModelBuilder;
using tests::conversion::common::SedsActivityBuilder;
using tests::conversion::common::SedsCommandPrimitiveBuilder;
using tests::conversion::common::SedsComponentBuilder;
using tests::conversion::common::SedsImplementationBuilder;
using tests::conversion::common::SedsInterfaceBuilder;
using tests::conversion::common::SedsInterfaceCommandBuilder;
using tests::conversion::common::SedsInterfaceDeclarationBuilder;
using tests::conversion::common::SedsInvocationBuilder;
using tests::conversion::common::SedsModelBuilder;
using tests::conversion::common::SedsStateMachineBuilder;

namespace conversion::sdl::test {

class MockModel final : public conversion::Model
{
    virtual auto modelType() const -> conversion::ModelType override { return conversion::ModelType::Unspecified; }
};

class tst_SedsToSdlTranslator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testMissingModel();
    void testNotEnoughModels();
    void testTooManyModels();
    void testMissingModels();

    void testTranslateStateMachineIntoProcess();
    void testTranslateStateMachineStates();
    void testTranslateStateMachineInputs();
    void testTranslateStateMachineInputsWithVariables();
    void testTranslateActivity();
    void testTranslateStateTransitionActivity();

    void testTranslateAddOperation();
    void testTranslateSubOperation();
    void testTranslateMulOperation();
    void testTranslateDivOperation();
    void testTranslateModOperation();
    void testTranslatePowOperation();
    void testTranslateInvOperation();
    void testTranslateTanOperation();
    void testTranslateSinOperation();
    void testTranslateCosOperation();
    void testTranslateCeilOperation();
    void testTranslateFloorOperation();
    void testTranslateRoundOperation();
    void testTranslateAbsOperation();
    void testTranslateMinOperation();
    void testTranslateMaxOperation();
    void testTranslateSqrtOperation();
    void testTranslateComplexOperation();

    void testTranslateAssignment();
    void testTranslateMathOperation();
    void testTranslatePolynomialCalibrator();
    void testTranslateActivityCall();
    void testTranslateSendCommand();
    void testTranslateOnEntryAndOnExit();
};

void tst_SedsToSdlTranslator::testMissingModel()
{
    Options options;
    SedsToSdlTranslator translator;

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({}, options), TranslationException,
            "No models passed for translation for SEDS to SDL translation");
}

void tst_SedsToSdlTranslator::testNotEnoughModels()
{
    Options options;
    SedsToSdlTranslator translator;

    const auto sedsModel = SedsModelBuilder("Package").build();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({ sedsModel.get() }, options), TranslationException,
            "Not enough models passed for SEDS to SDL translation");
}

void tst_SedsToSdlTranslator::testTooManyModels()
{
    Options options;
    SedsToSdlTranslator translator;

    const auto sedsModel1 = SedsModelBuilder("Package").build();
    const auto sedsModel2 = SedsModelBuilder("Package").build();
    const auto sedsModel3 = SedsModelBuilder("Package").build();
    const auto sedsModel4 = SedsModelBuilder("Package").build();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(
            translator.translateModels(
                    { sedsModel1.get(), sedsModel2.get(), sedsModel3.get(), sedsModel4.get() }, options),
            TranslationException, "Too many models passed for SEDS to SDL translation");
}

void tst_SedsToSdlTranslator::testMissingModels()
{
    Options options;
    SedsToSdlTranslator translator;

    const auto sedsModel = SedsModelBuilder("Package").build();
    const auto mockModel = std::make_unique<MockModel>();

    VERIFY_EXCEPTION_THROWN_WITH_MESSAGE(translator.translateModels({ sedsModel.get(), mockModel.get() }, options),
            TranslationException, "Not enough models passed for SEDS to SDL translation");
}

/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2490
void tst_SedsToSdlTranslator::testTranslateStateMachineIntoProcess()
{
    // clang-format off
    const auto sedsModel =
        SedsModelBuilder("Package")
            .withComponent(
                SedsComponentBuilder("Component")
                    .withImplementation(
                        SedsImplementationBuilder()
                        .withStateMachine(
                            SedsStateMachineBuilder()
                            .build()
                        )
                        .build()
                    )
                    .build())
            .build();

    // clang-format on

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);
    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Sdl);

    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    QVERIFY(sdlModel);

    QCOMPARE(sdlModel->processes().size(), 1);

    const auto &process = sdlModel->processes()[0];
    QCOMPARE(process.name(), "Component");
}

static inline auto getStateOfName(const ::sdl::Process &process, const QString name) -> const ::sdl::State *
{
    auto iterator = std::find_if(process.stateMachine()->states().begin(), process.stateMachine()->states().end(),
            [name](auto &state) { return state->name() == name; });

    if (iterator == process.stateMachine()->states().end())
        return nullptr;
    return (*iterator).get();
}

/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2500
/// \SRS  ETB-FUN-2520
void tst_SedsToSdlTranslator::testTranslateStateMachineStates()
{
    // clang-format off
    const auto sedsModel =
        SedsModelBuilder("Package")
            .withComponent(
                SedsComponentBuilder("Component")
                    .withImplementation(
                        SedsImplementationBuilder()
                        .withStateMachine(
                            SedsStateMachineBuilder()
                            .withEntryState("StateA")
                            .withState("StateB")
                            .build()
                        )
                        .build()
                    )
                    .build())
            .build();

    // clang-format on

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    QCOMPARE(resultModels.size(), 1);

    const auto &resultModel = resultModels[0];
    QCOMPARE(resultModel->modelType(), ModelType::Sdl);

    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    QVERIFY(sdlModel);
    QCOMPARE(sdlModel->processes().size(), 1);

    const auto &process = sdlModel->processes()[0];
    QCOMPARE(process.name(), "Component");
    const auto stateA = getStateOfName(process, "StateA");
    QVERIFY(stateA);
    const auto stateB = getStateOfName(process, "StateB");
    QVERIFY(stateB);
}

/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2010
/// \SRS  ETB-FUN-2490
/// \SRS  ETB-FUN-2500
/// \SRS  ETB-FUN-2520
/// \SRS  ETB-FUN-2550
void tst_SedsToSdlTranslator::testTranslateStateMachineInputs()
{
    // clang-format off
    const auto sedsModel =
        SedsModelBuilder("Package")
            .withComponent(
                SedsComponentBuilder("Component")
                    .withImplementation(
                        SedsImplementationBuilder()
                        .withStateMachine(
                            SedsStateMachineBuilder()
                            .withEntryState("StateA")
                            .withState("StateB")
                            .withTransition("StateA", "StateB",
                                SedsCommandPrimitiveBuilder("If1", "Cmd1").build()
                            )
                            .build()
                        )
                        .build()
                    )
                    .build())
            .build();

    // clang-format on

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    const auto &resultModel = resultModels[0];
    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    const auto &process = sdlModel->processes()[0];
    const auto &startTransition = sdlModel->processes()[0].startTransition();

    QVERIFY(startTransition);
    QCOMPARE(startTransition->actions().size(), 1);
    const auto entryAction = startTransition->actions()[0].get();
    const auto entryState = dynamic_cast<const ::sdl::NextState *>(entryAction);
    QVERIFY(entryState);
    QCOMPARE(entryState->state()->name(), "StateA");
    const auto stateA = getStateOfName(process, "StateA");
    QCOMPARE(stateA->inputs().size(), 1);
    QCOMPARE(stateA->inputs()[0]->name(), "If1_Cmd1_Pi");
    QCOMPARE(stateA->inputs()[0]->transition()->actions().size(), 1);
    const auto action = stateA->inputs()[0]->transition()->actions()[0].get();
    const auto nextState = dynamic_cast<const ::sdl::NextState *>(action);
    QVERIFY(nextState);
    QCOMPARE(nextState->state()->name(), "StateB");
}

/// \SRS  ETB-FUN-2020
/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2570
void tst_SedsToSdlTranslator::testTranslateStateMachineInputsWithVariables()
{
    // clang-format off
    const auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(
                            SedsComponentBuilder("Component")
                                    .declaringInterface(SedsInterfaceDeclarationBuilder("If1Type")
                                                                .withCommand(SedsInterfaceCommandBuilder(
                                                                        "Cmd1", InterfaceCommandMode::Async)
                                                                                     .withArgument("arg1", "Integer",
                                                                                             CommandArgumentMode::In)
                                                                                     .build())
                                                                .build())
                                    .withProvidedInterface(SedsInterfaceBuilder("If1", "If1Type").build())
                                    .withImplementation(
                                            SedsImplementationBuilder()
                                                    .withVariable("var1", "Integer")
                                                    .withStateMachine(
                                                            SedsStateMachineBuilder()
                                                                    .withEntryState("StateA")
                                                                    .withState("StateB")
                                                                    .withTransition("StateA", "StateB",
                                                                            SedsCommandPrimitiveBuilder("If1", "Cmd1")
                                                                                    .withArgumentValue("arg1", "var1")
                                                                                    .build())
                                                                    .build())
                                                    .build())
                                    .build())
                    .build();

    // clang-format on

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    const auto &resultModel = resultModels[0];
    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    const auto &process = sdlModel->processes()[0];
    const auto stateA = getStateOfName(process, "StateA");

    QCOMPARE(stateA->inputs().size(), 1);
    QCOMPARE(stateA->inputs()[0]->name(), "If1_Cmd1_Pi");
    QCOMPARE(stateA->inputs()[0]->transition()->actions().size(), 2);

    const auto unpackAction = stateA->inputs()[0]->transition()->actions()[0].get();
    const auto unpack = dynamic_cast<const ::sdl::Task *>(unpackAction);
    QCOMPARE(unpack->content(), "var1 := io_If1_Cmd1_Pi.arg1");

    const auto nextStateAction = stateA->inputs()[0]->transition()->actions()[1].get();
    const auto nextState = dynamic_cast<const ::sdl::NextState *>(nextStateAction);
    QVERIFY(nextState);
    QCOMPARE(nextState->state()->name(), "StateB");
}

/// \SRS  ETB-FUN-2060
/// \SRS  ETB-FUN-2070
void tst_SedsToSdlTranslator::testTranslateActivity()
{
    // clang-format off
    const auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(SedsComponentBuilder("Component")
                                           .withImplementation(
                                                   SedsImplementationBuilder()
                                                           .withVariable("var1", "Integer")
                                                           .withActivity(SedsActivityBuilder("activity1")
                                                                                 .withArgument("arg1", "Integer")
                                                                                 .withValueAssignment("var1", "1969")
                                                                                 .build())
                                                           .build())
                                           .build())
                    .build();

    // clang-format on

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    const auto &resultModel = resultModels[0];
    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    const auto &process = sdlModel->processes()[0];

    QCOMPARE(process.procedures().size(), 1);
    const auto &procedure = process.procedures()[0];
    QCOMPARE(procedure->name(), "activity1");
    QCOMPARE(procedure->parameters().size(), 1);
    QCOMPARE(procedure->parameters()[0]->name(), "arg1");
    const auto &transition = procedure->transition();
    QCOMPARE(transition->actions().size(), 1);
    const auto action = dynamic_cast<::sdl::Task *>(transition->actions()[0].get());
    QVERIFY(action);
    QCOMPARE(action->content(), "var1 := 1969");
}

/// \SRS  ETB-FUN-2550
/// \SRS  ETB-FUN-2560
/// \SRS  ETB-FUN-2660
void tst_SedsToSdlTranslator::testTranslateStateTransitionActivity()
{
    // clang-format off
    const auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(
                            SedsComponentBuilder("Component")
                                    .declaringInterface(SedsInterfaceDeclarationBuilder("If1Type")
                                                                .withCommand(SedsInterfaceCommandBuilder(
                                                                        "Cmd1", InterfaceCommandMode::Async)
                                                                                     .build())
                                                                .build())
                                    .withProvidedInterface(SedsInterfaceBuilder("If1", "If1Type").build())
                                    .withImplementation(
                                            SedsImplementationBuilder()
                                                    .withActivity(SedsActivityBuilder("activity1")
                                                                          .build())
                                                    .withStateMachine(
                                                            SedsStateMachineBuilder()
                                                                    .withEntryState("StateA")
                                                                    .withState("StateB")
                                                                    .withTransition("StateA", "StateB",
                                                                            SedsCommandPrimitiveBuilder("If1", "Cmd1")
                                                                                    .build(),
                                                                            "activity1")
                                                                    .build())
                                                    .build())
                                    .build())
                    .build();

    // clang-format on

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    const auto &resultModel = resultModels[0];
    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    const auto &process = sdlModel->processes()[0];

    const auto stateA = getStateOfName(process, "StateA");

    QCOMPARE(stateA->inputs()[0]->transition()->actions().size(), 2);

    const auto invocationAction = stateA->inputs()[0]->transition()->actions()[0].get();
    const auto invocation = dynamic_cast<const ::sdl::ProcedureCall *>(invocationAction);
    QVERIFY(invocation);
    QCOMPARE(invocation->procedure()->name(), "activity1");
}

/// \SRS  ETB-FUN-2220
void tst_SedsToSdlTranslator::testTranslateAddOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Add);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    operation.push_back(seds::model::VariableRef("b"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "a + b");
}

/// \SRS  ETB-FUN-2230
void tst_SedsToSdlTranslator::testTranslateSubOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Subtract);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    operation.push_back(seds::model::VariableRef("b"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "a - b");
}

/// \SRS  ETB-FUN-2240
void tst_SedsToSdlTranslator::testTranslateMulOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Multiply);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    operation.push_back(seds::model::VariableRef("b"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "a * b");
}

/// \SRS  ETB-FUN-2250
void tst_SedsToSdlTranslator::testTranslateDivOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Divide);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    operation.push_back(seds::model::VariableRef("b"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "a / b");
}

/// \SRS  ETB-FUN-2260
void tst_SedsToSdlTranslator::testTranslateModOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Modulus);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    operation.push_back(seds::model::VariableRef("b"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "mod(a, b)");
}

/// \SRS  ETB-FUN-2270
void tst_SedsToSdlTranslator::testTranslatePowOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Pow);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    operation.push_back(seds::model::VariableRef("b"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "power(a, b)");
}

/// \SRS  ETB-FUN-2280
void tst_SedsToSdlTranslator::testTranslateInvOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Inverse);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "1/a");
}

/// \SRS  ETB-FUN-2290
void tst_SedsToSdlTranslator::testTranslateTanOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Tan);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "tan(a)");
}

/// \SRS  ETB-FUN-2300
void tst_SedsToSdlTranslator::testTranslateSinOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Sin);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "sin(a)");
}

/// \SRS  ETB-FUN-2310
void tst_SedsToSdlTranslator::testTranslateCosOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Cos);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "cos(a)");
}

/// \SRS  ETB-FUN-2330
void tst_SedsToSdlTranslator::testTranslateCeilOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Ceil);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "ceil(a)");
}

/// \SRS  ETB-FUN-2340
void tst_SedsToSdlTranslator::testTranslateFloorOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Floor);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "floor(a)");
}

/// \SRS  ETB-FUN-2350
void tst_SedsToSdlTranslator::testTranslateRoundOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Round);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "round(a)");
}

/// \SRS  ETB-FUN-2360
void tst_SedsToSdlTranslator::testTranslateAbsOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Abs);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "abs(a)");
}

/// \SRS  ETB-FUN-2370
void tst_SedsToSdlTranslator::testTranslateMinOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Min);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    operation.push_back(seds::model::VariableRef("b"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "min(a, b)");
}

/// \SRS  ETB-FUN-2380
void tst_SedsToSdlTranslator::testTranslateMaxOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Max);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    operation.push_back(seds::model::VariableRef("b"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "max(a, b)");
}

/// \SRS  ETB-FUN-2390
void tst_SedsToSdlTranslator::testTranslateSqrtOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator op;
    op.setMathOperator(seds::model::CoreMathOperator::Sqrt);
    operation.push_back(std::move(op));
    operation.push_back(seds::model::VariableRef("a"));
    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "sqrt(a)");
}

/// \SRS  ETB-FUN-2220
/// \SRS  ETB-FUN-2230
/// \SRS  ETB-FUN-2240
/// \SRS  ETB-FUN-2250
void tst_SedsToSdlTranslator::testTranslateComplexOperation()
{
    seds::model::MathOperation::Elements operation;
    seds::model::Operator add;
    add.setMathOperator(seds::model::CoreMathOperator::Add);

    seds::model::Operator sub;
    sub.setMathOperator(seds::model::CoreMathOperator::Subtract);

    seds::model::Operator mul;
    mul.setMathOperator(seds::model::CoreMathOperator::Multiply);

    seds::model::Operator div;
    div.setMathOperator(seds::model::CoreMathOperator::Divide);

    operation.push_back(std::move(div));
    operation.push_back(std::move(add));
    operation.push_back(std::move(mul));
    operation.push_back(std::move(sub));
    operation.push_back(seds::model::VariableRef("s1"));
    operation.push_back(seds::model::VariableRef("s2"));
    operation.push_back(seds::model::VariableRef("m2"));
    operation.push_back(seds::model::VariableRef("a2"));
    operation.push_back(seds::model::VariableRef("d2"));

    const auto result = MathOperationTranslator::translateOperation(operation);

    QCOMPARE(result, "(((s1 - s2) * m2) + a2) / d2");
}

static inline auto translateAndVerifyActivityContainsAction(const std::unique_ptr<seds::model::SedsModel> model,
        const QString activityName, const QString actionContent) -> void
{
    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ model.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ model.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ model.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    const auto &resultModel = resultModels[0];
    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    const auto &process = sdlModel->processes()[0];

    QCOMPARE(process.procedures().size(), 1);
    const auto &procedure = process.procedures()[0];
    QCOMPARE(procedure->name(), activityName);
    const auto &transition = procedure->transition();
    QCOMPARE(transition->actions().size(), 1);
    const auto action = dynamic_cast<::sdl::Task *>(transition->actions()[0].get());
    QVERIFY(action);
    QCOMPARE(action->content(), actionContent);
}

/// \SRS  ETB-FUN-2400
void tst_SedsToSdlTranslator::testTranslateAssignment()
{
    auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(
                            SedsComponentBuilder("Component")
                                    .withImplementation(SedsImplementationBuilder()
                                                                .withVariable("x", "Integer")
                                                                .withActivity(SedsActivityBuilder("activity1")
                                                                                      .withValueAssignment("a", "12")
                                                                                      .build())
                                                                .build())
                                    .build())
                    .build();

    translateAndVerifyActivityContainsAction(std::move(sedsModel), "activity1", "a := 12");
}

/// \SRS  ETB-FUN-2210
void tst_SedsToSdlTranslator::testTranslateMathOperation()
{
    auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(SedsComponentBuilder("Component")
                                           .withImplementation(
                                                   SedsImplementationBuilder()
                                                           .withVariable("x", "Integer")
                                                           .withVariable("y", "Integer")
                                                           .withActivity(SedsActivityBuilder("activity1")
                                                                                 .withMathOperation("x",
                                                                                         CoreMathOperator::Sin, "y")
                                                                                 .build())
                                                           .build())
                                           .build())
                    .build();

    translateAndVerifyActivityContainsAction(std::move(sedsModel), "activity1", "x := sin(y)");
}

/// \SRS  ETB-FUN-2200
/// \SRS  ETB-FUN-2480
void tst_SedsToSdlTranslator::testTranslatePolynomialCalibrator()
{
    auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(SedsComponentBuilder("Component")
                                           .withImplementation(
                                                   SedsImplementationBuilder()
                                                           .withVariable("x", "Integer")
                                                           .withVariable("y", "Integer")
                                                           .withActivity(SedsActivityBuilder("activity1")
                                                                                 .withPolynomialCalibration("x", "y",
                                                                                         { 2.0, 4.0, 5.0 }, { 0, 2, 4 })
                                                                                 .build())
                                                           .build())
                                           .build())
                    .build();

    translateAndVerifyActivityContainsAction(std::move(sedsModel), "activity1", "x := 2 + 4 * y * y + 5 * power(y, 4)");
}

/// \SRS  ETB-FUN-2430
/// \SRS  ETB-FUN-2440
/// \SRS  ETB-FUN-2460
void tst_SedsToSdlTranslator::testTranslateActivityCall()
{
    auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(
                            SedsComponentBuilder("Component")
                                    .withImplementation(SedsImplementationBuilder()
                                                                .withVariable("x", "Integer")
                                                                .withActivity(SedsActivityBuilder("activity1")
                                                                                      .withArgument("arg1", "Integer")
                                                                                      .build())
                                                                .withActivity(SedsActivityBuilder("activity2")
                                                                                      .withActivityCall("activity1",
                                                                                              { "arg1" }, { "x" })
                                                                                      .build())
                                                                .build())
                                    .build())
                    .build();

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    const auto &resultModel = resultModels[0];
    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    const auto &process = sdlModel->processes()[0];

    QCOMPARE(process.procedures().size(), 2);
    const auto &procedure1 = process.procedures()[0];
    QCOMPARE(procedure1->name(), "activity1");
    const auto &procedure2 = process.procedures()[1];
    QCOMPARE(procedure2->name(), "activity2");
    const auto &transition = procedure2->transition();
    QCOMPARE(transition->actions().size(), 1);
    const auto call = dynamic_cast<::sdl::ProcedureCall *>(transition->actions()[0].get());
    QVERIFY(call);
    QCOMPARE(call->procedure()->name(), "activity1");
    QCOMPARE(call->arguments().size(), 1);
    const auto &argument = call->arguments()[0];
    QVERIFY(std::holds_alternative<std::unique_ptr<VariableReference>>(argument));
    QCOMPARE(std::get<std::unique_ptr<VariableReference>>(argument)->declaration()->name(), "x");
}

/// \SRS  ETB-FUN-2140
/// \SRS  ETB-FUN-2150
/// \SRS  ETB-FUN-2170
void tst_SedsToSdlTranslator::testTranslateSendCommand()
{
    auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(
                            SedsComponentBuilder("Component")
                                    .declaringInterface(SedsInterfaceDeclarationBuilder("If1Type")
                                                                .withCommand(SedsInterfaceCommandBuilder(
                                                                        "Cmd1", InterfaceCommandMode::Async)
                                                                                     .withArgument("arg1", "Integer",
                                                                                             CommandArgumentMode::In)
                                                                                     .build())
                                                                .build())
                                    .withRequiredInterface(SedsInterfaceBuilder("If1", "If1Type").build())
                                    .withImplementation(SedsImplementationBuilder()
                                                                .withVariable("x", "Integer")
                                                                .withActivity(SedsActivityBuilder("activity1")
                                                                                      .withSendCommand("If1", "Cmd1",
                                                                                              { "arg1" }, { "x" })
                                                                                      .build())
                                                                .build())
                                    .build())
                    .build();

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto *ivModel = dynamic_cast<ivm::IVModel *>(ivModels[0].get());
    const auto *function = ivModel->getFunction("Component", Qt::CaseSensitive);
    for (const auto &interface : function->interfaces()) {
        std::cout << "IF " << interface->ifaceLabel().toStdString() << std::endl;
    }

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    const auto &resultModel = resultModels[0];
    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    const auto &process = sdlModel->processes()[0];

    QCOMPARE(process.variables().size(), 2);
    QCOMPARE(process.variables()[0]->name(), "x");
    QCOMPARE(process.variables()[1]->name(), "io_If1_Cmd1_Ri");

    QCOMPARE(process.procedures().size(), 1);
    const auto &procedure = process.procedures()[0];
    QCOMPARE(procedure->name(), "activity1");
    const auto &transition = procedure->transition();
    QCOMPARE(transition->actions().size(), 2);

    const auto pack = dynamic_cast<::sdl::Task *>(transition->actions()[0].get());
    QVERIFY(pack);
    QCOMPARE(pack->content(), "io_If1_Cmd1_Ri.arg1 := x");

    const auto output = dynamic_cast<::sdl::Output *>(transition->actions()[1].get());
    QVERIFY(output);
    QCOMPARE(output->name(), "If1_Cmd1_Ri");
    QCOMPARE(output->parameter()->declaration()->name(), "io_If1_Cmd1_Ri");
}

/// \SRS  ETB-FUN-2530
/// \SRS  ETB-FUN-2540
/// \SRS  ETB-FUN-2550
void tst_SedsToSdlTranslator::testTranslateOnEntryAndOnExit()
{
    const auto sedsModel =
            SedsModelBuilder("Package")
                    .withIntegerDataType("Integer")
                    .withComponent(
                            SedsComponentBuilder("Component")
                                    .declaringInterface(SedsInterfaceDeclarationBuilder("If1Type")
                                                                .withCommand(SedsInterfaceCommandBuilder(
                                                                        "Cmd1", InterfaceCommandMode::Async)
                                                                                     .build())
                                                                .build())
                                    .withProvidedInterface(SedsInterfaceBuilder("If1", "If1Type").build())
                                    .withImplementation(
                                            SedsImplementationBuilder()
                                                    .withActivity(SedsActivityBuilder("activity1").build())
                                                    .withActivity(SedsActivityBuilder("onExit1").build())
                                                    .withActivity(SedsActivityBuilder("onEntry1").build())
                                                    .withStateMachine(
                                                            SedsStateMachineBuilder()
                                                                    .withEntryState("StateA")
                                                                    .withState("StateB",
                                                                            SedsInvocationBuilder("onEntry1").build(),
                                                                            SedsInvocationBuilder("onExit1").build())
                                                                    .withTransition("StateA", "StateB",
                                                                            SedsCommandPrimitiveBuilder("If1", "Cmd1")
                                                                                    .build(),
                                                                            "activity1")
                                                                    .withTransition("StateB", "StateA",
                                                                            SedsCommandPrimitiveBuilder("If1", "Cmd1")
                                                                                    .build(),
                                                                            "activity1")
                                                                    .build())
                                                    .build())
                                    .build())
                    .build();

    Options options;
    options.add(conversion::iv::IvOptions::configFilepath, "config.xml");

    conversion::asn1::translator::SedsToAsn1Translator asn1Translator;
    conversion::iv::translator::SedsToIvTranslator ivTranslator;
    SedsToSdlTranslator sdlTranslator;

    const auto asn1Models = asn1Translator.translateModels({ sedsModel.get() }, options);
    const auto ivModels = ivTranslator.translateModels({ sedsModel.get(), asn1Models[0].get() }, options);

    const auto resultModels =
            sdlTranslator.translateModels({ sedsModel.get(), asn1Models[0].get(), ivModels[0].get() }, options);

    const auto &resultModel = resultModels[0];
    const auto *sdlModel = dynamic_cast<SdlModel *>(resultModel.get());
    const auto &process = sdlModel->processes()[0];

    const auto stateA = getStateOfName(process, "StateA");

    QCOMPARE(stateA->inputs()[0]->transition()->actions().size(), 3);

    const auto activityInvocationActionA = stateA->inputs()[0]->transition()->actions()[0].get();
    const auto activityInvocationA = dynamic_cast<const ::sdl::ProcedureCall *>(activityInvocationActionA);
    QVERIFY(activityInvocationA);
    QCOMPARE(activityInvocationA->procedure()->name(), "activity1");

    const auto entryInvocationActionA = stateA->inputs()[0]->transition()->actions()[1].get();
    const auto entryInvocationA = dynamic_cast<const ::sdl::ProcedureCall *>(entryInvocationActionA);
    QVERIFY(entryInvocationA);
    QCOMPARE(entryInvocationA->procedure()->name(), "onEntry1");

    const auto stateB = getStateOfName(process, "StateB");

    QCOMPARE(stateB->inputs()[0]->transition()->actions().size(), 3);

    const auto exitInvocationActionB = stateB->inputs()[0]->transition()->actions()[0].get();
    const auto exitInvocationB = dynamic_cast<const ::sdl::ProcedureCall *>(exitInvocationActionB);
    QVERIFY(exitInvocationB);
    QCOMPARE(exitInvocationB->procedure()->name(), "onExit1");

    const auto activityInvocationActionB = stateB->inputs()[0]->transition()->actions()[1].get();
    const auto activityInvocationB = dynamic_cast<const ::sdl::ProcedureCall *>(activityInvocationActionB);
    QVERIFY(activityInvocationB);
    QCOMPARE(activityInvocationB->procedure()->name(), "activity1");
}

} // namespace conversion::sdl::test

QTEST_MAIN(conversion::sdl::test::tst_SedsToSdlTranslator)

#include "tst_sedstosdltranslator.moc"

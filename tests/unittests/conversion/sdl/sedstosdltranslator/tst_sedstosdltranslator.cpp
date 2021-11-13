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
#include <conversion/sdl/SedsToSdlTranslator/translator.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/task.h>
#include <sedsmodelbuilder/sedsactivitybuilder.h>
#include <sedsmodelbuilder/sedscommandprimitivebuilder.h>
#include <sedsmodelbuilder/sedscomponentbuilder.h>
#include <sedsmodelbuilder/sedsimplementationbuilder.h>
#include <sedsmodelbuilder/sedsinterfacebuilder.h>
#include <sedsmodelbuilder/sedsinterfacecommandbuilder.h>
#include <sedsmodelbuilder/sedsinterfacedeclarationbuilder.h>
#include <sedsmodelbuilder/sedsmodelbuilder.h>
#include <sedsmodelbuilder/sedsstatemachinebuilder.h>
#include <unittests/common/verifyexception.h>

using namespace seds::model;
using namespace sdl;

using conversion::Options;
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
    options.add(conversion::iv::IvOptions::configFilename, "config.xml");

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
    options.add(conversion::iv::IvOptions::configFilename, "config.xml");

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
    options.add(conversion::iv::IvOptions::configFilename, "config.xml");

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
    options.add(conversion::iv::IvOptions::configFilename, "config.xml");

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
    QCOMPARE(unpack->content(), "var1 := input_If1_Cmd1_Pi.arg1");

    const auto nextStateAction = stateA->inputs()[0]->transition()->actions()[1].get();
    const auto nextState = dynamic_cast<const ::sdl::NextState *>(nextStateAction);
    QVERIFY(nextState);
    QCOMPARE(nextState->state()->name(), "StateB");
}

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
    options.add(conversion::iv::IvOptions::configFilename, "config.xml");

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
    options.add(conversion::iv::IvOptions::configFilename, "config.xml");

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

} // namespace conversion::sdl::test

QTEST_MAIN(conversion::sdl::test::tst_SedsToSdlTranslator)

#include "tst_sedstosdltranslator.moc"

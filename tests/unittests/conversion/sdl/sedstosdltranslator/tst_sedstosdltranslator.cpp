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
#include <conversion/common/options.h>
#include <conversion/common/translation/exceptions.h>
#include <conversion/sdl/SedsToSdlTranslator/translator.h>
#include <ivcore/ivmodel.h>
#include <sdl/SdlModel/nextstate.h>
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

    const auto asn1Model =
        Asn1ModelBuilder("Package")
        .build();
    // clang-format on

    Options options;
    SedsToSdlTranslator translator;

    const auto ivModel = std::make_unique<ivm::IVModel>(nullptr);

    const auto resultModels = translator.translateModels({ sedsModel.get(), asn1Model.get(), ivModel.get() }, options);
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
                            .withState("StateA")
                            .withState("StateB")
                            .build()
                        )
                        .build()
                    )
                    .build())
            .build();

    const auto asn1Model =
        Asn1ModelBuilder("Package")
        .build();
    // clang-format on

    Options options;
    SedsToSdlTranslator translator;

    const auto ivModel = std::make_unique<ivm::IVModel>(nullptr);

    const auto resultModels = translator.translateModels({ sedsModel.get(), asn1Model.get(), ivModel.get() }, options);
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

    const auto asn1Model =
        Asn1ModelBuilder("Package")
        .build();
    // clang-format on

    Options options;
    SedsToSdlTranslator translator;

    const auto ivModel = std::make_unique<ivm::IVModel>(nullptr);

    const auto resultModels = translator.translateModels({ sedsModel.get(), asn1Model.get(), ivModel.get() }, options);
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
    QCOMPARE(stateA->inputs().size(), 1);
    QCOMPARE(stateA->inputs()[0]->name(), "If1_Cmd1");
    QCOMPARE(stateA->inputs()[0]->transition()->actions().size(), 1);
    const auto action = stateA->inputs()[0]->transition()->actions()[0].get();
    const auto nextState = dynamic_cast<const ::sdl::NextState *>(action);
    QVERIFY(nextState);
    QCOMPARE(nextState->state()->name(), "StateB");
}

} // namespace conversion::sdl::test

QTEST_MAIN(conversion::sdl::test::tst_SedsToSdlTranslator)

#include "tst_sedstosdltranslator.moc"

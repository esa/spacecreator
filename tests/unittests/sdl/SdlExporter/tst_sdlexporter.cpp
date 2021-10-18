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

#include "statemachine.h"
#include "variabledeclaration.h"

#include <QObject>
#include <QtTest>
#include <common/sdlmodelbuilder/sdlinputbuilder.h>
#include <common/sdlmodelbuilder/sdlmodelbuilder.h>
#include <common/sdlmodelbuilder/sdlprocessbuilder.h>
#include <common/sdlmodelbuilder/sdlstatebuilder.h>
#include <common/sdlmodelbuilder/sdlstatemachinebuilder.h>
#include <common/sdlmodelbuilder/sdltaskbuilder.h>
#include <common/sdlmodelbuilder/sdltransitionbuilder.h>
#include <conversion/common/options.h>
#include <memory>
#include <sdl/SdlExporter/SdlOptions/options.h>
#include <sdl/SdlExporter/exporter.h>
#include <sdl/SdlModel/action.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/output.h>
#include <sdl/SdlModel/process.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/task.h>
#include <sdl/SdlModel/variabledeclaration.h>
#include <sdl/SdlModel/variableliteral.h>
#include <sdl/SdlModel/variablereference.h>

using conversion::ModelType;
using conversion::Options;
using sdl::Action;
using sdl::ContinuousSignal;
using sdl::Input;
using sdl::NextState;
using sdl::Output;
using sdl::Procedure;
using sdl::Process;
using sdl::SdlModel;
using sdl::State;
using sdl::StateMachine;
using sdl::Task;
using sdl::Transition;
using sdl::VariableDeclaration;
using sdl::VariableLiteral;
using sdl::VariableReference;
using sdl::exporter::SdlExporter;
using sdl::exporter::SdlOptions;
using tests::common::SdlInputBuilder;
using tests::common::SdlModelBuilder;
using tests::common::SdlProcessBuilder;
using tests::common::SdlStateBuilder;
using tests::common::SdlStateMachineBuilder;
using tests::common::SdlTaskBuilder;
using tests::common::SdlTransitionBuilder;

namespace tests::Sdl {

class tst_sdlmodel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDefaultValuesInModel();
    void testGenerateProcess();
};

void tst_sdlmodel::testDefaultValuesInModel()
{
    QString processName = "name_of_the_process";

    // clang-format off
    const auto exampleModel =
        SdlModelBuilder("")
            .withProcess(
                SdlProcessBuilder(processName)
                    .withStateMachine(
                        SdlStateMachineBuilder().build()
                    ).build()
            ).build();
    // clang-format on

    QVERIFY(exampleModel->modelType() == ModelType::Sdl);
    const auto *const data = &exampleModel->data();
    QVERIFY(processName == data->name());
}

void verifyAndConsume(QTextStream &stream, const QString &string)
{
    QString line;
    do {
        line = stream.readLine();
        if (line.contains(string)) {
            return;
        }
    } while ((!line.isEmpty() || !line.contains(string)) && !stream.atEnd());

    if (stream.atEnd()) {
        QString message = QString("the generated file does not contain '%1' substring").arg(string);
        QFAIL(message.toStdString().c_str());
        // TODO: abort the test
    }
}

void checkSequenceAndConsume(std::vector<QString> &expectedOutput, QTextStream &consumableOutput)
{
    for (const auto &expectedLine : expectedOutput) {
        verifyAndConsume(consumableOutput, expectedLine);
    }
}

void tst_sdlmodel::testGenerateProcess()
{
    QString modelName = "Example";
    QString modelPrefix = "Sdl_";
    QString processName = "Modemanager";

    auto variable = std::make_unique<VariableDeclaration>("howManyLoops", "MyInteger");

    auto transition1 = SdlTransitionBuilder().withNextStateAction().build();
    auto howManyLoopsRef = std::make_unique<VariableReference>(variable.get());
    auto someInput = SdlInputBuilder()
                             .withName("some_input_name")
                             .withTransition(transition1.get())
                             .withParameter(std::move(howManyLoopsRef))
                             .build();
    auto state1 = SdlStateBuilder("Looping")
                          .withInput(std::move(someInput))
                          .withContinuousSignal(std::make_unique<ContinuousSignal>())
                          .build();

    auto transition2 = SdlTransitionBuilder()
                               .withTask(SdlTaskBuilder().withContents("'EXAMPLE TASK CONTENTS'").build())
                               .withNextStateAction(state1.get())
                               .build();

    auto state2 = SdlStateBuilder("Idle")
                          .withInput(SdlInputBuilder()
                                             .withName("some_other_input_name")
                                             .withTransition(transition2.get())
                                             .build())
                          .withContinuousSignal(std::make_unique<ContinuousSignal>())
                          .build();

    // clang-format off
    const auto exampleModel = SdlModelBuilder(modelName)
        .withProcess(SdlProcessBuilder(processName)
                           .withStateMachine(SdlStateMachineBuilder()
                                                     .withState(std::move(state1))
                                                     .withState(std::move(state2))
                                                     .withTransition(std::move(transition1))
                                                     .withTransition(std::move(transition2))
                                                     .build())
                           .withVariable(std::move(variable)
                           ).build()
        ).build();
    // clang-format on

    Options options;
    options.add(SdlOptions::sdlFilepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, modelName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    std::vector<QString> expectedOutput = {
        "process Modemanager;",
        "dcl howManyLoops MyInteger",
        "START;",
        "NEXTSTATE Looping;",
        "state Looping;",
        "input some_input_name(howManyLoops);",
        // output parameterlessOutput;
        "NEXTSTATE -;",
        "endstate;",
        "state Idle;",
        "input some_other_input_name;",
        "task 'EXAMPLE TASK CONTENTS';",
        // output referenceOutput(howManyLoops);
        "NEXTSTATE Looping;",
        "endstate;",
        "endprocess Modemanager;",
    };
    checkSequenceAndConsume(expectedOutput, consumableOutput);
}

} // namespace tests::sdl

QTEST_MAIN(tests::Sdl::tst_sdlmodel)

#include "tst_sdlexporter.moc"

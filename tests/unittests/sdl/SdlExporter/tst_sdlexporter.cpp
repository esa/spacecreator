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
#include <QStringList>
#include <QtTest>
#include <common/sdlmodelbuilder/sdlanswerbuilder.h>
#include <common/sdlmodelbuilder/sdlblockbuilder.h>
#include <common/sdlmodelbuilder/sdlchannelbuilder.h>
#include <common/sdlmodelbuilder/sdldecisionbuilder.h>
#include <common/sdlmodelbuilder/sdlinputbuilder.h>
#include <common/sdlmodelbuilder/sdlmodelbuilder.h>
#include <common/sdlmodelbuilder/sdloutputbuilder.h>
#include <common/sdlmodelbuilder/sdlprocedurebuilder.h>
#include <common/sdlmodelbuilder/sdlprocedurecallbuilder.h>
#include <common/sdlmodelbuilder/sdlprocessbuilder.h>
#include <common/sdlmodelbuilder/sdlsignalroutebuilder.h>
#include <common/sdlmodelbuilder/sdlstatebuilder.h>
#include <common/sdlmodelbuilder/sdlstatemachinebuilder.h>
#include <common/sdlmodelbuilder/sdlsystembuilder.h>
#include <common/sdlmodelbuilder/sdltaskbuilder.h>
#include <common/sdlmodelbuilder/sdltransitionbuilder.h>
#include <common/textcheckerandconsumer/textcheckerandconsumer.h>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/options.h>
#include <memory>
#include <qtestcase.h>
#include <sdl/SdlExporter/exporter.h>
#include <sdl/SdlModel/action.h>
#include <sdl/SdlModel/expression.h>
#include <sdl/SdlModel/join.h>
#include <sdl/SdlModel/label.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/output.h>
#include <sdl/SdlModel/procedure.h>
#include <sdl/SdlModel/procedurecall.h>
#include <sdl/SdlModel/procedureparameter.h>
#include <sdl/SdlModel/process.h>
#include <sdl/SdlModel/return.h>
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/statemachine.h>
#include <sdl/SdlModel/task.h>
#include <sdl/SdlModel/variabledeclaration.h>
#include <sdl/SdlModel/variableliteral.h>
#include <sdl/SdlModel/variablereference.h>
#include <sdl/SdlOptions/options.h>

using conversion::ModelType;
using conversion::Options;
using conversion::exporter::ExportException;
using conversion::sdl::SdlOptions;
using sdl::Action;
using sdl::Answer;
using sdl::ContinuousSignal;
using sdl::Decision;
using sdl::Expression;
using sdl::Input;
using sdl::Join;
using sdl::Label;
using sdl::NextState;
using sdl::Output;
using sdl::Procedure;
using sdl::ProcedureParameter;
using sdl::Process;
using sdl::Return;
using sdl::SdlModel;
using sdl::State;
using sdl::StateMachine;
using sdl::Task;
using sdl::Transition;
using sdl::VariableDeclaration;
using sdl::VariableLiteral;
using sdl::VariableReference;
using sdl::exporter::SdlExporter;
using tests::common::SdlAnswerBuilder;
using tests::common::SdlBlockBuilder;
using tests::common::SdlChannelBuilder;
using tests::common::SdlDecisionBuilder;
using tests::common::SdlInputBuilder;
using tests::common::SdlModelBuilder;
using tests::common::SdlOutputBuilder;
using tests::common::SdlProcedureBuilder;
using tests::common::SdlProcedureCallBuilder;
using tests::common::SdlProcessBuilder;
using tests::common::SdlSignalRouteBuilder;
using tests::common::SdlStateBuilder;
using tests::common::SdlStateMachineBuilder;
using tests::common::SdlSystemBuilder;
using tests::common::SdlTaskBuilder;
using tests::common::SdlTransitionBuilder;
using tests::common::TextCheckerAndConsumer;

namespace tests::Sdl {

class tst_sdlexporter : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDefaultValuesInModel();
    void testGenerateBasicProcess();
    void testGenerateProcessWithDeclarationsAndTasks();
    void testGenerateProcessWithLabelAndJoin();
    void testJoinWithoutSpecifiedLabel();
    void testGenerateProcessWithDecisionExpressionAndAnswer();
    void testGenerateProcessWithParamlessProcedure();
    void testGenerateProcessWithProcedureWithParamsAndReturn();
    void testGenerateProcessWithReturnlessProcedure();
    void testGenerateSystem();
    void testGenerateSystemAutoRoutes();
};

static std::unique_ptr<VariableDeclaration> makeVariableDeclaration(QString name, QString type)
{
    auto variable = std::make_unique<VariableDeclaration>();
    variable->setName(std::move(name));
    variable->setType(std::move(type));
    variable->setMonitor(false);

    return variable;
}

static std::unique_ptr<VariableDeclaration> makeMonitorDeclaration(QString name, QString type)
{
    auto variable = std::make_unique<VariableDeclaration>();
    variable->setName(std::move(name));
    variable->setType(std::move(type));
    variable->setMonitor(true);

    return variable;
}

static std::unique_ptr<ProcedureParameter> makeProcedureParameter(QString name, QString type, QString direction)
{
    auto parameter = std::make_unique<ProcedureParameter>();
    parameter->setName(std::move(name));
    parameter->setType(std::move(type));
    parameter->setDirection(std::move(direction));

    return parameter;
}

void tst_sdlexporter::testDefaultValuesInModel()
{
    QString processName = "exampleProcess";

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
    QVERIFY(!exampleModel->processes().empty());
    const auto *const process = &exampleModel->processes()[0];
    QVERIFY(processName == process->name());
}

void tst_sdlexporter::testGenerateBasicProcess()
{
    QString modelName = "BasicProcess";
    QString modelPrefix = "Sdl_";
    QString processName = modelName.toLower(); // NOLINT

    auto transition1 = SdlTransitionBuilder().withNextStateAction().build();
    auto state1 = SdlStateBuilder("Wait")
                          .withInput(SdlInputBuilder().withName("someInput").withTransition(transition1.get()).build())
                          .build();

    auto startTransition = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto transition2 = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto state2 =
            SdlStateBuilder("Idle")
                    .withInput(SdlInputBuilder().withName("someOtherInput").withTransition(transition2.get()).build())
                    .build();

    // clang-format off
    const auto exampleModel = SdlModelBuilder(modelName)
        .withProcess(SdlProcessBuilder(processName)
                           .withStartTransition(std::move(startTransition))
                           .withStateMachine(SdlStateMachineBuilder()
                                                     .withState(std::move(state1))
                                                     .withState(std::move(state2))
                                                     .withTransition(std::move(transition1))
                                                     .withTransition(std::move(transition2))
                                                     .build()
                           ).build()
        ).build();
    // clang-format on

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, processName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    QStringList expectedOutput = {
        QString("process %1;").arg(processName),

        "START;",
        "NEXTSTATE Wait;",

        "state Wait;",
        "input someInput;",
        "NEXTSTATE -;",
        "endstate;",

        "state Idle;",
        "input someOtherInput;",
        "NEXTSTATE Wait;",
        "endstate;",

        QString("endprocess %1;").arg(processName),
    };
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlexporter::testGenerateProcessWithDeclarationsAndTasks()
{
    QString modelName = "DeclarationsAndTasks";
    QString modelPrefix = "Sdl_";
    QString processName = modelName.toLower(); // NOLINT

    auto variable = makeVariableDeclaration("howManyLoops", "MyInteger");

    auto transition1 = SdlTransitionBuilder()
                               .withOutput(SdlOutputBuilder().withName("parameterlessOutput").build())
                               .withNextStateAction()
                               .build();
    auto someInput = SdlInputBuilder()
                             .withName("some_input_name")
                             .withTransition(transition1.get())
                             .withParameter(std::make_unique<VariableReference>(variable->name()))
                             .build();
    auto state1 = SdlStateBuilder("Looping")
                          .withInput(std::move(someInput))
                          .withContinuousSignal(std::make_unique<ContinuousSignal>())
                          .build();

    auto referenceOutput = SdlOutputBuilder()
                                   .withName("referenceOutput")
                                   .withParameter(std::make_unique<VariableReference>(variable->name()))
                                   .build();

    auto transition2 = SdlTransitionBuilder()
                               .withTask(SdlTaskBuilder().withContents("'EXAMPLE TASK CONTENTS'").build())
                               .withOutput(std::move(referenceOutput))
                               .withNextStateAction(state1.get())
                               .build();

    auto startTransition = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto state2 = SdlStateBuilder("Idle")
                          .withInput(SdlInputBuilder()
                                             .withName("some_other_input_name")
                                             .withTransition(transition2.get())
                                             .build())
                          .withContinuousSignal(std::make_unique<ContinuousSignal>())
                          .build();

    auto process = SdlProcessBuilder(processName)
                           .withStartTransition(std::move(startTransition))
                           .withVariable(std::move(variable))
                           .withStateMachine(SdlStateMachineBuilder()
                                                     .withState(std::move(state1))
                                                     .withState(std::move(state2))
                                                     .withTransition(std::move(transition1))
                                                     .withTransition(std::move(transition2))
                                                     .build())
                           .build();

    const auto exampleModel = SdlModelBuilder(modelName).withProcess(std::move(process)).build();

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, processName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    QStringList expectedOutput = {
        QString("process %1;").arg(processName),

        "dcl howManyLoops MyInteger",

        "START;",
        "NEXTSTATE Looping;",

        "state Looping;",
        "input some_input_name(howManyLoops);",
        "output parameterlessOutput;",
        "NEXTSTATE -;",
        "endstate;",

        "state Idle;",
        "input some_other_input_name;",
        "task 'EXAMPLE TASK CONTENTS';",
        "output referenceOutput(howManyLoops);",
        "NEXTSTATE Looping;",
        "endstate;",

        QString("endprocess %1;").arg(processName),
    };
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlexporter::testGenerateProcessWithLabelAndJoin()
{
    QString modelName = "LabelAndJoin";
    QString modelPrefix = "Sdl_";
    QString processName = modelName.toLower(); // NOLINT

    auto fromStartLabel = std::make_unique<Label>("fromStart");

    auto joinFromStart = std::make_unique<Join>();
    joinFromStart->setLabel("fromStart");
    auto transition = SdlTransitionBuilder() //
                              .withAction(std::move(joinFromStart))
                              .build();
    auto state1 = SdlStateBuilder("Idle")
                          .withInput(SdlInputBuilder().withName("sigReset").withTransition(transition.get()).build())
                          .build();

    auto process = SdlProcessBuilder(processName)
                           .withStartTransition(SdlTransitionBuilder()
                                                        .withAction(std::move(fromStartLabel))
                                                        .withAction(std::make_unique<NextState>("", state1.get()))
                                                        .build())
                           .withStateMachine(SdlStateMachineBuilder()
                                                     .withState(std::move(state1))
                                                     .withTransition(std::move(transition))
                                                     .build())
                           .build();

    const auto exampleModel = SdlModelBuilder(modelName).withProcess(std::move(process)).build();

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, processName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    QStringList expectedOutput = {
        QString("process %1;").arg(processName),

        "START;",
        "fromStart:",
        "NEXTSTATE Idle;",

        "state Idle;",
        "input sigReset;",
        "join fromStart;",
        "endstate;",

        QString("endprocess %1;").arg(processName),
    };
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlexporter::testGenerateProcessWithDecisionExpressionAndAnswer()
{
    QString modelName = "DecisionExpressionAndAnswer";
    QString modelPrefix = "Sdl_";
    QString processName = modelName.toLower(); // NOLINT

    auto variableX = makeVariableDeclaration("x", "MyInteger");

    auto transition =
            SdlTransitionBuilder()
                    .withAction(
                            SdlDecisionBuilder()
                                    .withExpression(std::make_unique<Expression>("x"))
                                    .withAnswer(SdlAnswerBuilder()
                                                        .withName("firstAnswer")
                                                        .withLiteral(VariableLiteral("1"))
                                                        .withTransition(
                                                                SdlTransitionBuilder().withNextStateAction().build())
                                                        .build())
                                    .withAnswer(
                                            SdlAnswerBuilder()
                                                    .withName("secondAnswer")
                                                    .withLiteral(VariableLiteral(">1"))
                                                    .withTransition(
                                                            SdlTransitionBuilder()
                                                                    .withOutput(SdlOutputBuilder()
                                                                                        .withName("sendOutput")
                                                                                        .withParameter(std::make_unique<
                                                                                                VariableReference>(
                                                                                                variableX->name()))
                                                                                        .build())
                                                                    .withTask(
                                                                            SdlTaskBuilder()
                                                                                    .withContents("'SOME EXAMPLE TASK'")
                                                                                    .build())
                                                                    .withAction(std::make_unique<NextState>(""))
                                                                    .build())
                                                    .build())
                                    .withAnswer(
                                            SdlAnswerBuilder()
                                                    .withLiteral(sdl::Answer::ElseLiteral)
                                                    .withTransition(
                                                            SdlTransitionBuilder()
                                                                    .withTask(SdlTaskBuilder()
                                                                                      .withContents("'ANSWER UNKNOWN'")
                                                                                      .build())
                                                                    .withAction(std::make_unique<NextState>(""))
                                                                    .build())
                                                    .build())
                                    .build())
                    .build();

    auto waitState = SdlStateBuilder("Wait")
                             .withInput(SdlInputBuilder()
                                                .withName("startProcess")
                                                .withParameter(std::make_unique<VariableReference>(variableX->name()))
                                                .withTransition(transition.get())
                                                .build())
                             .build();

    // clang-format off
    const auto exampleModel = SdlModelBuilder(modelName)
        .withProcess(
            SdlProcessBuilder(processName)
                .withStartTransition(
                    SdlTransitionBuilder()
                        .withAction(std::make_unique<NextState>("", waitState.get())).build())
                .withStateMachine(
                    SdlStateMachineBuilder()
                        .withState(std::move(waitState))
                        .withTransition(std::move(transition))
                    .build())
                .withVariable(std::move(variableX)).build())
            .build();
    // clang-format on

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, processName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    QStringList expectedOutput = {
        QString("process %1;").arg(processName),

        "dcl x MyInteger;",

        "START;",
        "NEXTSTATE Wait;",

        "state Wait;",
        "input startProcess(x);",
        "decision x;",
        "(1)",
        "NEXTSTATE -;",
        "(>1)",
        "output sendOutput(x);",
        "task 'SOME EXAMPLE TASK';",
        "NEXTSTATE -;",
        "ELSE:",
        "task 'ANSWER UNKNOWN';",
        "NEXTSTATE -;",
        "enddecision;",
        "endstate;",

        QString("endprocess %1;").arg(processName),
    };
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlexporter::testGenerateProcessWithParamlessProcedure()
{
    QString modelName = "ParamlessProcedure";
    QString modelPrefix = "Sdl_";
    QString processName = modelName.toLower(); // NOLINT

    auto procedure =
            SdlProcedureBuilder()
                    .withName("myParamlessProcedure")
                    .withTransition(
                            SdlTransitionBuilder()
                                    .withAction(SdlTaskBuilder().withContents("'TASK INSIDE PROCEDURE'").build())
                                    .withAction(SdlTaskBuilder().withContents("'SECOND TASK INSIDE PROCEDURE'").build())
                                    .build())
                    .build();

    auto transition = SdlTransitionBuilder()
                              .withAction(SdlProcedureCallBuilder().withProcedure(procedure.get()).build())
                              .withNextStateAction()
                              .build();
    auto someInput = SdlInputBuilder().withName("startProcess").withTransition(transition.get()).build();
    auto state = SdlStateBuilder("Wait")
                         .withInput(std::move(someInput))
                         .withContinuousSignal(std::make_unique<ContinuousSignal>())
                         .build();

    auto process = SdlProcessBuilder(processName)
                           .withProcedure(std::move(procedure))
                           .withStartTransition(SdlTransitionBuilder().withNextStateAction(state.get()).build())
                           .withStateMachine(SdlStateMachineBuilder()
                                                     .withState(std::move(state))
                                                     .withTransition(std::move(transition))
                                                     .build())
                           .build();

    const auto exampleModel = SdlModelBuilder(modelName).withProcess(std::move(process)).build();

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, processName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    QStringList expectedOutput = {
        QString("process %1;").arg(processName),

        "procedure myParamlessProcedure;",
        "START;",
        "task 'TASK INSIDE PROCEDURE';",
        "task 'SECOND TASK INSIDE PROCEDURE';",
        "endprocedure;",

        "START;",
        "NEXTSTATE Wait;",

        "state Wait;",
        "input startProcess;",
        "call myParamlessProcedure;",
        "NEXTSTATE -;",

        "endstate;",

        QString("endprocess %1;").arg(processName),
    };
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlexporter::testJoinWithoutSpecifiedLabel()
{
    QString modelName = "JoinWithoutSpecifiedLabel";
    QString modelPrefix = "Sdl_";
    QString processName = modelName.toLower(); // NOLINT

    auto join = std::make_unique<Join>();
    auto transition = SdlTransitionBuilder() //
                              .withAction(std::move(join))
                              .build();
    auto state = SdlStateBuilder("Idle")
                         .withInput(SdlInputBuilder().withName("sigReset").withTransition(transition.get()).build())
                         .build();

    auto process =
            SdlProcessBuilder(processName)
                    .withStartTransition(
                            SdlTransitionBuilder().withAction(std::make_unique<NextState>("", state.get())).build())
                    .withStateMachine(SdlStateMachineBuilder()
                                              .withState(std::move(state))
                                              .withTransition(std::move(transition))
                                              .build())
                    .build();

    const auto exampleModel = SdlModelBuilder(modelName).withProcess(std::move(process)).build();

    Options options;
    SdlExporter exporter;
    QVERIFY_EXCEPTION_THROWN(exporter.exportModel(exampleModel.get(), options), ExportException);
}

void tst_sdlexporter::testGenerateProcessWithProcedureWithParamsAndReturn()
{
    QString modelName = "ProcedureWithParamsAndReturn";
    QString modelPrefix = "Sdl_";
    QString processName = modelName.toLower(); // NOLINT

    auto returnVariable = makeVariableDeclaration("ret", "MyInteger");

    auto procedure = SdlProcedureBuilder()
                             .withName("myProcedure")
                             .withReturnType(returnVariable->type())
                             .withVariable(std::move(returnVariable))
                             .withParameter(makeProcedureParameter("a", "MyInteger", "in/out"))
                             .withParameter(makeProcedureParameter("b", "MyInteger", "in"))
                             .withTransition(SdlTransitionBuilder()
                                                     .withAction(SdlTaskBuilder().withContents("ret := a + b;").build())
                                                     .withAction(SdlTaskBuilder().withContents("a := a + 1;").build())
                                                     .withAction(std::make_unique<Return>("ret"))
                                                     .build())
                             .build();

    auto transition = SdlTransitionBuilder()
                              .withAction(SdlTaskBuilder().withContents("y := call myProcedure(x, 2);").build())
                              .withNextStateAction()
                              .build();

    auto variableX = makeVariableDeclaration("x", "MyInteger");
    auto variableY = makeVariableDeclaration("y", "MyInteger");

    auto state = SdlStateBuilder("Wait")
                         .withInput(SdlInputBuilder()
                                            .withName("startProcess")
                                            .withTransition(transition.get())
                                            .withParameter(std::make_unique<VariableReference>(variableX->name()))
                                            .build())
                         .build();

    auto process = SdlProcessBuilder(processName)
                           .withVariable(std::move(variableX))
                           .withVariable(std::move(variableY))
                           .withProcedure(std::move(procedure))
                           .withStartTransition(SdlTransitionBuilder().withNextStateAction(state.get()).build())
                           .withStateMachine(SdlStateMachineBuilder()
                                                     .withState(std::move(state))
                                                     .withTransition(std::move(transition))
                                                     .build())
                           .build();

    const auto exampleModel = SdlModelBuilder(modelName).withProcess(std::move(process)).build();

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, processName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);

    QStringList expectedOutput = {
        QString("process %1;").arg(processName),

        "dcl x MyInteger;",
        "dcl y MyInteger;",

        "procedure myProcedure;",
        "dcl ret MyInteger;",
        "fpar",
        "in/out a MyInteger,",
        "in b MyInteger;",
        "returns MyInteger;",
        "START;",
        "task ret := a + b;",
        "task a := a + 1;",
        "return ret;",
        "endprocedure;",

        "START;",
        "NEXTSTATE Wait;",

        "state Wait;",
        "input startProcess(x);",
        "task y := call myProcedure(x, 2);",
        "NEXTSTATE -;",
        "endstate;",

        QString("endprocess %1;").arg(processName),
    };
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlexporter::testGenerateProcessWithReturnlessProcedure()
{
    QString modelName = "ReturnlessProcedure";
    QString modelPrefix = "Sdl_";
    QString processName = modelName.toLower(); // NOLINT

    auto variableX = makeVariableDeclaration("x", "MyInteger");

    auto procedure = SdlProcedureBuilder()
                             .withName("returnlessProcedure")
                             .withParameter(makeProcedureParameter("a", "MyInteger", "in"))
                             .withParameter(makeProcedureParameter("b", "MyInteger", "in"))
                             .withTransition(SdlTransitionBuilder()
                                                     .withAction(SdlTaskBuilder().withContents("'EXAMPLE'").build())
                                                     .build())
                             .build();

    auto transition = SdlTransitionBuilder()
                              .withAction(SdlProcedureCallBuilder()
                                                  .withProcedure(procedure.get())
                                                  .withArgument(std::make_unique<VariableLiteral>("2"))
                                                  .withArgument(std::make_unique<VariableReference>(variableX->name()))
                                                  .build())
                              .withNextStateAction()
                              .build();

    auto state = SdlStateBuilder("Wait")
                         .withInput(SdlInputBuilder()
                                            .withName("startProcess")
                                            .withParameter(std::make_unique<VariableReference>(variableX->name()))
                                            .withTransition(transition.get())
                                            .build())
                         .build();

    auto process = SdlProcessBuilder(processName)
                           .withVariable(std::move(variableX))
                           .withProcedure(std::move(procedure))
                           .withStartTransition(SdlTransitionBuilder().withNextStateAction(state.get()).build())
                           .withStateMachine(SdlStateMachineBuilder()
                                                     .withState(std::move(state))
                                                     .withTransition(std::move(transition))
                                                     .build())
                           .build();

    const auto exampleModel = SdlModelBuilder(modelName).withProcess(std::move(process)).build();

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, processName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);

    QStringList expectedOutput = {
        QString("process %1;").arg(processName),

        "dcl x MyInteger;",

        "procedure returnlessProcedure;",
        "fpar",
        "in a MyInteger,",
        "in b MyInteger;",
        "START;",
        "task 'EXAMPLE'",
        "endprocedure;",

        "START;",
        "NEXTSTATE Wait;",

        "state Wait;",
        "input startProcess(x);",
        "call returnlessProcedure(2, x);",
        "NEXTSTATE -;",
        "endstate;",

        QString("endprocess %1;").arg(processName),
    };
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlexporter::testGenerateSystem()
{
    QString modelName = "BasicSystem";
    QString modelPrefix = "Sdl_";
    QString systemName = modelName.toLower(); // NOLINT

    auto monitor = makeMonitorDeclaration("event", "Observable_Event");

    auto transition1 = SdlTransitionBuilder().withNextStateAction().build();
    auto state1 = SdlStateBuilder("Wait")
                          .withInput(SdlInputBuilder().withName("someInput").withTransition(transition1.get()).build())
                          .build();

    auto startTransition = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto transition2 = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto state2 =
            SdlStateBuilder("Idle")
                    .withInput(SdlInputBuilder().withName("someOtherInput").withTransition(transition2.get()).build())
                    .build();

    // clang-format off
    const auto exampleModel = SdlModelBuilder(systemName)
        .withSystem(SdlSystemBuilder(systemName)
            .withFreeformText("use datamodel comment 'observer.asn'")
            .withSignal("Signal1")
            .withInputRename("Signal2", "OGSignal2", "Func2")
            .withOutputRename("Signal3", "OGSignal3", "Func3")
            .withChannel(SdlChannelBuilder("c")
                .withRoute("env", systemName, {"Signal1", "Signal2", "Signal3" })
                .build())
            .withBlock(SdlBlockBuilder(systemName)
                .withSignalRoute(SdlSignalRouteBuilder("r")
                    .withRoute("env", systemName, {"Signal1", "Signal2", "Signal3" })
                    .build())
                .withConnection("c", "r")
                .withProcess(SdlProcessBuilder(systemName)
                    .withStartTransition(std::move(startTransition))
                    .withVariable(std::move(monitor))
                    .withStateMachine(SdlStateMachineBuilder()
                        .withState(std::move(state1))
                        .withState(std::move(state2))
                        .withTransition(std::move(transition1))
                        .withTransition(std::move(transition2))
                        .build())
                    .build())
                .build())
            .build())
        .build();
    // clang-format on

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, systemName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    // clang-format off
    QStringList expectedOutput = {
        QString("system %1;").arg(systemName),
        "use datamodel comment 'observer.asn';",
        "signal Signal1;",
        "signal Signal2 renames input OGSignal2 to Func2;",
        "signal Signal3 renames output OGSignal3 from Func3;",
        "channel c",
        QString("from env to %1 with Signal1, Signal2, Signal3").arg(systemName),
        "endchannel;",
        QString("block %1;").arg(systemName),
        "signalroute r",
        QString("from env to %1 with Signal1, Signal2, Signal3;").arg(systemName),
        "connect c and r;",
        QString("process %1;").arg(systemName),
        "monitor event Observable_Event",
        "START;", "NEXTSTATE Wait;",
        "state Wait;", "input someInput;", "NEXTSTATE -;", "endstate;",
        "state Idle;", "input someOtherInput;", "NEXTSTATE Wait;", "endstate;",
        QString("endprocess %1;").arg(systemName), "endblock;", "endsystem;"
    };
    // clang-format on
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlexporter::testGenerateSystemAutoRoutes()
{
    QString modelName = "BasicSystem";
    QString modelPrefix = "Sdl_";
    QString systemName = modelName.toLower(); // NOLINT

    auto monitor = makeMonitorDeclaration("event", "Observable_Event");

    auto transition1 = SdlTransitionBuilder().withNextStateAction().build();
    auto state1 = SdlStateBuilder("Wait")
                          .withInput(SdlInputBuilder().withName("someInput").withTransition(transition1.get()).build())
                          .build();

    auto startTransition = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto transition2 = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto state2 =
            SdlStateBuilder("Idle")
                    .withInput(SdlInputBuilder().withName("someOtherInput").withTransition(transition2.get()).build())
                    .build();

    // clang-format off
    auto exampleSystem = SdlSystemBuilder(systemName)
        .withFreeformText("use datamodel comment 'observer.asn'")
        .withSignal("Signal1")
        .withInputRename("Signal2", "OGSignal2", "Func2")
        .withOutputRename("Signal3", "OGSignal3", "Func3")
        .withBlock(SdlBlockBuilder(systemName)
            .withProcess(SdlProcessBuilder(systemName)
                .withStartTransition(std::move(startTransition))
                .withVariable(std::move(monitor))
                .withStateMachine(SdlStateMachineBuilder()
                    .withState(std::move(state1))
                    .withState(std::move(state2))
                    .withTransition(std::move(transition1))
                    .withTransition(std::move(transition2))
                    .build())
                .build())
            .build())
        .build();
    exampleSystem.createRoutes("c", "r");
    // clang-format on

    const auto exampleModel = SdlModelBuilder(systemName).withSystem(std::move(exampleSystem)).build();

    Options options;
    options.add(SdlOptions::filepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(exampleModel.get(), options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, systemName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    // clang-format off
    QStringList expectedOutput = {
        QString("system %1;").arg(systemName),
        "use datamodel comment 'observer.asn';",
        "signal Signal1;",
        "signal Signal2 renames input OGSignal2 to Func2;",
        "signal Signal3 renames output OGSignal3 from Func3;",
        " channel c",
        QString("from env to %1 with Signal1, Signal2, Signal3").arg(systemName),
        "endchannel;",
        QString("block %1;").arg(systemName),
        "signalroute r",
        QString("from env to %1 with Signal1, Signal2, Signal3;").arg(systemName),
        "connect c and r;",
        QString("process %1;").arg(systemName),
        "monitor event Observable_Event",
        "START;", "NEXTSTATE Wait;",
        "state Wait;", "input someInput;", "NEXTSTATE -;", "endstate;",
        "state Idle;", "input someOtherInput;", "NEXTSTATE Wait;", "endstate;",
        QString("endprocess %1;").arg(systemName), "endblock;", "endsystem;"
    };
    // clang-format on
    TextCheckerAndConsumer::checkSequenceAndConsume(expectedOutput, consumableOutput);
}

} // namespace tests::sdl

QTEST_MAIN(tests::Sdl::tst_sdlexporter)

#include "tst_sdlexporter.moc"

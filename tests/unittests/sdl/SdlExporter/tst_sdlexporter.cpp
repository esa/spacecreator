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

#include <QObject>
#include <QtTest>
#include <common/sdlmodelbuilder/sdlanswerbuilder.h>
#include <common/sdlmodelbuilder/sdldecisionbuilder.h>
#include <common/sdlmodelbuilder/sdlinputbuilder.h>
#include <common/sdlmodelbuilder/sdlmodelbuilder.h>
#include <common/sdlmodelbuilder/sdloutputbuilder.h>
#include <common/sdlmodelbuilder/sdlprocedurebuilder.h>
#include <common/sdlmodelbuilder/sdlprocedurecallbuilder.h>
#include <common/sdlmodelbuilder/sdlprocessbuilder.h>
#include <common/sdlmodelbuilder/sdlstatebuilder.h>
#include <common/sdlmodelbuilder/sdlstatemachinebuilder.h>
#include <common/sdlmodelbuilder/sdltaskbuilder.h>
#include <common/sdlmodelbuilder/sdltransitionbuilder.h>
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
#include <sdl/SdlModel/sdlmodel.h>
#include <sdl/SdlModel/task.h>
#include <sdl/SdlModel/variabledeclaration.h>
#include <sdl/SdlModel/variableliteral.h>
#include <sdl/SdlModel/variablereference.h>
#include <sdl/SdlOptions/options.h>

using conversion::ModelType;
using conversion::Options;
using conversion::exporter::ExportException;
using sdl::Action;
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
using tests::common::SdlAnswerBuilder;
using tests::common::SdlDecisionBuilder;
using tests::common::SdlInputBuilder;
using tests::common::SdlModelBuilder;
using tests::common::SdlOutputBuilder;
using tests::common::SdlProcedureBuilder;
using tests::common::SdlProcedureCallBuilder;
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
    void testGenerateProcessWithTasksVariablesAndParameters();
    void testGenerateProcessWithLabelAndJoin();
    void testJoinWithoutSpecifiedLabel();
    void testGenerateProcessWithDecisionExpressionAndAnswer();
    void testGenerateProcessWithParamlessProcedure();
    void testGenerateProcessWithProcedureWithParamsAndReturn();
    void testGenerateProcessWithReturnlessProcedure();
};

std::unique_ptr<VariableDeclaration> makeVariableDeclaration(QString name, QString type)
{
    auto variable = std::make_unique<VariableDeclaration>();
    variable->setName(std::move(name));
    variable->setType(std::move(type));

    return variable;
}

std::unique_ptr<ProcedureParameter> makeProcedureParameter(QString name, QString type, QString direction)
{
    auto parameter = std::make_unique<ProcedureParameter>();
    parameter->setName(std::move(name));
    parameter->setType(std::move(type));
    parameter->setDirection(std::move(direction));

    return parameter;
}

bool verifyAndConsume(QTextStream &stream, const QString &string)
{
    QString line;
    do {
        line = stream.readLine();
        if (line.contains(string)) {
            return true;
        }
    } while ((!line.isEmpty() || !line.contains(string)) && !stream.atEnd());

    return false;
}

void checkSequenceAndConsume(std::vector<QString> &expectedOutput, QTextStream &consumableOutput)
{
    for (const auto &expectedLine : expectedOutput) {
        if (verifyAndConsume(consumableOutput, expectedLine)) {
            continue;
        } else {
            QString message = QString("the generated file does not contain '%1' substring").arg(expectedLine);
            QFAIL(message.toStdString().c_str());
        }
    }
}

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

void tst_sdlmodel::testGenerateProcess()
{
    QString modelName = "BasicProcess";
    QString modelPrefix = "Sdl_";
    QString processName = "Exampleprocess";

    auto transition1 = SdlTransitionBuilder().withNextStateAction().build();
    auto someInput = SdlInputBuilder().withName("some_input_name").withTransition(transition1.get()).build();
    auto state1 = SdlStateBuilder("Looping").withInput(std::move(someInput)).build();

    auto startTransition = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto referenceOutput = SdlOutputBuilder().withName("referenceOutput").build();

    auto transition2 = SdlTransitionBuilder().withNextStateAction(state1.get()).build();

    auto state2 = SdlStateBuilder("Idle")
                          .withInput(SdlInputBuilder()
                                             .withName("some_other_input_name")
                                             .withTransition(transition2.get())
                                             .build())
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
        "process Exampleprocess;",

        "START;",
        "NEXTSTATE Looping;",

        "state Looping;",
        "input some_input_name;",
        "NEXTSTATE -;",
        "endstate;",

        "state Idle;",
        "input some_other_input_name;",
        "NEXTSTATE Looping;",
        "endstate;",

        "endprocess Exampleprocess;",
    };
    checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlmodel::testGenerateProcessWithTasksVariablesAndParameters()
{
    QString modelName = "TasksVariablesAndParameters";
    QString modelPrefix = "Sdl_";
    QString processName = "Exampleprocess";

    auto variable = makeVariableDeclaration("howManyLoops", "MyInteger");
    auto variableReference = VariableReference(variable.get());

    auto transition1 = SdlTransitionBuilder()
                               .withOutput(SdlOutputBuilder().withName("parameterlessOutput").build())
                               .withNextStateAction()
                               .build();
    auto someInput = SdlInputBuilder()
                             .withName("some_input_name")
                             .withTransition(transition1.get())
                             .withParameter(&variableReference)
                             .build();
    auto state1 = SdlStateBuilder("Looping")
                          .withInput(std::move(someInput))
                          .withContinuousSignal(std::make_unique<ContinuousSignal>())
                          .build();

    auto referenceOutput = SdlOutputBuilder().withName("referenceOutput").withParameter(&variableReference).build();

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
        "process Exampleprocess;",

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

        "endprocess Exampleprocess;",
    };
    checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlmodel::testGenerateProcessWithLabelAndJoin()
{
    QString modelName = "LabelAndJoin";
    QString modelPrefix = "Sdl_";
    QString processName = "Exampleprocess";

    auto fromStartLabel = std::make_unique<Label>("fromStart");

    auto joinFromStart = std::make_unique<Join>();
    joinFromStart->setLabel(fromStartLabel.get());
    auto transition = SdlTransitionBuilder() //
                              .withAction(std::move(joinFromStart))
                              .withAction(std::make_unique<NextState>(""))
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
        "process Exampleprocess;",

        "START;",
        "fromStart:",
        "NEXTSTATE Idle;",

        "state Idle;",
        "input sigReset;",
        "join fromStart;",
        "endstate;",

        "endprocess Exampleprocess;",
    };
    checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlmodel::testGenerateProcessWithDecisionExpressionAndAnswer()
{
    QString modelName = "DecisionExpressionAndAnswer";
    QString modelPrefix = "Sdl_";
    QString processName = "Exampleprocess";

    auto variableX = makeVariableDeclaration("x", "MyInteger");
    auto variableXRef = VariableReference(variableX.get());

    auto transition =
            SdlTransitionBuilder()
                    .withNextStateAction()
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
                                                                                        .withParameter(&variableXRef)
                                                                                        .build())
                                                                    .withTask(
                                                                            SdlTaskBuilder()
                                                                                    .withContents("'SOME EXAMPLE TASK'")
                                                                                    .build())
                                                                    .build())

                                                    .build())
                                    .withAnswer(
                                            SdlAnswerBuilder()
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
                                                .withParameter(&variableXRef)
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
        "process Exampleprocess;",

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
        "else:",
        "task 'ANSWER UNKNOWN';",
        "NEXTSTATE -;",
        "enddecision;",
        "endstate;",

        "endprocess Exampleprocess",
    };
    checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlmodel::testGenerateProcessWithParamlessProcedure()
{
    QString modelName = "ParamlessProcedure";
    QString modelPrefix = "Sdl_";
    QString processName = "Exampleprocess";

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
                              .withOutput(SdlOutputBuilder().withName("parameterlessOutput").build())
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
        "process Exampleprocess;",

        "procedure myParamlessProcedure;",
        "START;",
        "task 'TASK INSIDE PROCEDURE';",
        "task 'SECOND TASK INSIDE PROCEDURE';",
        "return ;",
        "endprocedure;",

        "START;",
        "NEXTSTATE Wait;",

        "state Wait;",
        "input startProcess;",
        "call myParamlessProcedure;",
        "NEXTSTATE -;",

        "endstate;",
        "endprocess Exampleprocess;",
    };
    checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlmodel::testJoinWithoutSpecifiedLabel()
{
    QString modelName = "JoinWithoutSpecifiedLabel";
    QString modelPrefix = "Sdl_";
    QString processName = "Exampleprocess";

    auto join = std::make_unique<Join>();
    auto transition = SdlTransitionBuilder() //
                              .withAction(std::move(join))
                              .withAction(std::make_unique<NextState>(""))
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

void tst_sdlmodel::testGenerateProcessWithProcedureWithParamsAndReturn()
{
    QString modelName = "Procedure";
    QString modelPrefix = "Sdl_";
    QString processName = "Exampleprocess";

    auto procedure = SdlProcedureBuilder()
                             .withName("myProcedure")
                             .withParameter(makeProcedureParameter("a", "MyInteger", "in/out"))
                             .withParameter(makeProcedureParameter("b", "MyInteger", "in"))
                             .withReturnVariableDeclaration(makeVariableDeclaration("ret", "MyInteger"))
                             .withTransition(SdlTransitionBuilder()
                                                     .withAction(SdlTaskBuilder().withContents("ret := a + b;").build())
                                                     .withAction(SdlTaskBuilder().withContents("a := a + 1;").build())
                                                     .build())
                             .build();

    auto transition = SdlTransitionBuilder()
                              .withAction(SdlTaskBuilder().withContents("y := call myProcedure(x, 2);").build())
                              .withNextStateAction()
                              .build();

    auto variableX = makeVariableDeclaration("x", "MyInteger");
    VariableReference varXRef(variableX.get());
    auto variableY = makeVariableDeclaration("y", "MyInteger");
    VariableReference varYRef(variableY.get());

    auto state = SdlStateBuilder("Wait")
                         .withInput(SdlInputBuilder()
                                            .withName("startProcess")
                                            .withTransition(transition.get())
                                            .withParameter(&varXRef)
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
        "process Exampleprocess;",

        "dcl x MyInteger;",
        "dcl y MyInteger;",

        "procedure myProcedure;",
        "fpar",
        "in/out a MyInteger,",
        "in b MyInteger;",
        "returns MyInteger;",
        "dcl ret MyInteger;",
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

        "endprocess Exampleprocess;",
    };
    checkSequenceAndConsume(expectedOutput, consumableOutput);
}

void tst_sdlmodel::testGenerateProcessWithReturnlessProcedure()
{
    QString modelName = "ReturnlessProcedure";
    QString modelPrefix = "Sdl_";
    QString processName = "Exampleprocess";

    auto variableX = makeVariableDeclaration("x", "MyInteger");
    VariableReference varXRef(variableX.get());

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
                                                  .withArgument(VariableLiteral("2"))
                                                  .withArgument(&varXRef)
                                                  .build())
                              .withNextStateAction()
                              .build();

    auto state = SdlStateBuilder("Wait")
                         .withInput(SdlInputBuilder()
                                            .withName("startProcess")
                                            .withParameter(&varXRef)
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
        "process Exampleprocess;",

        "dcl x MyInteger;",

        "procedure returnlessProcedure;",
        "fpar",
        "in a MyInteger,",
        "in b MyInteger;",
        "START;",
        "task 'EXAMPLE'",
        "return ;",
        "endprocedure;",

        "START;",
        "NEXTSTATE Wait;",

        "state Wait;",
        "input startProcess(x);",
        "call returnlessProcedure(2, x);",
        "NEXTSTATE -;",
        "endstate;",

        "endprocess Exampleprocess;",
    };
    checkSequenceAndConsume(expectedOutput, consumableOutput);
}

} // namespace tests::sdl

QTEST_MAIN(tests::Sdl::tst_sdlmodel)

#include "tst_sdlexporter.moc"

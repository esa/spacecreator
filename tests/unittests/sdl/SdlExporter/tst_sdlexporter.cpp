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
#include <common/sdlmodelbuilder/sdlmodelbuilder.h>
#include <conversion/common/options.h>
#include <memory>
#include <sdl/SdlExporter/SdlOptions/options.h>
#include <sdl/SdlExporter/exporter.h>
#include <sdl/SdlModel/action.h>
#include <sdl/SdlModel/nextstate.h>
#include <sdl/SdlModel/process.h>
#include <sdl/SdlModel/sdlmodel.h>

using conversion::ModelType;
using conversion::Options;
using sdl::Action;
using sdl::ContinuousSignal;
using sdl::Input;
using sdl::NextState;
using sdl::Procedure;
using sdl::Process;
using sdl::SdlModel;
using sdl::State;
using sdl::StateMachine;
using sdl::Transition;
using sdl::VariableDeclaration;
using sdl::exporter::SdlExporter;
using sdl::exporter::SdlOptions;
using tests::common::SdlModelBuilder;

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
                Process(processName, std::make_unique<StateMachine>()))
            .build();
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
    }
}

void tst_sdlmodel::testGenerateProcess()
{
    QString modelName = "Example";
    QString modelPrefix = "Sdl_";

    QString processName = "Modemanager";

    // transition from state to state
    auto nextstate = std::make_unique<NextState>("itself");
    auto actions = std::vector<std::unique_ptr<Action>>();
    actions.push_back(std::move(nextstate));
    auto transition = std::make_unique<Transition>("", std::move(actions));

    auto inputs = std::vector<std::unique_ptr<Input>>();
    auto input1 = std::make_unique<Input>("some_input_name", transition.get());
    inputs.push_back(std::move(input1));

    auto contSignals = std::vector<std::unique_ptr<ContinuousSignal>>();

    auto state1 = std::make_unique<State>("Looping", std::move(inputs), std::move(contSignals));

    // another transition, from state2 to state1
    auto nextstate2 = std::make_unique<NextState>("toggle", state1.get());
    auto actions2 = std::vector<std::unique_ptr<Action>>();
    actions2.push_back(std::move(nextstate2));
    auto transition2 = std::make_unique<Transition>("", std::move(actions2));

    auto inputs2 = std::vector<std::unique_ptr<Input>>();
    auto input2 = std::make_unique<Input>("some_other_input_name", transition2.get());
    inputs2.push_back(std::move(input2));

    auto contSignals2 = std::vector<std::unique_ptr<ContinuousSignal>>();

    auto state2 = std::make_unique<State>("Idle", std::move(inputs2), std::move(contSignals2));

    auto states = std::vector<std::unique_ptr<State>>();
    states.push_back(std::move(state1));
    states.push_back(std::move(state2));

    auto transitions = std::vector<std::unique_ptr<Transition>>();
    auto sm = std::make_unique<StateMachine>(std::move(states), std::move(transitions));
    auto process = Process(processName, std::move(sm));
    SdlModel exampleModel(std::move(process), modelName);

    Options options;
    options.add(SdlOptions::sdlFilepathPrefix, modelPrefix);

    SdlExporter exporter;
    try {
        exporter.exportModel(&exampleModel, options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }

    QString filename = QString("%1%2.%3").arg(modelPrefix, modelName, "pr");
    QFile outputFile(filename);
    if (!outputFile.open(QIODevice::ReadOnly)) {
        QFAIL("requested file cannot be found");
    }
    QTextStream consumableOutput(&outputFile);
    verifyAndConsume(consumableOutput, "process Modemanager;");
    verifyAndConsume(consumableOutput, "START;");
    verifyAndConsume(consumableOutput, "NEXTSTATE");
    verifyAndConsume(consumableOutput, "state Looping;");
    verifyAndConsume(consumableOutput, "input some_input_name");
    verifyAndConsume(consumableOutput, "NEXTSTATE -");
    verifyAndConsume(consumableOutput, "endstate;");
    verifyAndConsume(consumableOutput, "state Idle;");
    verifyAndConsume(consumableOutput, "input some_other_input_name;");
    verifyAndConsume(consumableOutput, "endstate;");
    verifyAndConsume(consumableOutput, "endprocess Modemanager;");
}

} // namespace tests::sdl

QTEST_MAIN(tests::Sdl::tst_sdlmodel)

#include "tst_sdlexporter.moc"

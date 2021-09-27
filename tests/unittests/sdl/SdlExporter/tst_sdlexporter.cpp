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
#include <conversion/common/options.h>
#include <sdl/SdlExporter/SdlOptions/options.h>
#include <sdl/SdlExporter/exporter.h>
#include <sdl/SdlModel/process.h>
#include <sdl/SdlModel/sdlmodel.h>

using conversion::ModelType;
using conversion::Options;
using sdl::Input;
using sdl::Procedure;
using sdl::Process;
using sdl::SdlModel;
using sdl::State;
using sdl::StateMachine;
using sdl::Transition;
using sdl::VariableDeclaration;
using sdl::exporter::SdlExporter;
using sdl::exporter::SdlOptions;

namespace tests::conversion::Sdl {

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
    Process process(processName);
    SdlModel exampleModel(process);

    QVERIFY(exampleModel.modelType() == ModelType::Sdl);
    const auto *const data = &exampleModel.data();
    QVERIFY(processName == data->name());
}

void tst_sdlmodel::testGenerateProcess()
{
    QString processName = "name_of_the_process";
    Input input1("some_input_name");
    State state("some_state", { input1 });
    Process process(processName, StateMachine({ state }, std::vector<Transition>()));
    SdlModel exampleModel(process, "Example");

    Options options;
    options.add(SdlOptions::sdlFilepathPrefix, "Sdl_");

    SdlExporter exporter;
    try {
        exporter.exportModel(&exampleModel, options);
    } catch (const std::exception &ex) {
        QFAIL(ex.what());
    }
}

} // namespace tests::sdl

QTEST_MAIN(tests::conversion::Sdl::tst_sdlmodel)

#include "tst_sdlexporter.moc"

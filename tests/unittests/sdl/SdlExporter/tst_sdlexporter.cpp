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
using sdl::Process;
using sdl::SdlModel;
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
    SdlModel exampleModel;

    QVERIFY(exampleModel.modelType() == ModelType::Sdl);
    const auto *const process = &exampleModel.data();
    QVERIFY("defaultProcessName" == process->name());
}

void tst_sdlmodel::testGenerateProcess()
{
    SdlModel exampleModel;

    Options options;
    options.add(SdlOptions::sdlFilepathPrefix, "Sdl_");

    // SdlExporter
    (void)exampleModel;
}

} // namespace tests::conversion::sdl

QTEST_MAIN(tests::conversion::Sdl::tst_sdlmodel)

#include "tst_sdlexporter.moc"

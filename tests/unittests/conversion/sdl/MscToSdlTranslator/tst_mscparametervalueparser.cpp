/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2024 N7 Space Sp. z o.o.
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

#include "tst_mscparametervalueparser.h"

#include <common/modelloader/modelloader.h>
#include <conversion/sdl/MscToSdlTranslator/mscparametervalueparser.h>
#include <conversion/sdl/MscToSdlTranslator/signalinfo.h>
#include <msccore/mscparameterlist.h>
#include <sdl/SdlModel/rename.h>

using conversion::sdl::translator::MscParameterValueParser;
using conversion::sdl::translator::SignalInfo;
using msc::MscParameter;
using msc::MscParameterList;

namespace tests::sdl {

void tst_MscParameterValueParser::testSequenceOfLength()
{
    auto asnModel = plugincommon::ModelLoader::loadAsn1Model(QString("resources/types.asn"));

    QVERIFY(asnModel != nullptr);

    QCOMPARE(asnModel->data().size(), 1);

    MscParameterValueParser parser("test", asnModel->data().front().get());

    auto sdlSignal = std::make_unique<::sdl::Rename>("example", ::sdl::Rename::Direction::Input, "example", "");
    sdlSignal->setParametersTypes(QStringList { "ExampleSequenceOf" });
    auto parameter = MscParameter("{ 1, 3, 1}", "");
    auto parameterList = MscParameterList { parameter };
    auto testSignals = std::unordered_map<uint32_t, SignalInfo>();

    uint32_t signalId = 0;
    testSignals.emplace(signalId, SignalInfo { sdlSignal.get(), parameterList });

    MscParameterValueParser::SignalRequirementsMap map = parser.parseSignals(testSignals);

    QCOMPARE(map.size(), 1);
    QVERIFY(map.find(signalId) != map.end());

    QCOMPARE(map.at(0).size(), 4);
    QCOMPARE(map.at(0).at(0).first, "length(example_param0)");
    QVERIFY(map.at(0).at(0).second.has_value());
    QCOMPARE(map.at(0).at(0).second.value(), "3");

    QCOMPARE(map.at(0).at(1).first, "example_param0(0)");
    QVERIFY(map.at(0).at(1).second.has_value());
    QCOMPARE(map.at(0).at(1).second.value(), "1");

    QCOMPARE(map.at(0).at(2).first, "example_param0(1)");
    QVERIFY(map.at(0).at(2).second.has_value());
    QCOMPARE(map.at(0).at(2).second.value(), "3");

    QCOMPARE(map.at(0).at(3).first, "example_param0(2)");
    QVERIFY(map.at(0).at(3).second.has_value());
    QCOMPARE(map.at(0).at(3).second.value(), "1");
}
}

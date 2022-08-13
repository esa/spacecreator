/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "tst_tftable.h"

#include <conversion/sdl/MscToSdlTranslator/tftable.h>

using conversion::sdl::translator::TFTable;

namespace tests::sdl {

void tst_TFTable::testTFTableSimple()
{
    std::vector<uint32_t> sequence { 0, 1, 0, 1 };

    TFTable table(sequence, 2);

    {
        const auto &transitions = table.transitionsForState(0);
        QCOMPARE(transitions.at(0), 1);
        QCOMPARE(transitions.at(1), 0);
    }

    {
        const auto &transitions = table.transitionsForState(1);
        QCOMPARE(transitions.at(0), 1);
        QCOMPARE(transitions.at(1), 2);
    }

    {
        const auto &transitions = table.transitionsForState(2);
        QCOMPARE(transitions.at(0), 3);
        QCOMPARE(transitions.at(1), 0);
    }

    {
        const auto &transitions = table.transitionsForState(3);
        QCOMPARE(transitions.at(0), 1);
        QCOMPARE(transitions.at(1), 4);
    }
}

void tst_TFTable::testTFTableComplex()
{
    std::vector<uint32_t> sequence { 0, 1, 0, 1, 0, 2, 0 };

    TFTable table(sequence, 4);

    {
        const auto &transitions = table.transitionsForState(0);
        QCOMPARE(transitions.at(0), 1);
        QCOMPARE(transitions.at(1), 0);
        QCOMPARE(transitions.at(2), 0);
        QCOMPARE(transitions.at(3), 0);
    }

    {
        const auto &transitions = table.transitionsForState(1);
        QCOMPARE(transitions.at(0), 1);
        QCOMPARE(transitions.at(1), 2);
        QCOMPARE(transitions.at(2), 0);
        QCOMPARE(transitions.at(3), 0);
    }

    {
        const auto &transitions = table.transitionsForState(2);
        QCOMPARE(transitions.at(0), 3);
        QCOMPARE(transitions.at(1), 0);
        QCOMPARE(transitions.at(2), 0);
        QCOMPARE(transitions.at(3), 0);
    }

    {
        const auto &transitions = table.transitionsForState(3);
        QCOMPARE(transitions.at(0), 1);
        QCOMPARE(transitions.at(1), 4);
        QCOMPARE(transitions.at(2), 0);
        QCOMPARE(transitions.at(3), 0);
    }

    {
        const auto &transitions = table.transitionsForState(4);
        QCOMPARE(transitions.at(0), 5);
        QCOMPARE(transitions.at(1), 0);
        QCOMPARE(transitions.at(2), 0);
        QCOMPARE(transitions.at(3), 0);
    }

    {
        const auto &transitions = table.transitionsForState(5);
        QCOMPARE(transitions.at(0), 1);
        QCOMPARE(transitions.at(1), 4);
        QCOMPARE(transitions.at(2), 6);
        QCOMPARE(transitions.at(3), 0);
    }

    {
        const auto &transitions = table.transitionsForState(6);
        QCOMPARE(transitions.at(0), 7);
        QCOMPARE(transitions.at(1), 0);
        QCOMPARE(transitions.at(2), 0);
        QCOMPARE(transitions.at(3), 0);
    }
}

} // namespace tests::sdl

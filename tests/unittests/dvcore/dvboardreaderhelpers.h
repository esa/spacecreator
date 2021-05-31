/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#pragma once

#include <QByteArray>

namespace helpers {

static inline QByteArray emptyDoc()
{
    return QByteArray("<Boards>\n</Boards>");
}

static inline QByteArray nonValidDoc()
{
    return QByteArray("<InterfaceView>\n</InterfaceView>");
}

static inline QByteArray singleBoard()
{
    return QByteArray("<Boards>"
                      "    <Board name=\"node_linux\"></Board>"
                      "</Boards>");
}

static inline QByteArray unknownProperty()
{
    return QByteArray("<Boards>"
                      "    <Unknown name=\"some_text\"></Unknown>"
                      "</Boards>");
}

static inline QByteArray openCloseTagMismatched()
{
    return QByteArray("<Boards>"
                      "    <Unknown name=\"some_text\"></Test>"
                      "</Boards>");
}

static inline QByteArray malformedXml()
{
    return QByteArray("<Boards>"
                      "    <Unknown name=\"some_text\"><Test>"
                      "    <Test2 />"
                      "<Boards>");
}

static inline QByteArray singleBoardWithPort()
{
    return QByteArray(
            "<Boards>"
            "    <Board name=\"Linux on x86\" type=\"ocarina_processors_x86::x86.linux\">"
            "        <Device "
            "         asn1file=\"/home/taste/tool-inst/include/ocarina/runtime/polyorb-hi-c/src/drivers/configuration/"
            "         ip.asn\" asn1module=\"POHICDRIVER-IP\" asn1type=\"IP-Conf-T\" bus=\"eth0\" "
            "         requiresBusAccess=\"ocarina_buses::ip.i\">"
            "        </Device>"
            "    </Board>"
            "</Boards>");
}

static inline QByteArray multiBoardsWithPort()
{
    return QByteArray(
            "<Boards>"
            "    <Board name=\"Linux on Risc-V 32\" type=\"ocarina_processors_riscv32::riscv32.linux\">"
            "        <Device "
            "         asn1file=\"/home/taste/tool-inst/include/ocarina/runtime/polyorb-hi-c/src/drivers/configuration/"
            "         ip.asn\" asn1module=\"POHICDRIVER-IP\" asn1type=\"IP-Conf-T\" bus=\"eth0\" "
            "         requiresBusAccess=\"ocarina_buses::ip.i\">"
            "        </Device>"
            "    </Board>"
            "    <Board name=\"Linux on x86\" type=\"ocarina_processors_x86::x86.linux\">"
            "        <Device "
            "         asn1file=\"/home/taste/tool-inst/include/ocarina/runtime/polyorb-hi-c/src/drivers/configuration/"
            "         ip.asn\" asn1module=\"POHICDRIVER-IP\" asn1type=\"IP-Conf-T\" bus=\"eth0\" "
            "         requiresBusAccess=\"ocarina_buses::ip.i\">"
            "        </Device>"
            "    </Board>"
            "</Boards>");
}

} // namespace helpers

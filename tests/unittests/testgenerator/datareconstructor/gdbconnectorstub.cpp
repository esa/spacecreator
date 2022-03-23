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

#include "gdbconnectorstub.h"

namespace testgenerator {

QByteArray GdbConnector::getRawTestResults(const QString &binaryUnderTestDir, const QStringList &clientArgs,
        const QStringList &serverArgs, const QString &client, const QString &server)
{
    (void)binaryUnderTestDir;
    (void)clientArgs;
    (void)serverArgs;
    (void)client;
    (void)server;

    return QByteArray::fromRawData(rawData, resultBytesNum);
}

const char *const GdbConnector::rawData = "\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe" //
                                          "\x66\x66\x66\x66\x66\x66\x02\x40"
                                          "\xfe\xff\xff\xff\xff\xff\xff\xff"
                                          "\x03\x00\x00\x00\x00\x00\x00\x00"
                                          "\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe"

                                          "\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
                                          "\x38\xf8\xc2\x64\xaa\x60\x15\x40"
                                          "\xfe\xff\xff\xff\xff\xff\xff\xff"
                                          "\x03\x00\x00\x00\x00\x00\x00\x00"
                                          "\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe"

                                          "\x00\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
                                          "\x66\x66\x66\x66\x66\x66\x02\x40"
                                          "\xf8\xff\xff\xff\xff\xff\xff\xff"
                                          "\x03\x00\x00\x00\x00\x00\x00\x00"
                                          "\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe"

                                          "\x00\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
                                          "\x66\x66\x66\x66\x66\x66\x02\x40"
                                          "\xff\xff\xff\xff\xff\xff\xff\xff"
                                          "\xff\xff\xff\xff\xff\xff\xff\xff"
                                          "\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe"

                                          "\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe"
                                          "\x66\x66\x66\x66\x66\x66\x02\x40"
                                          "\x04\x00\x00\x00\x00\x00\x00\x00"
                                          "\x01\x00\x00\x00\x00\x00\x00\x00"
                                          "\x01\xbe\xbe\xbe\xbe\xbe\xbe\xbe";

} // namespace testgenerator

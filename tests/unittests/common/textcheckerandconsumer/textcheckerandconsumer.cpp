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

#include "textcheckerandconsumer.h"

namespace tests::common {

void TextCheckerAndConsumer::checkSequenceAndConsume(
        const std::vector<QString> &expectedOutput, QTextStream &consumableOutput)
{
    for (const auto &expectedLine : expectedOutput) {
        if (verifyAndConsume(consumableOutput, expectedLine)) {
            continue;
        } else {
            QString message = QString("the generated file does not contain '%1' substring").arg(expectedLine);
            throw std::logic_error(message.toStdString().c_str());
        }
    }
}

bool TextCheckerAndConsumer::verifyAndConsume(QTextStream &stream, const QString &string)
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

} // namespace tests::common

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

#pragma once

#include <QStringList>
#include <QTextStream>

namespace tests::common {

/**
 * @brief Helper class for checking if consumableOutput contains all strings from expectedOutput in correct order
 *
 */
class TextCheckerAndConsumer
{
public:
    /**
     * @brief Check if actualConsumableOutput contains all strings from expectedOutput in correct order. Throws
     *        an exception if not.
     *
     * @param expectedOutput         expected strings
     * @param actualConsumableOutput actual text stream to be checked
     */
    static auto checkSequenceAndConsume(const QStringList &expectedOutput, QTextStream &actualConsumableOutput) -> void;

    /**
     * @brief  Read file and return its contents as a vector of strings. Each string is a separate line, trimmed.
     *
     * @param  filename name of file to load
     *
     * @return vector of read strings
     */
    static auto readLinesFromFile(const QString &filename) -> const QStringList;

private:
    /**
     * @brief check if stream contain a requested string. If yes, remove everything before that string as well as the
     *        string itself
     *
     * @param streamToVerify  text stream to be checked and modified
     * @param requestedString requested string
     *
     * @return true if stream contains required value, false if not
     */
    static auto doesStreamContainRequested(QTextStream &streamToVerify, const QString &requestedString) -> bool;
};

} // namespace tests::common

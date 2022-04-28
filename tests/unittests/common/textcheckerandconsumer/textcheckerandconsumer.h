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

#include <QTextStream>
#include <vector>

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
    static auto checkSequenceAndConsume(const std::vector<QString> &expectedOutput, QTextStream &actualConsumableOutput)
            -> void;

    /**
     * @brief  Read file and return its contents as a vector of strings. Each string is a separate line, trimmed.
     *
     * @param  filename name of file to load
     *
     * @return vector of read strings
     */
    static auto readLinesFromFile(const QString &filename) -> std::vector<QString>;

private:
    static auto verifyAndConsume(QTextStream &streamToRansack, const QString &actual) -> bool;
};

} // namespace tests::common

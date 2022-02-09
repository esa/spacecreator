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

#include <cstdlib>
#include <iostream>
#include <seds/XmlValidator/validator.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: xmlvalidator <filename>\n";
        return EXIT_FAILURE;
    }

    try {
        const auto inputFilepath = QString(argv[1]);
        seds::validator::XmlValidator::validate(inputFilepath);
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "Validation successful!\n";
    return EXIT_SUCCESS;
}

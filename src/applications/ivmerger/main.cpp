/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2023 N7 Space Sp. z o.o.
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

#include "errorhub.h"

#include <cstdlib>
#include <iostream>
#include <ivmerger/ivmerger.h>

int main(int argc, char **argv)
{
    if (argc != 3) 
    {
        std::cerr << "Usage: ivmerger <inputTargetFilename> <inputSourceFilename>\n";
        return EXIT_FAILURE;
    }

    shared::ErrorHub::instance();
    ivmerger::IvMerger merger;
    const auto inputTargetFilepath = QString(argv[1]);
    const auto inputSourceFilepath = QString(argv[2]);

    if(!merger.mergeInterfaceViews(inputTargetFilepath, inputSourceFilepath))
    {
        std::cerr << "Failure during merging\n";

        for(shared::ErrorItem error : shared::ErrorHub::errors())
        {
            std::cerr << error.m_description.toStdString() << "\n";
        }

        return EXIT_FAILURE;
    }

    std::cout << "Merging successful!\n";
    return EXIT_SUCCESS;
}

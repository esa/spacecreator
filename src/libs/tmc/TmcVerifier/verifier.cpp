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

#include "verifier.h"

using tmc::converter::TmcConverter;

namespace tmc::verifier {
TmcVerifier::TmcVerifier(
        const QString &inputIvFilepath, const QString &outputDirectory, std::vector<QString> environmentFunctions)
    : m_inputIvFilepath(inputIvFilepath)
    , m_outputDirectory(outputDirectory)
{
    m_converter = std::make_unique<TmcConverter>(m_inputIvFilepath, m_outputDirectory, std::move(environmentFunctions));
}

bool TmcVerifier::addStopConditionFiles(const QStringList &files)
{
    return m_converter->addStopConditionFiles(files);
}

bool TmcVerifier::attachObserver(const QString &observerPath, const uint32_t priority)
{
    return m_converter->attachObserver(observerPath, priority);
}

bool TmcVerifier::execute()
{
    if (!m_converter->convert()) {
        return false;
    }
    if (!buildVerifier()) {
        return false;
    }
    if (!executeVerifier()) {
        return false;
    }
    return true;
}

bool TmcVerifier::buildVerifier()
{
    // TODO to implement
    return true;
}

bool TmcVerifier::executeVerifier()
{
    // TODO to implement
    return true;
}

}

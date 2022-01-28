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

#include "TestGeneratorContext.h"

#include "TestGeneratorException.h"

namespace testgenerator {

TestGeneratorContext::TestGeneratorContext(const Fields &headerFields, const InterfaceParameters &params)
{
    countOutputParameters(params);
    calculateHeaderFieldsToParamsMappings(headerFields, params);
}

auto TestGeneratorContext::mappings() const -> Mappings
{
    return m_mappings;
}

auto TestGeneratorContext::outputParametersCount() const -> unsigned int
{
    return m_outputParametersCount;
}

auto TestGeneratorContext::countOutputParameters(const InterfaceParameters &params) -> void
{
    m_outputParametersCount = std::count_if(params.begin(), params.end(),
            [](const auto &param) { return param.direction() == shared::InterfaceParameter::Direction::OUT; });
}

auto TestGeneratorContext::calculateHeaderFieldsToParamsMappings(
        const std::vector<csv::Field> &headerFields, const InterfaceParameters &params) -> void
{
    const unsigned int paramsSize = static_cast<unsigned int>(params.size());

    auto mappings = std::vector<unsigned int>(paramsSize, 0);

    if (!headerFields.empty()) {
        if (headerFields.size() != paramsSize - outputParametersCount()) {
            throw TestGeneratorException("Imported CSV contains invalid number of data columns");
        }
        std::vector<bool> elementsFound(paramsSize, false);
        for (unsigned int i = 0; i < paramsSize; i++) {
            const auto &param = params.at(static_cast<int>(i));
            const QString &name = param.name();
            if (param.direction() == shared::InterfaceParameter::Direction::OUT) {
                elementsFound.at(i) = true;
                continue;
            }
            for (unsigned int j = 0; j < headerFields.size(); j++) {
                const auto &field = headerFields.at(j);
                if (name.compare(field) == 0) {
                    elementsFound.at(i) = true;
                    mappings.at(i) = j;
                    break;
                }
            }
        }
        if (std::any_of(elementsFound.begin(), elementsFound.end(), [](const auto &found) -> bool { return !found; })) {
            throw TestGeneratorException("Header fields do not match interface parameter names");
        }
    } else {
        for (unsigned int i = 0; i < paramsSize; i++) {
            mappings[i] = i;
        }
    }

    m_mappings = mappings;
}

} // namespace testgenerator

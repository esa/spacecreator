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

#include <csv/CsvModel/csvmodel.h>
#include <shared/parameter.h>

namespace testgenerator {

typedef QVector<shared::InterfaceParameter> InterfaceParameters;
typedef std::vector<unsigned int> Mappings;
typedef std::vector<csv::Field> Fields;

class TestGeneratorContext final
{
public:
    TestGeneratorContext(const Fields &headerFields, const InterfaceParameters &params);

    auto mappings() const -> Mappings;

    auto outputParametersNum() const -> unsigned int;

private:
    auto countOutputParameters(const InterfaceParameters &params) -> void;
    auto getHeaderFieldsToParamsMappings(const Fields &headerFields, const InterfaceParameters &params) -> void;

    // which column in CSV model matches which interface input parameter
    // mappings index - interface parameter number
    // mappings value - data column in CSV model
    Mappings m_mappings;
    unsigned int m_outputParameters; // TODO: change to m_outputParametersNum
};

} // namespace testgenerator

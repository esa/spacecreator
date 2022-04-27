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

/**
 * @brief Context parameters for Test Driver Generator (holds values needed during the current generation)
 *
 */
class TestDriverGeneratorContext final
{
public:
    /**
     * @brief Construct a new Test Driver Generator Context object in the beginning of TestDriver generation
     *
     * @param headerFields  header of the CSV data
     * @param params        parameters of the interface under test
     */
    TestDriverGeneratorContext(const Fields &headerFields, const InterfaceParameters &params);

    /**
     * @brief   Get interface parameter number to data column mapping (which column in CSV model matches which interface
     *          input parameter:
     *          mappings index - interface parameter number
     *          mappings value - data column in CSV model
     *
     * @return  Interface parameter number to data column mapping
     */
    auto mappings() const -> Mappings;

    /**
     * @brief   Get number of output parameters of the interface
     *
     * @return  Number of output parameters of the interface
     */
    auto outputParametersCount() const -> unsigned int;

private:
    auto countOutputParameters(const InterfaceParameters &params) -> void;
    auto calculateHeaderFieldsToParamsMappings(const Fields &headerFields, const InterfaceParameters &params) -> void;

    Mappings m_mappings;
    unsigned int m_outputParametersCount;
};

} // namespace testgenerator

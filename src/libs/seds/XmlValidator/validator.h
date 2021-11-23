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

#pragma once

#include <QString>
#include <optional>

namespace seds::validator {

/**
 * This validator uses LibXml2 to validate given XML file
 * against given schema file.
 *
 * Validator will throw an exception in case of a validation error.
 */
class XmlValidator final
{
public:
    /**
     * @brief   Deleted constructor
     */
    XmlValidator() = delete;

public:
    /**
     * @brief   Validate given file
     *
     * This searches for the schema filename in the input XML file
     *
     * @param   inputFilepath       Input XML filepath
     *
     * @throws  XmlValidatorException  Any error that occurred during validation
     */
    static auto validate(const QString &inputFilepath) -> void;
    /**
     * @brief   Validate given file
     *
     * @param   inputFilepath       Input XML filepath
     * @param   schemaFilepath      Schema filepath
     *
     * @throws  XmlValidatorException  Any error that occurred during validation
     */
    static auto validate(const QString &inputFilepath, const QString &schemaFilepath) -> void;

private:
    /**
     * @brief   Extract schema filepath from the XML file
     *
     * @param   filename    File to search in
     *
     * @returns Schema filepath
     */
    static auto getSchemaFilepath(const QString &filepath) -> std::optional<QString>;

    /**
     * @brief   Callback for LibXml2 validation errors
     * @param   context     Context
     * @param   format      Format
     */
    static auto libXmlErrorFunc(void *context, const char *format, ...) -> void;
    /**
     * @brief   Callback for LibXml2 validation warnings
     * @param   context     Context
     * @param   format      Format
     */
    static auto libXmlWarningFunc(void *context, const char *format, ...) -> void;

private:
    /// @brief  Max size of a char buffer in handling XML errors
    static constexpr std::size_t m_errorBufferSize = 1024;
    /// @brief  Max size of a char buffer in handling XML warnings
    static constexpr std::size_t m_warningBufferSize = 1024;
};

} // namespace seds::validator

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

#include <memory>
#include <variant>

#include <seds/SedsCommon/basetypesmappings.h>

class QXmlStreamReader;

namespace converter {
class Model;
} // namespace converter

namespace seds::model {
class DataSheet;
class Package;
class PackageFile;
} // namespace seds::model

namespace seds::parser {

/**
 * This parser produces PackageFile or DataSheet object
 * from given SEDS XML file. Used to create SEDS representation
 * using SEDS::Model library.
 *
 * Parser assumes that passed XML is valid according to SEDS XSD,
 * as it doesn't perform any validation by itself.
 *
 * Parser will throw an exception in case of unrecoverable error
 * or when an unknown element/attribute is encountered.
 */
class Parser final
{
  public:
    /**
     * @brief   Parse given file
     *
     * @param   filename   Input SEDS filename
     *
     * @throws  ParserException
     *
     * @returns SEDS Model
     */
    static auto parse(const QString& filename) -> std::unique_ptr<converter::Model>;

  private:
    /**
     * @brief   Constructs PackageFile object
     *
     * @param   xmlReader   XML reader
     *
     * @returns PackageFile
     */
    static auto readPackageFile(QXmlStreamReader& xmlReader) -> model::PackageFile;
    /**
     * @brief   Constructs DataSheet object
     *
     * @param   xmlReader   XML reader
     *
     * @returns  Package
     */
    static auto readDataSheet(QXmlStreamReader& xmlReader) -> model::DataSheet;

    /**
     * @brief   Constructs Package object
     *
     * @param   xmlReader   XML reader
     *
     * @returns Package
     */
    static auto readPackage(QXmlStreamReader& xmlReader) -> model::Package;
};

} // namespace seds::parser

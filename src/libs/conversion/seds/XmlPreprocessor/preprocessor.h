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

#include <unordered_map>

#include <QRegularExpression>
#include <QString>

#include <seds/SymbolDefinitionReader/symboldefinitionreader.h>

class QDomNode;
class QDomElement;
class QDomText;

namespace seds::preprocessor {

/**
 * This preprocessor uses libxml2 and QtXml to preprocess given file
 *
 * Preprocesor will throw an exception in case of a preprocessing error
 */
class XmlPreprocessor final
{
  private:
    using ExternalReferencesMap = symbolreader::SymbolDefinitionReader::ExternalReferencesMap;

  public:
    /**
     * @brief   Preprocesses given file
     *
     * Preprocessing has two stages:
     * First libxml2 is used to resolve xincludes.
     * Then QtXml is used to replace external references with actual values that
     * user provided.
     *
     * The resulted preprocessed file is saved to the current directory.
     *
     * @param   inputFilename       Input filename
     * @param   outputFilename      Output filename
     * @param   externalReferences  External references values
     */
    static auto preprocess(const QString& inputFilename,
                           const QString& outputFilename,
                           const ExternalReferencesMap& externalReferences) -> void;

  private:
    /**
     * @brief   Preprocess xinclude tags
     *
     * @param   inputFilename       Input file
     * @param   outputFilename      Output file
     */
    static auto processXInclude(const QString& inputFilename, const QString& outputFilename) -> void;
    /**
     * @brief   Preprocess external references
     *
     * @param   filename            File to preprocess
     * @param   externalReferences  External references values
     */
    static auto processExternalReferences(const QString& filename, const ExternalReferencesMap& externalReferences)
            -> void;

    /**
     * @brief   Parse DOM node
     *
     * @param   node                Node to parse
     * @param   externalReferences  External references values
     * @param   filename            File that is being preprocessed
     */
    static auto parseNode(QDomNode& node, const ExternalReferencesMap& externalReferences, const QString& filename)
            -> void;
    /**
     * @brief   Parse DOM element node
     *
     * @param   element             Node to parse
     * @param   externalReferences  External references values
     * @param   filename            File that is being preprocessed
     */
    static auto parseElementNode(QDomElement& element,
                                 const ExternalReferencesMap& externalReferences,
                                 const QString& filename) -> void;
    /**
     * @brief   Parse DOM text node
     *
     * @param   text                Node to parse
     * @param   externalReferences  External references values
     * @param   filename            File that is being preprocessed
     */
    static auto parseTextNode(QDomText& text, const ExternalReferencesMap& externalReferences, const QString& filename)
            -> void;

  private:
    /** @brief  Regular expression for external reference */
    static const QRegularExpression externalReferenceRegExp;
    /** @brief  Output file encoding name */
    static constexpr const char* outputFileEncoding = "UTF-8";
    /** @brief  Output file indentation size */
    static constexpr int outputFileIndentation = 4;
};

} // namespace seds::preprocessor

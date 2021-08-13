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

#include "validator.h"

#include "exceptions.h"

#include <QFileInfo>
#include <conversion/common/exceptions.h>
#include <libxml/xinclude.h>
#include <libxml/xmlschemastypes.h>

using conversion::FileNotFoundException;

namespace seds::validator {

void XmlValidator::validate(const QString &inputFilename, const QString &schemaFilename)
{
    const QFileInfo schemaFileInfo(schemaFilename);
    if (!schemaFileInfo.exists()) {
        throw FileNotFoundException(schemaFilename, "while validating");
    }

    const QFileInfo inputXmlFileInfo(inputFilename);
    if (!inputXmlFileInfo.exists()) {
        throw FileNotFoundException(inputFilename, "while validating");
    }

    // Create context for schema
    auto parserCtx = xmlSchemaNewParserCtxt(schemaFilename.toUtf8().constData());
    if (parserCtx == nullptr) {
        throw XmlValidatorException(schemaFilename, "Failed to create schema parser context");
    }

    // Load schema file
    QString errorStr;
    xmlSchemaSetParserErrors(parserCtx, static_cast<xmlSchemaValidityErrorFunc>(XmlValidator::libXmlErrorFunc),
            static_cast<xmlSchemaValidityWarningFunc>(XmlValidator::libXmlWarningFunc), static_cast<void *>(&errorStr));
    auto schema = xmlSchemaParse(parserCtx);
    xmlSchemaFreeParserCtxt(parserCtx);

    if (!errorStr.isEmpty()) {
        throw XmlValidatorException(schemaFilename, errorStr);
    }

    // Load XML file
    auto xmlDoc = xmlReadFile(inputFilename.toUtf8().constData(), NULL, 0);

    if (xmlDoc == nullptr) {
        throw XmlValidatorException(inputFilename, "libxml2: couldn't parse");
    }

    // Preprocess XIncludes
    int includingResult = xmlXIncludeProcess(xmlDoc);

    if (includingResult == -1) {
        throw XmlValidatorException(inputFilename, "libxml2: XInclude error");
    }

    // Validate XML against schema
    auto validCtx = xmlSchemaNewValidCtxt(schema);
    xmlSchemaSetValidErrors(validCtx, static_cast<xmlSchemaValidityErrorFunc>(XmlValidator::libXmlErrorFunc),
            static_cast<xmlSchemaValidityWarningFunc>(XmlValidator::libXmlWarningFunc), static_cast<void *>(&errorStr));

    int validationResult = xmlSchemaValidateDoc(validCtx, xmlDoc);

    xmlSchemaFreeValidCtxt(validCtx);
    xmlFreeDoc(xmlDoc);

    if (validationResult == 0) {
        return;
    } else if (validationResult > 0) {
        throw XmlValidatorException(inputFilename, errorStr);
    } else {
        throw XmlValidatorException(inputFilename, "libxml2: Internal error");
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

void XmlValidator::libXmlErrorFunc(void *context, const char *format, ...)
{
    char buf[1024];
    va_list args;

    va_start(args, format);
    int len = vsnprintf(buf, sizeof(buf) / sizeof(buf[0]), format, args);
    va_end(args);

    if (len == 0) {
        *(static_cast<QString *>(context)) = "Can't create LibXml2 error message";
    } else {
        *(static_cast<QString *>(context)) = buf;
    }
}

void XmlValidator::libXmlWarningFunc(void *context, const char *format, ...)
{
    char buf[1024];
    va_list args;

    va_start(args, format);
    int len = vsnprintf(buf, sizeof(buf) / sizeof(buf[0]), format, args);
    va_end(args);

    if (len == 0) {
        *(static_cast<QString *>(context)) = "Can't create LibXml2 warning message";
    } else {
        *(static_cast<QString *>(context)) = buf;
    }
}

#pragma GCC diagnostic pop

} // namespace seds::validator

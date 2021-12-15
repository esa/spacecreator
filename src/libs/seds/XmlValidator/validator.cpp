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
#include <QXmlStreamReader>
#include <conversion/common/exceptions.h>
#include <filesystem>
#include <libxml/xinclude.h>
#include <libxml/xmlschemastypes.h>

using conversion::FileNotFoundException;

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#define SEDS_SPLIT_BEHAVIOR QString::SkipEmptyParts
#else
#define SEDS_SPLIT_BEHAVIOR Qt::SkipEmptyParts
#endif

namespace seds::validator {

void XmlValidator::validate(const QString &inputFilepath)
{
    const auto schemaFilepath = getSchemaFilepath(inputFilepath);
    if (!schemaFilepath) {
        throw XmlValidatorException(inputFilepath, "Schema filepath not passed nor found in the input XML file");
    }

    validate(inputFilepath, *schemaFilepath);
}

void XmlValidator::validate(const QString &inputFilepath, const QString &schemaFilepath)
{
    const QFileInfo schemaFileInfo(schemaFilepath);
    if (!schemaFileInfo.exists()) {
        throw FileNotFoundException(schemaFilepath, "while validating");
    }

    const QFileInfo inputXmlFileInfo(inputFilepath);
    if (!inputXmlFileInfo.exists()) {
        throw FileNotFoundException(inputFilepath, "while validating");
    }

    // Create context for schema
    auto parserCtx = xmlSchemaNewParserCtxt(schemaFilepath.toUtf8().constData());
    if (parserCtx == nullptr) {
        throw XmlValidatorException(schemaFilepath, "Failed to create schema parser context");
    }

    // Load schema file
    QString errorStr;
    xmlSchemaSetParserErrors(parserCtx, static_cast<xmlSchemaValidityErrorFunc>(XmlValidator::libXmlErrorFunc),
            static_cast<xmlSchemaValidityWarningFunc>(XmlValidator::libXmlWarningFunc), static_cast<void *>(&errorStr));
    auto schema = xmlSchemaParse(parserCtx);
    xmlSchemaFreeParserCtxt(parserCtx);

    if (!errorStr.isEmpty()) {
        throw XmlValidatorException(schemaFilepath, errorStr.trimmed());
    }

    // Load XML file
    auto xmlDoc = xmlReadFile(inputFilepath.toUtf8().constData(), NULL, 0);

    if (xmlDoc == nullptr) {
        throw XmlValidatorException(inputFilepath, "libxml2: couldn't parse");
    }

    // Preprocess XIncludes
    int includingResult = xmlXIncludeProcess(xmlDoc);

    if (includingResult == -1) {
        throw XmlValidatorException(inputFilepath, "libxml2: XInclude error");
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
        throw XmlValidatorException(inputFilepath, errorStr.trimmed());
    } else {
        throw XmlValidatorException(inputFilepath, "libxml2: Internal error");
    }
}

std::optional<QString> XmlValidator::getSchemaFilepath(const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadWrite)) {
        throw XmlValidatorException(filepath, file.errorString());
    }

    // Schema filename should be present in PackageFile/DataSheet
    auto xmlReader = QXmlStreamReader(&file);
    xmlReader.readNextStartElement();

    const auto attributes = xmlReader.attributes();
    const auto found = std::find_if(std::begin(attributes), std::end(attributes),
            [](auto &&attribute) { return attribute.name() == QStringLiteral("schemaLocation"); });

    if (found == std::end(attributes)) {
        return std::nullopt;
    }
    // xmi:schemaLocation contains url and schema filename separated by space
    const auto schemaLocation = found->value().split(" ", SEDS_SPLIT_BEHAVIOR);

    if (schemaLocation.size() != 2) {
        return std::nullopt;
    }

    const auto schemaFilename = schemaLocation.at(1).toString();

    auto fsPath = std::filesystem::path(filepath.toStdString());
    return QString::fromStdString(fsPath.replace_filename(schemaFilename.toStdString()));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

void XmlValidator::libXmlErrorFunc(void *context, const char *format, ...)
{
    char buf[m_errorBufferSize];
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
    char buf[m_warningBufferSize];
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

#undef SEDS_SPLIT_BEHAVIOR

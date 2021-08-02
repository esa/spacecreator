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

#include "preprocessor.h"

#include <converter/import/exceptions.h>

#include <QDomAttr>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomText>
#include <QFileInfo>
#include <QTextStream>
#include <libxml/xinclude.h>

#include "exceptions.h"

using converter::import::FileNotFound;

namespace seds::preprocessor {

const QRegularExpression XmlPreprocessor::externalReferenceRegExp = QRegularExpression("^\\${([a-z_\\.]+)}$");

void
XmlPreprocessor::preprocess(const QString& inputFilename,
                            const QString& outputFilename,
                            const ExternalReferencesMap& externalReferences)
{
    const QFileInfo inputFileInfo(inputFilename);
    if(!inputFileInfo.exists()) {
        throw FileNotFound(inputFilename, "while preprocessing");
    }

    processXInclude(inputFilename, outputFilename);
    processExternalReferences(outputFilename, externalReferences);
}

void
XmlPreprocessor::processXInclude(const QString& inputFilename, const QString& outputFilename)
{
    auto xmlDoc = xmlReadFile(inputFilename.toUtf8().constData(), NULL, 0);
    if(xmlDoc == nullptr) {
        throw XmlPreprocessorException(inputFilename, "libxml2: Unable to read");
    }

    int includingResult = xmlXIncludeProcess(xmlDoc);
    if(includingResult == -1) {
        throw XmlPreprocessorException(inputFilename, "libxml2: Failed to preprocess XInclude");
    }

    int savedBytes = xmlSaveFileEnc(outputFilename.toUtf8().constData(), xmlDoc, outputFileEncoding);
    if(savedBytes == -1) {
        throw XmlPreprocessorException(outputFilename, "libxml2: Failed to save file");
    }

    xmlFreeDoc(xmlDoc);
}

void
XmlPreprocessor::processExternalReferences(const QString& filename, const ExternalReferencesMap& externalReferences)
{
    const QFileInfo fileInfo(filename);
    if(!fileInfo.exists()) {
        throw FileNotFound(filename, "while processing external references");
    }

    QFile file(filename);
    if(!file.open(QIODevice::ReadWrite)) {
        throw XmlPreprocessorException(filename, file.errorString());
    }

    QDomDocument xmlDoc;
    if(!xmlDoc.setContent(&file)) {
        throw XmlPreprocessorException(filename, "Unable to load DOM");
    }

    auto node = xmlDoc.firstChild();
    while(!node.isNull()) {
        parseNode(node, externalReferences, filename);

        node = node.nextSibling();
    }

    file.resize(0);
    QTextStream textStream;
    textStream.setDevice(&file);
    xmlDoc.save(textStream, outputFileIndentation);
}

void
XmlPreprocessor::parseNode(QDomNode& node, const ExternalReferencesMap& externalReferences, const QString& filename)
{
    if(node.isElement()) {
        auto element = node.toElement();
        parseElementNode(element, externalReferences, filename);
    } else if(node.isText()) {
        auto text = node.toText();
        parseTextNode(text, externalReferences, filename);
    }

    auto child = node.firstChild();
    while(!child.isNull()) {
        parseNode(child, externalReferences, filename);

        child = child.nextSibling();
    }
}

void
XmlPreprocessor::parseElementNode(QDomElement& element,
                                  const ExternalReferencesMap& externalReferences,
                                  const QString& filename)
{
    const auto attributes = element.attributes();

    QDomAttr attribute;
    QString value;
    for(int i = 0; i < attributes.size(); ++i) {
        attribute = attributes.item(i).toAttr();
        value = attribute.value();

        const auto match = externalReferenceRegExp.match(value);
        if(match.hasMatch()) {
            const auto externalReference = match.captured(1);
            if(externalReferences.count(externalReference) == 0) {
                throw UndefinedExternalReference(filename, externalReference);
            }

            attribute.setValue(externalReferences.at(externalReference));
        }
    }
}

void
XmlPreprocessor::parseTextNode(QDomText& text, const ExternalReferencesMap& externalReferences, const QString& filename)
{
    const auto value = text.nodeValue();

    const auto match = externalReferenceRegExp.match(value);
    if(match.hasMatch()) {
        const auto externalReference = match.captured(1);
        if(externalReferences.count(externalReference) == 0) {
            throw UndefinedExternalReference(filename, externalReference);
        }

        text.setData(externalReferences.at(externalReference));
    }
}

} // namespace seds::preprocessor

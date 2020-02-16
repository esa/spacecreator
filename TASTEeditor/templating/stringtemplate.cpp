/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "stringtemplate.h"

#include <QApplication>
#include <QFileInfo>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <grantlee_templates.h>

#include "tab_aadl/aadlobject.h"

namespace taste3 {
namespace templating {

StringTemplate::StringTemplate(QObject *parent)
    : QObject(parent)
    , m_engine(new Grantlee::Engine(this))
    , m_fileLoader(QSharedPointer<Grantlee::FileSystemTemplateLoader>::create())
    , m_validateXMLDocument(true)
    , m_autoFormattingIndent(4)
{
    m_engine->setSmartTrimEnabled(true);

    auto cache = QSharedPointer<Grantlee::CachingLoaderDecorator>::create(m_fileLoader);
    m_engine->addTemplateLoader(cache);
}

/**
 * @brief StringTemplate::parseFile parses template file
 * @param grouppedObjects objects which are groupped by type name.
 * Type names can be Functions, Connections, Comments and etc.
 * @param templateFileName name of template file
 * @return generated and formatted XML text document
 */
QString StringTemplate::parseFile(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName)
{
    QFileInfo fileInfo(templateFileName);

    auto cache = m_engine->templateLoaders().first().staticCast<Grantlee::CachingLoaderDecorator>();
    cache->clear();

    m_fileLoader->setTemplateDirs({ fileInfo.absolutePath() });

    m_engine->setPluginPaths({ GRANTLEE_LIB_DIR, QApplication::applicationDirPath() });

    Grantlee::Context context;
    for (auto it = grouppedObjects.cbegin(); it != grouppedObjects.cend(); ++it)
        context.insert(it.key(), it.value());

    Grantlee::Template stringTemplate = m_engine->loadByName(fileInfo.fileName());
    if (stringTemplate->error()) {
        // Tokenizing or parsing error, or couldn't find custom tags or filters.
        qWarning() << stringTemplate->errorString();
        emit errorOccurred(stringTemplate->errorString());
        return QString();
    }

    const QString &result = stringTemplate->render(&context).trimmed();
    return formatText(result);
}

/**
 * @brief StringTemplate::formatText formats XML text to good preview
 * It uses autoFormattingIndent() for XML indentation
 * @param text input text
 * @return formatted text
 */
QString StringTemplate::formatText(const QString &text)
{
    if (!m_validateXMLDocument)
        return text;

    QString formattedText;
    QXmlStreamWriter xmlWriter(&formattedText);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.setAutoFormattingIndent(m_autoFormattingIndent);

    QXmlStreamReader xmlReader(text);
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isWhitespace())
            continue;
        if (xmlReader.hasError())
            break;
        xmlWriter.writeCurrentToken(xmlReader);
    }

    if (xmlReader.hasError()) {
        const QString &errorString = tr("Error: %1, error line: %2:%3").arg(xmlReader.errorString())
                                   .arg(xmlReader.lineNumber()).arg(xmlReader.columnNumber());
        qWarning() << Q_FUNC_INFO << errorString;
        emit errorOccurred(errorString);
        return text;
    }

    return formattedText.trimmed();
}

bool StringTemplate::isValidateXMLDocument() const
{
    return m_validateXMLDocument;
}

/**
 * @brief StringTemplate::autoFormattingIndent returns the current indent for XML formatting
 * @return indent
 */
int StringTemplate::autoFormattingIndent() const
{
    return m_autoFormattingIndent;
}

void StringTemplate::setValidateXMLDocument(bool validate)
{
    m_validateXMLDocument = validate;
}

/**
 * @brief StringTemplate::setAutoFormattingIndent sets a new indent for XML formatting
 * @param autoFormattingIndent
 */
void StringTemplate::setAutoFormattingIndent(int autoFormattingIndent)
{
    m_autoFormattingIndent = autoFormattingIndent;
}

} // ns processing
} // ns taste3

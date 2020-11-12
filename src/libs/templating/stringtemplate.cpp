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
#include <QDebug>
#include <QFileInfo>
#include <QSharedPointer>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <grantlee/outputstream.h>
#include <grantlee_templates.h>
#include <memory>

namespace templating {

/**
  Don't escape the code generation output.
'const QString &' should not become 'const QString &amp;'
*/
class NoEscapeOutputStream : public Grantlee::OutputStream
{
public:
    NoEscapeOutputStream()
        : Grantlee::OutputStream()
    {
    }

    NoEscapeOutputStream(QTextStream *stream, bool doEscape)
        : OutputStream(stream)
        , m_doEscape(doEscape)
    {
    }

    QSharedPointer<Grantlee::OutputStream> clone(QTextStream *stream) const override
    {
        return QSharedPointer<Grantlee::OutputStream>(new NoEscapeOutputStream(stream, m_doEscape));
    }

    QString escape(const QString &input) const override
    {
        return m_doEscape ? Grantlee::OutputStream::escape(input) : input;
    }

private:
    bool m_doEscape = true;
};

/*!
 * \namespace templating
 * \brief Shared code for the template based saving
 */

StringTemplate *StringTemplate::create(QObject *parent)
{
    StringTemplate *instance = new StringTemplate(parent);
    instance->init();
    return instance;
}

/**
 * @brief StringTemplate::StringTemplate ctor
 * @param parent
 */
StringTemplate::StringTemplate(QObject *parent)
    : QObject(parent)
{
}

void StringTemplate::init()
{
    m_engine = new Grantlee::Engine(this);
    m_engine->setSmartTrimEnabled(true);
    m_fileLoader = QSharedPointer<Grantlee::FileSystemTemplateLoader>::create();

    auto cache = QSharedPointer<Grantlee::CachingLoaderDecorator>::create(m_fileLoader);
    m_engine->addTemplateLoader(cache);
}

/**
 * @brief StringTemplate::parseFile parses template file
 * @param grouppedObjects objects which are grouped by type name.
 * Type names can be Functions, Connections, Comments and etc.
 * @param templateFileName name of template file
 * @param out a pointer to the QIODevice to store result
 * @return false if data has not been written
 */
bool StringTemplate::parseFile(
        const QHash<QString, QVariant> &grouppedObjects, const QString &templateFileName, QIODevice *out)
{
    if (!out || templateFileName.isEmpty()) {
        return false;
    }

    const QFileInfo fileInfo(templateFileName);

    if (auto cache = m_engine->templateLoaders().size()
                    ? m_engine->templateLoaders().first().staticCast<Grantlee::CachingLoaderDecorator>()
                    : nullptr)
        cache->clear();

    m_fileLoader->setTemplateDirs({ fileInfo.absolutePath() });

    //    m_engine->setPluginPaths({ GRANTLEE_LIB_DIR, QApplication::applicationDirPath() });
    m_engine->addPluginPath(QApplication::applicationDirPath());

    Grantlee::Context context;
    for (auto it = grouppedObjects.cbegin(); it != grouppedObjects.cend(); ++it) {
        const QString &name = it.key();
        QVariant v = it.value();
        bool isObject = false;
        if (v.canConvert<QVariantList>()) {
            QVariantList list = v.value<QVariantList>();
            if (list.size() == 1) {
                if (list[0].canConvert<QObject *>()) {
                    QObject *obj = list[0].value<QObject *>();
                    context.insert(name, obj);
                    isObject = true;
                }
            }
        }
        if (!isObject) {
            context.insert(name, v);
        }
    }

    const Grantlee::Template stringTemplate = m_engine->loadByName(fileInfo.fileName());
    if (stringTemplate->error()) {
        // Tokenizing or parsing error, or couldn't find custom tags or filters.
        qWarning() << Q_FUNC_INFO << stringTemplate->errorString();
        Q_EMIT errorOccurred(stringTemplate->errorString());
        return false;
    }

    QString output;
    QTextStream textStream(&output);
    NoEscapeOutputStream outputStream(&textStream, m_doEscape);
    stringTemplate->render(&outputStream, &context);
    const QString result = output.trimmed();

    const QString formatted = formatText(result);
    if (!out->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Can't open device for writing:" << out->errorString();
        Q_EMIT errorOccurred(out->errorString());
        return false;
    }

    out->write(formatted.toUtf8());
    out->close();
    return true;
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
        const QString &errorString = tr("Error: %1, error line: %2:%3\n")
                                             .arg(xmlReader.errorString())
                                             .arg(xmlReader.lineNumber())
                                             .arg(xmlReader.columnNumber());
        qWarning() << Q_FUNC_INFO << errorString;
        Q_EMIT errorOccurred(errorString);
        return text;
    }

    return formattedText.trimmed();
}

/**
 * @brief StringTemplate::needValidateXMLDocument returns whether XML needs to be validated
 * @return flag of validation XML
 */
bool StringTemplate::needValidateXMLDocument() const
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

/**
 * @brief StringTemplate::setValidateXMLDocument sets validation flag
 * @param validate
 */
void StringTemplate::setNeedValidateXMLDocument(bool validate)
{
    m_validateXMLDocument = validate;
}

/*!
   Set, if HTML sensible characters should be escaped
 */
void StringTemplate::setEscapeCharacters(bool doEscape)
{
    m_doEscape = doEscape;
}

/**
 * @brief StringTemplate::setAutoFormattingIndent sets a new indent for XML formatting
 * @param autoFormattingIndent
 */
void StringTemplate::setAutoFormattingIndent(int autoFormattingIndent)
{
    m_autoFormattingIndent = autoFormattingIndent;
}

}

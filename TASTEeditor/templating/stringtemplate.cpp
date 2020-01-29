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
    , m_autoFormattingIndent(4)
{
    QSharedPointer<Grantlee::FileSystemTemplateLoader> loader(new Grantlee::FileSystemTemplateLoader());
    m_engine->addTemplateLoader(loader);
    m_engine->setSmartTrimEnabled(true);
}

QString StringTemplate::parseFile(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName)
{
    QFileInfo fileInfo(templateFileName);

    QSharedPointer<Grantlee::FileSystemTemplateLoader> loader = m_engine->templateLoaders().first().staticCast<Grantlee::FileSystemTemplateLoader>();
    loader->setTemplateDirs({ fileInfo.absolutePath() });

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

    QString result = stringTemplate->render(&context).trimmed();
    return formatText(result);
}

QString StringTemplate::formatText(const QString &text)
{
    QString formattedText;
    QXmlStreamWriter xmlWriter(&formattedText);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.setAutoFormattingIndent(m_autoFormattingIndent);

    QXmlStreamReader xmlReader(text);
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isWhitespace() || xmlReader.isStartDocument() || xmlReader.isEndDocument())
            continue;
        if (xmlReader.hasError())
            break;
        xmlWriter.writeCurrentToken(xmlReader);
    }

    if (xmlReader.hasError()) {
        emit errorOccurred(tr("Error: %1, error line: %2:%3").arg(xmlReader.errorString())
                           .arg(xmlReader.lineNumber()).arg(xmlReader.columnNumber()));
        return text;
    }

    return formattedText.trimmed();
}

int StringTemplate::autoFormattingIndent() const
{
    return m_autoFormattingIndent;
}

void StringTemplate::setAutoFormattingIndent(int autoFormattingIndent)
{
    m_autoFormattingIndent = autoFormattingIndent;
}

} // ns processing
} // ns taste3

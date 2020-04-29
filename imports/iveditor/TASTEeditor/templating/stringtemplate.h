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

#pragma once

#include <QObject>
#include <QSharedPointer>

namespace Grantlee {
class Engine;
class FileSystemTemplateLoader;
}

class QIODevice;

namespace aadl {
class AADLObject;
}

namespace templating {

/**
 * @brief The StringTemplate class generates XML document from string template
 * which is based on Django syntax. Also it validates and formtats XML schema.
 * Internally it uses Grantlee libary: https://github.com/steveire/grantlee
 * To build and install Grantlee, please read README.md.
 */
class StringTemplate : public QObject
{
    Q_OBJECT
public:
    static StringTemplate *create(QObject *parent = nullptr);

    bool parseFile(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName,
                   QIODevice *out);

    QString formatText(const QString &text);

    bool needValidateXMLDocument() const;
    int autoFormattingIndent() const;

public Q_SLOTS:
    void setNeedValidateXMLDocument(bool validate);
    void setAutoFormattingIndent(int autoFormattingIndent);

Q_SIGNALS:
    void errorOccurred(const QString &errorString);

private:
    explicit StringTemplate(QObject *parent = nullptr);
    void init();
    Grantlee::Engine *m_engine;
    QSharedPointer<Grantlee::FileSystemTemplateLoader> m_fileLoader;
    bool m_validateXMLDocument;
    int m_autoFormattingIndent;
};

}

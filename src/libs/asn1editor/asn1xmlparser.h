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

class QDomElement;
class QDomNodeList;
class QFileInfo;
class QStringList;

namespace asn1 {

class Asn1XMLParser : public QObject
{
    Q_OBJECT

public:
    Asn1XMLParser(QObject *parent = nullptr);

    QVariantList parseAsn1File(const QFileInfo &fileInfo, QStringList *errorMessages);
    QVariantList parseAsn1File(const QString &filePath, const QString &fileName, QStringList *errorMessages);
    QVariantList parseAsn1XmlFile(const QString &fileName);

    QString asn1AsHtml(const QString &filename) const;

Q_SIGNALS:
    void parseError(const QString &error);

private:
    QVariantList parseXml(const QString &content);

    QVariantMap parseType(
            const QList<QDomNodeList> &typeAssignments, const QDomElement &type, const QString &name = QString());
    template<typename T>
    void parseRange(const QDomElement &type, QVariantMap &result);
    void parseSequenceType(const QList<QDomNodeList> &typeAssignments, const QDomElement &type, QVariantMap &result);
    void parseEnumeratedType(const QDomElement &type, QVariantMap &result);
    void parseChoiceType(const QList<QDomNodeList> &typeAssignments, const QDomElement &type, QVariantMap &result);

    void checkforCompiler() const;
    QString asn1CompilerCommand() const;
    QString temporaryFileName(const QString &basename, const QString &suffix) const;

    static QString m_asn1Compiler;
    static QString m_mono;
};

} // namespace asn1

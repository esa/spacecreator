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

#include <QCache>
#include <QObject>
#include <QStringList>
#include <QVector>
#include <memory>

class QFileInfo;

namespace Asn1Acn {
class File;

/*!
   Class to parse ASN.1 files and return the result as Asn1Acn::File object.

   If the source is a file, the converted xml is cached in the the application's cache directory.
   If the source is a text, the converted xml is cached in memory.

   \note uses the program asn1scc for converting to xml https://github.com/ttsiodras/asn1scc
 */
class Asn1Reader : public QObject
{
    Q_OBJECT

public:
    Asn1Reader(QObject *parent = nullptr);

    std::unique_ptr<Asn1Acn::File> parseAsn1File(const QFileInfo &fileInfo, QStringList *errorMessages);
    std::unique_ptr<Asn1Acn::File> parseAsn1File(
            const QString &filePath, const QString &fileName, QStringList *errorMessages);
    std::unique_ptr<Asn1Acn::File> parseAsn1File(
            const QString &fileName, QStringList *errorMessages, const QString &content);
    std::map<QString, std::unique_ptr<File>> parseAsn1Files(
            const QVector<QFileInfo> &fileInfos, QStringList *errorMessages);

    std::unique_ptr<Asn1Acn::File> parseAsn1XmlFile(const QString &fileName);
    std::unique_ptr<Asn1Acn::File> parseAsn1XmlContent(const QString &xmlContent, const QString &fileName);

    QString asn1AsHtml(const QString &filename) const;

    QString checkforCompiler() const;

    QString defaultParameter() const;

    int lineNumberFromError(const QString &error) const;
    QString fileNameFromError(const QString &error) const;

Q_SIGNALS:
    void parseError(const QString &error);

private:
    QString asn1CompilerCommand() const;
    QString temporaryFileName(const QString &basename, const QString &suffix) const;

    QByteArray fileHash(const QStringList &fileNames) const;
    bool convertToXML(const QStringList &asn1FileNames, const QString &xmlFilename, QStringList *errorMessages) const;
    QString createFilenameParameter(const QStringList &asn1FileNames) const;
    void parseAsn1SccErrors(QString errorString, QStringList *errorMessages) const;
    std::map<QString, std::unique_ptr<Asn1Acn::File>> parseAsn1XmlFileImpl(const QString &fileName);

    static QCache<QString, QString> m_cache;
};

}

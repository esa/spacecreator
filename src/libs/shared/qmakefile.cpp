/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "qmakefile.h"

#include "common.h"
#include "errorhub.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>

namespace shared {

bool QMakeFile::renameFileBasename(const QString &proFilePath, const QString &oldBasename, const QString &newBasename)
{
    QFile proFile(proFilePath);
    if (!proFile.open(QIODevice::ReadOnly)) {
        ErrorHub::addError(ErrorItem::Error, tr("Unable to read file %1").arg(proFilePath));
        return false;
    }

    QByteArray content = proFile.readAll();
    proFile.close();

    content.replace(oldBasename.toUtf8(), newBasename.toUtf8());

    if (!proFile.open(QIODevice::WriteOnly)) {
        ErrorHub::addError(ErrorItem::Error, tr("Unable to write file %1").arg(proFilePath));
        return false;
    }

    proFile.write(content);

    return true;
}

bool QMakeFile::createProFileForDirectory(const QString &path, const QStringList &externalFiles)
{
    const QDir targetDir(path);
    if (!targetDir.exists()) {
        ErrorHub::addError(ErrorItem::Error, tr("Directory %1 doesn't exist").arg(path));
        return false;
    }

    const QString proFileName = targetDir.dirName() + QLatin1String(".pro");
    QFile file(targetDir.filePath(proFileName));
    if (!file.open(QIODevice::WriteOnly)) {
        ErrorHub::addError(ErrorItem::Error, tr("Unable to create pro file for directory %1").arg(path));
        return false;
    }

    QTextStream stream(&file);
    stream << "TEMPLATE = lib" << Qt::endl;
    stream << "CONFIG -= qt" << Qt::endl;
    stream << "CONFIG += generateC" << Qt::endl << Qt::endl;

    QDirIterator it(path, QDir::Files);
    while (it.hasNext()) {
        const QString &filePath = it.next();
        if (it.fileName() == proFileName)
            continue;

        stream << "DISTFILES += $$PWD/" << it.fileName() << Qt::endl;
    }

    if (!externalFiles.isEmpty()) {
        stream << Qt::endl;
        stream << "# External ASN.1 files" << Qt::endl;
        for (const QString &asnFile : externalFiles) {
            stream << "DISTFILES += " << asnFile << Qt::endl;
        }
        stream << Qt::endl;
    }

    if (targetDir.exists(kRootImplementationPath)) {
        stream << Qt::endl << QStringLiteral("include($$PWD/%1/taste.pro)").arg(kRootImplementationPath) << Qt::endl;
    }

    return true;
}

QStringList QMakeFile::readFilesList(const QString &path, const QStringList &fileExtentions)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return {};
    }
    QStringList files;
    QTextStream stream(&file);
    QString line;
    bool isMultiLine = false;

    auto parseLine = [&](QString line, bool &isMultiLine) {
        if (isMultiLine || line.startsWith(QLatin1String("DISTFILES +="))) {
            line.remove(QLatin1String("DISTFILES +="));
            if (line.endsWith(QLatin1Char('\\'))) {
                isMultiLine = true;
                line.chop(1);
            } else { // last entry
                isMultiLine = false;
            }
            line = line.trimmed();
            if (line.isEmpty())
                return;

            const QStringList lineParts = line.split(QLatin1Char(' '), Qt::SkipEmptyParts);
            if (lineParts.isEmpty())
                return;

            for (QString token : qAsConst(lineParts)) {
                if (fileExtentions.contains(token.section(QLatin1Char('.'), -1))) {
                    files.append(token.remove(QLatin1String("$$PWD/")));
                }
            }
        }
    };

    while (stream.readLineInto(&line)) {
        if (line.isEmpty())
            continue;

        parseLine(line, isMultiLine);
    }
    return files;
}

} // namespace shared

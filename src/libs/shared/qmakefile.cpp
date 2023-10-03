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

bool QMakeFile::renameFileName(const QString &proFilePath, const QString &oldFileName, const QString &newFileName)
{
    static const QByteArray allowedCharsBeforer { " /\\(=" };
    static const QByteArray allowedCharsAfter { " )\n" };
    return replace(proFilePath, oldFileName, newFileName, allowedCharsBeforer, allowedCharsAfter);
}

bool QMakeFile::renameDirectory(const QString &proFilePath, const QString &oldDirectory, const QString &newDirectory)
{
    static const QByteArray allowedCharsBeforeDir { " /\\(=" };
    static const QByteArray allowedCharsAfterDir { " /\\)\n" };
    return replace(proFilePath, oldDirectory, newDirectory, allowedCharsBeforeDir, allowedCharsAfterDir);
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

bool QMakeFile::replace(const QString &proFilePath, const QString &oldString, const QString &newString,
        const QByteArray &allowedCharsBefore, const QByteArray &allowedCharsAfter)
{
    QFile proFile(proFilePath);
    if (!proFile.open(QIODevice::ReadOnly)) {
        ErrorHub::addError(ErrorItem::Error, tr("Unable to read file %1").arg(proFilePath));
        return false;
    }

    QList<QByteArray> content;
    while (!proFile.atEnd()) {
        content.append(proFile.readLine());
    }
    proFile.close();

    const QByteArray oldStr = oldString.toUtf8();
    int length = oldStr.length();
    const QByteArray newStr = newString.toUtf8();

    for (QByteArray &line : content) {
        int idx = 0;
        while (idx >= 0) {
            idx = line.indexOf(oldStr, idx);
            if (idx >= 0) {
                char charBefore = line[idx - 1];
                if (allowedCharsBefore.isEmpty() || allowedCharsBefore.contains(charBefore)) {
                    if (idx + length < line.length()) {
                        char charAfter = line[idx + length];
                        if (allowedCharsAfter.isEmpty() || allowedCharsAfter.contains(charAfter)) {
                            line.replace(idx, length, newStr);
                        }
                    }
                }
                idx = idx + newStr.length();
            }
        }
    }

    if (!proFile.open(QIODevice::WriteOnly)) {
        ErrorHub::addError(ErrorItem::Error, tr("Unable to open file %1").arg(proFilePath));
        return false;
    }

    if (proFile.write(content.join()) < 0) {
        ErrorHub::addError(ErrorItem::Error, tr("Unable to write file %1").arg(proFilePath));
        return false;
    }

    return false;
}

} // namespace shared

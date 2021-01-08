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

#include "asn1reader.h"

#include "asn1const.h"
#include "astxmlparser.h"
#include "file.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QRandomGenerator>
#include <QStandardPaths>

namespace Asn1Acn {

QString Asn1Reader::m_asn1Compiler {};
QString Asn1Reader::m_mono {};

Asn1Reader::Asn1Reader(QObject *parent)
    : QObject(parent)
{
}

std::unique_ptr<Asn1Acn::File> Asn1Reader::parseAsn1File(const QFileInfo &fileInfo, QStringList *errorMessages)
{
    return parseAsn1File(fileInfo.dir().absolutePath(), fileInfo.fileName(), errorMessages);
}

std::unique_ptr<Asn1Acn::File> Asn1Reader::parseAsn1File(
        const QString &filePath, const QString &fileName, QStringList *errorMessages)
{
    const QFileInfo asn1File(QDir(filePath), fileName);
    if (!asn1File.exists()) {
        errorMessages->append(tr("ASN.1 file %1 does not exist").arg(asn1File.absoluteFilePath()));
        return {};
    }

    const QString fullFilePath = asn1File.absoluteFilePath();
    const QByteArray asn1FileHash = fileHash(fullFilePath);

    const QString asnCacheFile =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/asn/" + asn1FileHash + ".xml";

    if (!QFile::exists(asnCacheFile)) {
        convertToXML(fullFilePath, asnCacheFile, errorMessages);
    }

    std::unique_ptr<Asn1Acn::File> asn1TypesData = parseAsn1XmlFile(asnCacheFile);

    return asn1TypesData;
}

std::unique_ptr<Asn1Acn::File> Asn1Reader::parseAsn1XmlFile(const QString &fileName)
{
    QFileInfo fileInfo;
    if (fileInfo.exists(fileName)) {
        QFile file(fileName);

        if (file.open(QIODevice::ReadOnly)) {

            QXmlStreamReader reader(&file);
            Asn1Acn::AstXmlParser parser(reader);
            const bool ok = parser.parse();
            if (!ok) {
                Q_EMIT parseError(tr("Error parsing the asn1 file %1").arg(fileName));
            }
            std::map<QString, std::unique_ptr<Asn1Acn::File>> data = parser.takeData();
            if (data.empty()) {
                Q_EMIT parseError(tr("Invalid XML format"));
                return {};
            }

            if (data.size() == 1) {
                return std::move(data.begin()->second);
            }

            if (data.find(fileInfo.fileName()) != data.end()) {
                return std::move(data[fileInfo.fileName()]);
            }
        } else
            Q_EMIT parseError(file.errorString());
    } else {
        Q_EMIT parseError(tr("File not found"));
    }

    return {};
}

QString Asn1Reader::asn1AsHtml(const QString &filename) const
{
    if (filename.isEmpty() || !QFile::exists(filename) || QFileInfo(filename).isDir()) {
        return {};
    }

    checkforCompiler();
    if (!m_asn1Compiler.isEmpty()) {
        const QString prettyPrintFileName = temporaryFileName("pretty_print", "stg");
        const QString asn1HtmlFileName = temporaryFileName("asn1", "html");

        // copy over pretty print fileb
        QFile prettyFile(prettyPrintFileName);
        if (prettyFile.open(QIODevice::WriteOnly)) {
            QFile pfile(":/asn1Resources/resources/pretty_print_asn1.stg");
            if (pfile.open(QIODevice::ReadOnly)) {
                prettyFile.write(pfile.readAll());
            } else {
                qDebug() << pfile.errorString() << "[:/asn1Resources/resources/pretty_print_asn1.stg]";
                return {};
            }
        } else {
            qDebug() << prettyFile.errorString() << "[" << prettyPrintFileName << "]";
            return {};
        }
        prettyFile.close();

#ifdef Q_OS_WIN
        QString cmd = QString("asn1 -customStg \"%1::%2 %3\"").arg(prettyPrintFileName, asn1HtmlFileName, filename);
#else
        QString cmd = QString("%1 %2 -customIcdUper %3::%4 %5")
                              .arg(m_mono, m_asn1Compiler, prettyPrintFileName, asn1HtmlFileName, filename);
#endif
        QProcess process;
        process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
        process.setProcessChannelMode(QProcess::MergedChannels);
        process.start(cmd);
        process.waitForFinished();

        QFile htmlFile(asn1HtmlFileName);
        QString html;
        if (htmlFile.open(QIODevice::ReadOnly)) {
            html = htmlFile.readAll();
            htmlFile.close();
        } else {
            qDebug() << htmlFile.errorString() << "[" << asn1HtmlFileName << "]";
            return {};
        }

        QFile::remove(prettyPrintFileName);
        QFile::remove(asn1HtmlFileName);
        return html;
    } else {
        return {};
    }
}

void Asn1Reader::checkforCompiler() const
{
#ifdef Q_OS_WIN
    if (m_asn1Compiler.isEmpty()) {
        QProcess process;
        process.start(QString("where asn1"));
        process.waitForFinished();
        QString asn1Exec = process.readAll();
        asn1Exec.remove("\r\n");
        asn1Exec.remove("asn1.exe");
        if (asn1Exec.isEmpty()) {
            qWarning() << tr("Unable to find the asn1scc compiler");
            return;
        }
        m_asn1Compiler = asn1Exec;
    }
#else
    if (m_asn1Compiler.isEmpty()) {
        QProcess process;
        process.start(QString("which asn1.exe"));
        process.waitForFinished();
        QString asn1Exec = process.readAll();
        asn1Exec.remove('\n');
        if (asn1Exec.isEmpty()) {
            qWarning() << tr("Unable to find the asn1scc compiler");
            return;
        }
        m_asn1Compiler = asn1Exec;
    }

    if (m_mono.isEmpty()) {
        QProcess process;
        process.start(QString("which mono"));
        process.waitForFinished();
        QString monoExec = process.readAll();
        monoExec.remove('\n');
        if (monoExec.isEmpty()) {
            qWarning() << tr("Unable to find the mono framework to run the asn1scc compiler");
            return;
        }
        m_mono = monoExec;
    }
#endif
}

QString Asn1Reader::asn1CompilerCommand() const
{
    checkforCompiler();
    if (!m_asn1Compiler.isEmpty()) {
#ifdef Q_OS_WIN
        return QString("asn1 -customStg \"%1xml.stg\"::").arg(m_asn1Compiler);
#else
        QFileInfo asnFile(m_asn1Compiler);
        return QString("%1 %2 -customStg %3/xml.stg:").arg(m_mono, m_asn1Compiler, asnFile.path());
#endif
    } else
        return {};
}

QString Asn1Reader::temporaryFileName(const QString &basename, const QString &suffix) const
{
    quint32 value = QRandomGenerator::securelySeeded().generate();
    return QDir::tempPath() + QDir::separator() + QString("%1_%2.%3").arg(basename).arg(value).arg(suffix);
}

QByteArray Asn1Reader::fileHash(const QString &fileName) const
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray content = file.readAll();
        QByteArray hash = QCryptographicHash::hash(content, QCryptographicHash::Md5);
        return hash.toHex();
    }
    return {};
}

bool Asn1Reader::convertToXML(const QString &asn1FileName, const QString &xmlFilename, QStringList *errorMessages) const
{
    static QString asn1Command;
    if (asn1Command.isEmpty()) {
        QString cmd = asn1CompilerCommand();
        if (cmd.isEmpty()) {
            if (errorMessages)
                errorMessages->append(tr(
                        "ASN1 parse error: Unable to run the asn1scc compiler. https://github.com/ttsiodras/asn1scc"));
            return false;
        }
        asn1Command = cmd + "%1 %2";
    }

    QString asn1FileNameParameter = "\"" + asn1FileName + "\"";
    QString asn1XMLFileName = temporaryFileName("asn1", "xml");

    QProcess asn1Process;
    connect(&asn1Process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [&](int, QProcess::ExitStatus exitStatus) {
                if (exitStatus == QProcess::CrashExit) {
                    const QString message = tr("asn1scc compiler process crashed");
                    qWarning() << message;
                    if (errorMessages)
                        errorMessages->append(message);
                }
            });

    connect(&asn1Process, &QProcess::errorOccurred, [&](QProcess::ProcessError) {
        qWarning() << asn1Process.errorString();
        if (errorMessages)
            errorMessages->append(asn1Process.errorString());
    });

    asn1Process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    asn1Process.setProcessChannelMode(QProcess::MergedChannels);
    asn1Process.start(QString(asn1Command).arg(asn1XMLFileName, asn1FileNameParameter));
    asn1Process.waitForFinished();

    auto error = asn1Process.readAll();
    if (!error.isEmpty()) {
        qWarning() << error;
        if (errorMessages)
            errorMessages->append(error);
        QFile::remove(asn1XMLFileName);
        return false;
    }

    QDir createpath;
    createpath.mkpath(QFileInfo(xmlFilename).absolutePath());
    QFile::rename(asn1XMLFileName, xmlFilename);

    return QFile::exists(xmlFilename);
}

} // namespace asn1

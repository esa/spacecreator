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
#include <QSettings>
#include <QStandardPaths>
#include <QTemporaryDir>

namespace Asn1Acn {

QString Asn1Reader::m_mono;
QCache<QString, QString> Asn1Reader::m_cache {};

#ifdef Q_OS_WIN
static const QString defaultParameter("-customStg \"%1xml.stg\"::");
#else
static const QString defaultParameter("-customStg %1/xml.stg:");
#endif

Asn1Reader::Asn1Reader(QObject *parent)
    : QObject(parent)
{
}

/*!
   \brief Parses a asn.1 file
   \param fileInfo information of the asn.1 file to parse
   \param errorMessages optional output parameter containing error messages
   \return An empty unique ptr if an error occured
   \return
 */
std::unique_ptr<Asn1Acn::File> Asn1Reader::parseAsn1File(const QFileInfo &fileInfo, QStringList *errorMessages)
{
    return parseAsn1File(fileInfo.dir().absolutePath(), fileInfo.fileName(), errorMessages);
}

/*!
   \brief Parses a asn.1 file
   \param filePath the directory (full path) of the asn.1 file. Does not inlcude the filename.
   \param fileName filename (without path) of the asn.1 file
   \param errorMessages optional output parameter containing error messages
   \return An empty unique ptr if an error occured
 */
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

/*!
   \brief Parses a asn.1 text
   \param fileName filename (without path) of the asn.1 file
   \param errorMessages optional output parameter containing error messages
   \param content the actual content of the asn.1 file
   \return An empty unique ptr if an error occured
 */
std::unique_ptr<Asn1Acn::File> Asn1Reader::parseAsn1File(
        const QString &fileName, QStringList *errorMessages, const QString &content)
{
    QByteArray hash = QCryptographicHash::hash(content.toUtf8(), QCryptographicHash::Md5).toHex();
    QTemporaryDir tempDir;
    QFileInfo fi(fileName);
    const QString asnFile = tempDir.filePath(fi.fileName());
    const QString xmlFile = asnFile + "xml";

    if (!m_cache.contains(hash)) {
        QFile fileAsn(asnFile);
        if (!fileAsn.open(QIODevice::WriteOnly)) {
            return {};
        }
        fileAsn.write(content.toUtf8());
        fileAsn.close();

        bool ok = convertToXML(asnFile, xmlFile, errorMessages);
        if (!ok) {
            return {};
        }

        QFile fileXml(xmlFile);
        if (!fileXml.open(QIODevice::ReadOnly)) {
            return {};
        }
        m_cache.insert(hash, new QString(fileXml.readAll()));
    }

    std::unique_ptr<Asn1Acn::File> asn1TypesData = parseAsn1XmlContent(*(m_cache[hash]), xmlFile);
    if (asn1TypesData) {
        asn1TypesData->setName(fileName);
    }
    return asn1TypesData;
}

/*!
   Reads the file \p fileName which is a asn->xml converted file
   \return An empty unique ptr if an error occured
 */
std::unique_ptr<Asn1Acn::File> Asn1Reader::parseAsn1XmlFile(const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists()) {
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

/*!
   Reads the text \p xmlContent (originally from asn.1 file \p fileName) which is a asn->xml converted file
   \return An empty unique ptr if an error occured
 */
std::unique_ptr<File> Asn1Reader::parseAsn1XmlContent(const QString &xmlContent, const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    QXmlStreamReader reader(xmlContent);
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

    return {};
}

/*!
   Returns the asn.1 file \p filename as HTML representation
 */
QString Asn1Reader::asn1AsHtml(const QString &filename) const
{
    if (filename.isEmpty() || !QFile::exists(filename) || QFileInfo(filename).isDir()) {
        return {};
    }

    QSettings settings;
    QString asn1Compiler = settings.value("SpaceCreator/asn1compiler").toString();
    if (asn1Compiler.isEmpty() || !QFile::exists(asn1Compiler)) {
        asn1Compiler = checkforCompiler();
    }
    if (!asn1Compiler.isEmpty()) {
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
                              .arg(m_mono, asn1Compiler, prettyPrintFileName, asn1HtmlFileName, filename);
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

QString Asn1Reader::checkforCompiler() const
{
#ifdef Q_OS_WIN
    QProcess process;
    process.start(QString("where asn1"));
    process.waitForFinished();
    QString asn1Exec = process.readAll();
    asn1Exec.remove("\r\n");
    asn1Exec.remove("asn1.exe");
    if (asn1Exec.isEmpty()) {
        qWarning() << tr("Unable to find the asn1scc compiler");
        return {};
    }
    return asn1Exec;
#else
    QProcess process;
    process.start(QString("which asn1.exe"));
    process.waitForFinished();
    QString asn1Exec = process.readAll();
    asn1Exec.remove('\n');
    if (asn1Exec.isEmpty()) {
        qWarning() << tr("Unable to find the asn1scc compiler");
        return {};
    }

    process.start(QString("which mono"));
    process.waitForFinished();
    m_mono = process.readAll();
    m_mono.remove('\n');
    if (m_mono.isEmpty()) {
        qWarning() << tr("Unable to find the mono framework to run the asn1scc compiler");
        return {};
    }

    return asn1Exec;
#endif
}

QString Asn1Reader::defaultParameter() const
{
    return Asn1Acn::defaultParameter;
}

QString Asn1Reader::asn1CompilerCommand() const
{
    QSettings settings;
    QString asn1Compiler = settings.value("SpaceCreator/asn1compiler").toString();
    QString parameter = settings.value("SpaceCreator/asn1compilerparameter").toString();

    if (asn1Compiler.isEmpty() || !QFile::exists(asn1Compiler)) {
        asn1Compiler = checkforCompiler();
    }

    if (parameter.isEmpty()) {
        parameter = defaultParameter();
    }

    if (!asn1Compiler.isEmpty()) {
        QFileInfo asnFile(asn1Compiler);
        const QString param = parameter.contains("%1") ? parameter.arg(asnFile.path()) : parameter;
        if (m_mono.isEmpty()) {
            return QString("%1 %2").arg(asn1Compiler, param);
        } else {
            return QString("%1 %2 %3").arg(m_mono, asn1Compiler, param);
        }
    } else {
        qWarning() << "No asn1scc compiler found";
        return {};
    }
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
    QString cmd = asn1CompilerCommand();
    if (cmd.isEmpty()) {
        if (errorMessages)
            errorMessages->append(
                    tr("ASN1 parse error: Unable to run the asn1scc compiler. https://github.com/ttsiodras/asn1scc"));
        return false;
    }
    QString asn1Command = cmd + "%1 %2";

    QString asn1FileNameParameter = "\"" + asn1FileName + "\"";
    QString asn1XMLFileName = temporaryFileName("asn1", "xml");

    QProcess asn1Process;
    connect(&asn1Process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [&](int, QProcess::ExitStatus exitStatus) {
                if (exitStatus == QProcess::CrashExit) {
                    const QString message = tr("asn1scc compiler process crashed");
                    if (errorMessages) {
                        errorMessages->append(message);
                    }
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
        if (errorMessages) {
            errorMessages->append(error);
        }
        QFile::remove(asn1XMLFileName);
        return false;
    }

    QDir createpath;
    createpath.mkpath(QFileInfo(xmlFilename).absolutePath());
    QFile::rename(asn1XMLFileName, xmlFilename);

    return QFile::exists(xmlFilename);
}

}

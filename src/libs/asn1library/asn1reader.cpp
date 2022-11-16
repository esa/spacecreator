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

#include "astxmlparser.h"
#include "errorhub.h"
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

QCache<QString, QString> Asn1Reader::m_cache {};

static const QStringList defaultParameter { "--field-prefix", "AUTO", "--xml-ast" };

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
    shared::ErrorHub::clearFileErrors(asn1File.absoluteFilePath());

    if (!asn1File.exists()) {
        QString msg = tr("ASN.1 file %1 does not exist").arg(asn1File.absoluteFilePath());
        errorMessages->append(msg);
        shared::ErrorHub::addError(shared::ErrorItem::Error, msg, asn1File.absoluteFilePath());

        return {};
    }

    const QString fullFilePath = asn1File.absoluteFilePath();
    const QByteArray asn1FileHash = fileHash(QStringList(fullFilePath));

    const QString asnCacheFile =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/asn/" + asn1FileHash + ".xml";

    if (!QFile::exists(asnCacheFile)) {
        if (!convertToXML(QStringList(fullFilePath), asnCacheFile, errorMessages)) {
            return {};
        }
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

        bool ok = convertToXML(QStringList(asnFile), xmlFile, errorMessages);
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

std::map<QString, std::unique_ptr<Asn1Acn::File>> Asn1Reader::parseAsn1Files(
        const QVector<QFileInfo> &fileInfos, QStringList *errorMessages)
{
    bool earlyError = false;
    for (const QFileInfo &fileInfo : fileInfos) {
        shared::ErrorHub::clearFileErrors(fileInfo.absoluteFilePath());

        if (!fileInfo.exists()) {
            QString msg = tr("ASN.1 file %1 does not exist").arg(fileInfo.absoluteFilePath());
            errorMessages->append(msg);
            shared::ErrorHub::addError(shared::ErrorItem::Error, msg, fileInfo.absoluteFilePath());
            earlyError = true;
        }
    }

    if (earlyError) {
        return {};
    }

    QStringList absoluteFilePaths;
    for (const QFileInfo &fileInfo : fileInfos) {
        absoluteFilePaths.append(fileInfo.absoluteFilePath());
    }

    const QByteArray asn1FileHash = fileHash(absoluteFilePaths);

    const QString asnCacheFile =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/asn/" + asn1FileHash + ".xml";

    if (!QFile::exists(asnCacheFile)) {
        if (!convertToXML(absoluteFilePaths, asnCacheFile, errorMessages)) {
            return {};
        }
    }

    return parseAsn1XmlFileImpl(asnCacheFile);
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

    return nullptr;
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
        QString cmd = QString("%1 -customIcdUper %2::%3 %4")
                              .arg(asn1Compiler, prettyPrintFileName, asn1HtmlFileName, filename);
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

/*!
   Does search in the system path for asn1scc and returns the full file path.
   An empty string is returned if asn1scc was not found.
 */
QString Asn1Reader::checkforCompiler() const
{
#ifdef Q_OS_WIN
    QProcess process;
    process.start(QString("where asn1"));
    process.waitForFinished();
    QString asn1Exec = process.readAll();
    asn1Exec.remove("\r\n");
    asn1Exec.remove("asn1scc.exe");
    if (asn1Exec.isEmpty()) {
        qWarning() << tr("Unable to find the asn1scc compiler");
        return {};
    }
    return asn1Exec;
#else
    QProcess process;
    process.start(QString("which asn1scc"));
    process.waitForFinished();
    QString asn1Exec = process.readAll();
    asn1Exec.remove('\n');
    if (asn1Exec.isEmpty()) {
        asn1Exec = QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
                + QLatin1String("/tool-inst/share/asn1scc/asn1scc");
        if (!QFileInfo::exists(asn1Exec)) {
            qWarning() << tr("Unable to find the asn1scc compiler");
            return {};
        }
    }
    return asn1Exec;
#endif
}

QStringList Asn1Reader::defaultParameter() const
{
    return Asn1Acn::defaultParameter;
}

int Asn1Reader::lineNumberFromError(const QString &error) const
{
    QStringList tokens = error.split(':');
    if (tokens.size() < 4) {
        return -1;
    }

    bool ok;
    int line = tokens[1].toInt(&ok);
    return ok ? line : -1;
}

QString Asn1Reader::fileNameFromError(const QString &error) const
{
    QStringList tokens = error.split(':');
    if (tokens.size() < 4) {
        return "";
    }

    return tokens.first();
}

QPair<QString, QStringList> Asn1Reader::asn1CompilerCommand() const
{
    QSettings settings;
    QString asn1Compiler = settings.value("SpaceCreator/asn1compiler").toString();
    QString parameter = settings.value("SpaceCreator/asn1compilerparameter").toString();

    // Compiler. Use compiler set by user or see if it is installed on the system
    if (asn1Compiler.isEmpty() || !QFile::exists(asn1Compiler)) {
        asn1Compiler = checkforCompiler();
    }
    if (asn1Compiler.isEmpty()) {
        qWarning() << "No asn1scc compiler found";
        return { {}, {} };
    }

    // Parameters to compiler. Use parameters set by user or use the default ones
    QStringList paramsList;
    if (parameter.isEmpty()) {
        paramsList = defaultParameter();
    } else {
#if QTC_VERSION == 408
        QStringList paramsList = parameter.split(' ', QString::SkipEmptyParts);
#endif
#if QTC_VERSION == 800
        QStringList paramsList = parameter.split(' ', Qt::SkipEmptyParts);
#endif
    }

    // replace %1 in users parameters with path to the asn1 compiler.
    QFileInfo asnFile(asn1Compiler);
    if (paramsList.contains("%1")) {
        int pos = paramsList.indexOf("%1");
        paramsList[pos] = asnFile.path();
    }

    return { asn1Compiler, paramsList };
}

QString Asn1Reader::temporaryFileName(const QString &basename, const QString &suffix) const
{
    quint32 value = QRandomGenerator::securelySeeded().generate();
    return QDir::tempPath() + QDir::separator() + QString("%1_%2.%3").arg(basename).arg(value).arg(suffix);
}

QByteArray Asn1Reader::fileHash(const QStringList &fileNames) const
{
    QVector<QString> tmp = QVector<QString>::fromList(fileNames);
    std::sort(tmp.begin(), tmp.end());

    QByteArray content;
    for (const QString &fileName : tmp) {
        /*
         * Append filename to the content to generate different
         * hash for the same files, but in different locations.
         * The cache xml file contains original locations
         * of input ASN.1 files, what was origin of at least one bug.
         */
        content.append(fileName.toUtf8());
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            content.append(file.readAll());
        }
    }

    QByteArray hash = QCryptographicHash::hash(content, QCryptographicHash::Md5);
    return hash.toHex();
}

bool Asn1Reader::convertToXML(
        const QStringList &asn1FileNames, const QString &xmlFilename, QStringList *errorMessages) const
{
    QPair<QString, QStringList> cmd = asn1CompilerCommand();
    auto asn1Compiler = cmd.first;
    QStringList parameters;
    parameters.append(cmd.second);

    // Check the compiler exists
    if (asn1Compiler.isEmpty()) {
        QString msg = tr("ASN1 parse error: Unable to run the asn1scc compiler. https://github.com/ttsiodras/asn1scc");
        if (errorMessages)
            errorMessages->append(msg);
        for (const QString &asn1FileName : asn1FileNames) {
            shared::ErrorHub::addError(shared::ErrorItem::Error, msg, asn1FileName);
        }
        return false;
    }

    // Add parameters for output xml file
    QString asn1XMLFileName = temporaryFileName("asn1", "xml");
    parameters << asn1XMLFileName;

    // Add parameters for asn1 input files
    parameters << asn1FileNames;

    // Setup the process
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
    connect(&asn1Process, &QProcess::errorOccurred,
            [&](QProcess::ProcessError) { parseAsn1SccErrors(asn1Process.errorString(), errorMessages); });

    asn1Process.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
    asn1Process.setProcessChannelMode(QProcess::MergedChannels);
    asn1Process.setProgram(asn1Compiler);
    asn1Process.setArguments(parameters);
    asn1Process.start();
    asn1Process.waitForFinished();

    auto error = asn1Process.readAll();
    if (!error.isEmpty()) {
        parseAsn1SccErrors(error, errorMessages);
        QFile::remove(asn1XMLFileName);
        return false;
    }

    QDir createpath;
    createpath.mkpath(QFileInfo(xmlFilename).absolutePath());
    QFile::rename(asn1XMLFileName, xmlFilename);

    return QFile::exists(xmlFilename);
}

void Asn1Reader::parseAsn1SccErrors(QString errorString, QStringList *errorMessages) const
{
    QTextStream stream(&errorString);

    QString line;
    while (stream.readLineInto(&line)) {
        const int lineNumber = lineNumberFromError(line);
        const QString fileName = fileNameFromError(line);
        shared::ErrorHub::addError(shared::ErrorItem::Error, line, fileName, lineNumber);
        if (errorMessages) {
            errorMessages->append(line);
        }
    }
}

std::map<QString, std::unique_ptr<Asn1Acn::File>> Asn1Reader::parseAsn1XmlFileImpl(const QString &fileName)
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

            return data;
        } else
            Q_EMIT parseError(file.errorString());
    } else {
        Q_EMIT parseError(tr("File not found"));
    }

    return {};
}

}

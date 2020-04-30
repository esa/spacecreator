/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "loghandler.h"

#include "logcategory.h"

#include <QDir>
#include <QLibraryInfo>
#include <QLoggingCategory>
#include <QStandardPaths>
#include <QtDebug>

LogHandler *LogHandler::m_instance = nullptr;
QtMessageHandler LogHandler::defaultHandler = nullptr;
QFile LogHandler::m_logFile;

static const qint64 kMaxLogSize = 2 * 1024 * 1024; // 2Mb
static const QString kLogFilename = QStringLiteral("taste.log");

LogHandler::LogHandler()
{
    Q_ASSERT_X(!m_instance, "LogHandler", "only one instance of Logger can be created!");
    LogHandler::m_instance = this;

    qSetMessagePattern(QLatin1String("%{if-debug}DEBUG     %{endif}"
                                     "%{if-info}INFO      %{endif}"
                                     "%{if-warning}ERROR     %{endif}"
                                     "%{if-critical}CRITICAL  %{endif}"
                                     "%{if-fatal}FATAL     %{endif} "
                                     "[%{time hh:mm:ss.zzz yyyy.MM.dd}] "
                                     "[%{function}@%{file}:%{line}] %{message}"));

    const QString configName = QLatin1String("qtlogging.ini");
    if (!QFile::exists(QLibraryInfo::location(QLibraryInfo::DataPath) + QDir::separator() + configName)) {
        const QStringList configLocations = QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation);
        auto it = std::find_if(configLocations.constBegin(), configLocations.constEnd(),
                               [configName](const QString &location) {
                                   const QString configPath =
                                           location.isEmpty() ? configName : location + QDir::separator() + configName;
                                   return QFile::exists(configPath);
                               });
        if (it == configLocations.constEnd())
            QLoggingCategory::setFilterRules(QStringLiteral("c_log.*=true\n"
                                                            "c_esa.*=true\n"
                                                            "c_aadl.*=true\n"));
    }

    m_logFile.setFileName(kLogFilename);
    open();
}

LogHandler::~LogHandler()
{
    close();
    LogHandler::m_instance = nullptr;
}

void LogHandler::open()
{
    if (m_logFile.isOpen()) {
        qWarning(c_log) << "Log file is already opened:" << m_logFile.fileName();
        return;
    }

    const QIODevice::OpenMode mode = m_logFile.size() < kMaxLogSize ? QIODevice::Append : QIODevice::WriteOnly;
    if (m_logFile.open(mode)) {
        defaultHandler = qInstallMessageHandler(&LogHandler::msgHandler);
        m_logFile.flush();
    }
    qInfo(c_log) << "Session started:" << VS_BUILD_HASH << m_logFile.size() << kMaxLogSize;
}

void LogHandler::close()
{
    qInfo(c_log) << "Session finished...\n\n";
    qInstallMessageHandler(nullptr);
    m_logFile.close();
}

QString LogHandler::logPath()
{
    return QFileInfo(m_logFile).absoluteFilePath();
}

void LogHandler::msgHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const QString newMsg = qFormatLogMessage(type, context, msg + QChar::CarriageReturn);
    if (m_logFile.isOpen()) {
        m_logFile.write(newMsg.toUtf8());
        m_logFile.flush();
    }
    defaultHandler(type, context, msg);
}

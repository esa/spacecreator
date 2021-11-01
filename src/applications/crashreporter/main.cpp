/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandlineparser.h"
#include "reports/bugreporthandler.h"
#include "scversion.h"
#include "sharedlibrary.h"

#include <QApplication>
#include <QFile>
#include <QtDebug>

int main(int argc, char *argv[])
{
    shared::initSharedLibrary();

    QApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(spaceCreatorVersion);
    a.setApplicationName(QObject::tr("Crash Reporter"));

    shared::CommandLineParser cmdParser;
    cmdParser.handlePositional(shared::CommandLineParser::Positional::StackTraceFilePath);
    cmdParser.process(a.arguments());

    const QVector<shared::CommandLineParser::Positional> args = cmdParser.positionalsSet();
    if (args.contains(shared::CommandLineParser::Positional::StackTraceFilePath)) {
        const QString stackTracePath = cmdParser.value(shared::CommandLineParser::Positional::StackTraceFilePath);
        const QString hostBase64 = cmdParser.value(shared::CommandLineParser::Positional::CrashReportHostBase64);
        const QString projectIDBase64 =
                cmdParser.value(shared::CommandLineParser::Positional::CrashReportProjectIDBase64);
        const QString accessTokenBase64 =
                cmdParser.value(shared::CommandLineParser::Positional::CrashReportAccessTokenBase64);

        const QString host = QByteArray::fromBase64(hostBase64.toUtf8());
        const QString projectID = QByteArray::fromBase64(projectIDBase64.toUtf8());
        const QByteArray accessToken = QByteArray::fromBase64(accessTokenBase64.toUtf8());
        if (host.isEmpty()) {
            qCritical("Remote endpoint can't be empty.");
            exit(1);
        }

        reports::BugReportHandler handler(host, projectID, accessToken);
        auto report = QSharedPointer<reports::BugReport>(new reports::BugReport);
        QFile file(stackTracePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qCritical() << QStringLiteral("Can't open stack trace file [%1]:").arg(stackTracePath, file.errorString());
            qApp->exit(1);
        }
        report->log = file.readAll();
        report->title = QLatin1String("Crash Report");
        handler.sendReport(report);
        QObject::connect(&handler, &reports::BugReportHandler::reportSent, &a, &QApplication::quit);
        QObject::connect(&handler, &reports::BugReportHandler::error, &a, [](const QString &text) {
            qCritical(qUtf8Printable(text));
            qApp->exit(1);
        });
        return a.exec();
    } else {
        cmdParser.showHelp();
    }

    return 0;
}

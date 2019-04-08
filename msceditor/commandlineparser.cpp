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

#include "commandlineparser.h"

#include <QDebug>
#include <QMetaEnum>

CommandLineParser::CommandLineParser()
    : QCommandLineParser()
{
    setApplicationDescription("MSC file editor");
    addHelpOption();
    addVersionOption();

    populatePositionalArgs();
}

bool CommandLineParser::isSet(CommandLineParser::Positional arg) const
{
    if (CommandLineParser::Positional::Unknown == arg)
        return false;

    return QCommandLineParser::isSet(positionalArg(arg));
}

QString CommandLineParser::value(CommandLineParser::Positional arg) const
{
    if (CommandLineParser::Positional::Unknown == arg)
        return QString();

    return QCommandLineParser::value(positionalArg(arg));
}

QCommandLineOption CommandLineParser::positionalArg(CommandLineParser::Positional arg)
{
    switch (arg) {
    case CommandLineParser::Positional::OpenFileMsc: {
        return QCommandLineOption({ "m", "open-msc" },
                                  QCoreApplication::translate("CommandLineParser", "Open the MSC <file> on startup."),
                                  QCoreApplication::translate("CommandLineParser", "file"));
    }
    case CommandLineParser::Positional::OpenFileAsn: {
        return QCommandLineOption({ "a", "open-asn" },
                                  QCoreApplication::translate("CommandLineParser", "Open the ASN <file> on startup."),
                                  QCoreApplication::translate("CommandLineParser", "file"));
    }
    case CommandLineParser::Positional::DbgOpenMscExamplesChain: {
        return QCommandLineOption(
                { "e", "examples-chain" },
                QCoreApplication::translate("CommandLineParser", "Open all MSC files in the <dir> one-by-one"),
                QCoreApplication::translate("CommandLineParser", "dir"));
    }
    case CommandLineParser::Positional::StartRemoteControl: {
        return QCommandLineOption({ "p", "remote-control-port" },
                                  QCoreApplication::translate("CommandLineParser", "Start remote control using <port>"),
                                  QCoreApplication::translate("CommandLineParser", "port"));
    }
    case CommandLineParser::Positional::DropUnsavedChangesSilently: {
        return QCommandLineOption(
                { "d", "drop-changes-silently" },
                QCoreApplication::translate("CommandLineParser",
                                            "Do not propose to save changes when closing a document"));
    }
    case CommandLineParser::Positional::EnableCifSupport: {
        return QCommandLineOption(
                { "c", "cif-enabled" },
                QCoreApplication::translate("CommandLineParser", "Enable geometry management via CIF"));
    }
    default: {
        Q_UNREACHABLE();
        break;
    }
    }

    qWarning() << Q_FUNC_INFO << "It seems the new option type is not handled here.";
    return QCommandLineOption("Unknown option");
}

void CommandLineParser::populatePositionalArgs()
{
    const QMetaEnum &e = QMetaEnum::fromType<CommandLineParser::Positional>();
    for (int i = 0; i < e.keyCount(); ++i) {
        if (CommandLineParser::Positional::Unknown != static_cast<CommandLineParser::Positional>(e.value(i)))
            addOption(positionalArg(CommandLineParser::Positional(e.value(i))));
    }
}

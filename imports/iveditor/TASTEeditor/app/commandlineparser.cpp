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

namespace taste3 {

/*!
\class taste3::CommandLineParser
\brief The processor of options passed as command line arguments.
*/
CommandLineParser::CommandLineParser()
    : QCommandLineParser()
{
    setApplicationDescription(tr("TASTE editor"));
    addHelpOption();
    addVersionOption();

    populatePositionalArgs();
}

/*!
    \enum CommandLineParser::Positional

    This enum type specifies a particular command line option:

    \value DropUnsavedChangesSilently
           Do not warn about unsaved changes on the document closing.
    \value OpenAADLXMLFile
           Automatically load the speceficied file on startup.
    \value OpenStringTemplateFile
           Load the specificied template file.
    \value ExportToFile
           Save the file opened by OpenAADLXMLFile using the template passed with OpenStringTemplateFile.
    \value ListScriptableActions
           List all the actions available for scripting and exit.
*/

/*!
  \fn bool CommandLineParser::isSet(Positional arg) const
 \brief checks the presence of the option \a arg.
 \a arg - the option to be checked
 Returns \c true if set.
*/
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
    case CommandLineParser::Positional::DropUnsavedChangesSilently:
        return QCommandLineOption(
                { "d", "drop-changes-silently" },
                QCoreApplication::translate("CommandLineParser",
                                            "Do not propose to save changes when closing a document."));
    case CommandLineParser::Positional::OpenAADLXMLFile:
        return QCommandLineOption(
                { "o", "open-aadl-xml" },
                QCoreApplication::translate("CommandLineParser", "Open an AADL XML <file> on startup."),
                QCoreApplication::translate("CommandLineParser", "file"));
    case CommandLineParser::Positional::OpenStringTemplateFile:
        return QCommandLineOption(
                { "t", "open-template" },
                QCoreApplication::translate("CommandLineParser",
                                            "Open a string template <file> on startup (-o option is required)."),
                QCoreApplication::translate("CommandLineParser", "file"));
    case CommandLineParser::Positional::ExportToFile:
        return QCommandLineOption(
                { "e", "export-to-xml" },
                QCoreApplication::translate(
                        "CommandLineParser",
                        "Export the doc to the <file> using default template (-o option is required)."),
                QCoreApplication::translate("CommandLineParser", "file"));
    case CommandLineParser::Positional::ListScriptableActions:
        return QCommandLineOption(
                { "l", "list-actions" },
                QCoreApplication::translate("CommandLineParser", "List scriptable actions and exit."));

    case CommandLineParser::Positional::Unknown:
        Q_UNREACHABLE();
        break;
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

} // ns taste3

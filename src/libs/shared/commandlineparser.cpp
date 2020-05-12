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

namespace shared {

CommandLineParser::CommandLineParser()
    : QCommandLineParser()
{
    addHelpOption();
    addVersionOption();
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
    if (CommandLineParser::Positional::Unknown == arg) {
        return false;
    }

    return QCommandLineParser::isSet(positionalArg(arg));
}

QString CommandLineParser::value(CommandLineParser::Positional arg) const
{
    if (CommandLineParser::Positional::Unknown == arg) {
        return QString();
    }

    return QCommandLineParser::value(positionalArg(arg));
}

void CommandLineParser::handlePositional(Positional arg)
{
    if (!isPositionalHandled(arg)) {
        addOption(positionalArg(arg));
        m_handledPositionals << arg;
    }
}

bool CommandLineParser::isPositionalHandled(CommandLineParser::Positional arg) const
{
    return m_handledPositionals.contains(arg);
}

/*! Get the list of command line options the user gave */
QVector<CommandLineParser::Positional> CommandLineParser::positionalsSet() const
{
    QVector<Positional> args;
    auto e = QMetaEnum::fromType<Positional>();
    for (int i = 0; i < e.keyCount(); ++i) {
        auto posArgType = static_cast<Positional>(e.value(i));
        if (isPositionalHandled(posArgType) && isSet(posArgType)) {
            if (posArgType == Positional::OpenFileMsc) {
                args.prepend(posArgType);
            } else {
                args.append(posArgType);
            }
        }
    }
    return args;
}

QCommandLineOption CommandLineParser::positionalArg(CommandLineParser::Positional arg)
{
    QStringList names;
    QString description;
    QString valueName;

    switch (arg) {
    case CommandLineParser::Positional::OpenFileMsc:
        names << "m"
              << "open-msc";
        description = QCoreApplication::translate("CommandLineParser", "Open the MSC <file> on startup");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
        break;
    case CommandLineParser::Positional::DbgOpenMscExamplesChain:
        names << "e"
              << "examples-chain";
        description = QCoreApplication::translate("CommandLineParser", "Open all MSC files in the <dir> one-by-one");
        valueName = QCoreApplication::translate("CommandLineParser", "dir");
        break;
    case CommandLineParser::Positional::StartRemoteControl:
        names << "p"
              << "remote-control-port";
        description = QCoreApplication::translate("CommandLineParser", "Start remote control using <port>");
        valueName = QCoreApplication::translate("CommandLineParser", "port");
        break;
    case CommandLineParser::Positional::DropUnsavedChangesSilently:
        names << "d"
              << "drop-changes-silently";
        description = QCoreApplication::translate(
                "CommandLineParser", "Do not propose to save changes when closing a document");
        break;
    case CommandLineParser::Positional::OpenAADLXMLFile:
        names << "o"
              << "open-aadl-xml";
        description = QCoreApplication::translate(
                "CommandLineParser", "Do not propose to save changes when closing a document");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
        break;
    case CommandLineParser::Positional::OpenStringTemplateFile:
        names << "t"
              << "open-template";
        description = QCoreApplication::translate(
                "CommandLineParser", "Open a string template <file> on startup (-o option is required).");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
        break;
    case CommandLineParser::Positional::ExportToFile:
        names << "x"
              << "export-to-xml";
        description = QCoreApplication::translate(
                "CommandLineParser", "Export the doc to the <file> using default template (-o option is required).");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
        break;
    case CommandLineParser::Positional::ListScriptableActions:
        names << "l"
              << "list-actions";
        description = QCoreApplication::translate("CommandLineParser", "List scriptable actions and exit.");
        break;
    default:
        qWarning() << Q_FUNC_INFO << "It seems the new option type is not handled here.";
        return QCommandLineOption("Unknown option");
    }

    return QCommandLineOption(names, description, valueName);
}

}

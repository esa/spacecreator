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

#include <QCoreApplication>
#include <QDebug>
#include <QMetaEnum>

namespace shared {

/*!
   \class CommandLineParser parses the command line
 */

/*!
    \enum shared::CommandLineParser::Positional

    This enum type specifies a particular command line option:
    \var shared::CommandLineParser::OpenFileMsc
           Msc file to open directly
    \var shared::CommandLineParser::DbgOpenMscExamplesChain
           Used for debug/test purpose only
    \var shared::CommandLineParser::StartRemoteControl
           Run the MSC editor in streaming mode
    \var shared::CommandLineParser::OpenXMLFile
           Automatically load the speceficied file on startup.
    \var shared::CommandLineParser::ListScriptableActions
           List all the actions available for scripting and exit.
    \var shared::CommandLineParser::OpenStringTemplateFile
           Load the specified template file.
    \var shared::CommandLineParser::ExportToFile
           Save the file opened by OpenXMLFile using the template passed with OpenStringTemplateFile.
    \var shared::CommandLineParser::DropUnsavedChangesSilently
           Do not warn about unsaved changes on the document closing.
*/

CommandLineParser::CommandLineParser()
    : QCommandLineParser()
{
    addHelpOption();
    addVersionOption();
    setApplicationDescription(QCoreApplication::applicationName());
}

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
    case CommandLineParser::Positional::OpenFileMsc: {
        names << "m"
              << "open-msc";
        description = QCoreApplication::translate("CommandLineParser", "Open the MSC <file> on startup");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
    } break;
    case CommandLineParser::Positional::DbgOpenMscExamplesChain: {
        names << "e"
              << "examples-chain";
        description = QCoreApplication::translate("CommandLineParser", "Open all MSC files in the <dir> one-by-one");
        valueName = QCoreApplication::translate("CommandLineParser", "dir");
    } break;
    case CommandLineParser::Positional::StartRemoteControl: {
        names << "p"
              << "remote-control-port";
        description = QCoreApplication::translate("CommandLineParser", "Start remote control using <port>");
        valueName = QCoreApplication::translate("CommandLineParser", "port");
    } break;
    case CommandLineParser::Positional::DropUnsavedChangesSilently: {
        names << "d"
              << "drop-changes-silently";
        description = QCoreApplication::translate(
                "CommandLineParser", "Do not propose to save changes when closing a document");
    } break;
    case CommandLineParser::Positional::OpenXMLFile: {
        names << "o"
              << "open-xml";
        description = QCoreApplication::translate("CommandLineParser", "The xml document to load document");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
    } break;
    case CommandLineParser::Positional::OpenStringTemplateFile: {
        names << "t"
              << "template";
        description = QCoreApplication::translate("CommandLineParser", "Template to use for storing the file");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
    } break;
    case CommandLineParser::Positional::ExportToFile: {
        names << "x"
              << "export";
        description = QCoreApplication::translate("CommandLineParser",
                "Export the doc to the <file> using the template set by -t or the default template.");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
    } break;
    case CommandLineParser::Positional::ListScriptableActions: {
        names << "l"
              << "list-actions";
        description = QCoreApplication::translate("CommandLineParser", "List scriptable actions and exit.");
    } break;
    case CommandLineParser::Positional::SedsConverterInputFilename: {
        names << "i"
              << "in";
        description = QCoreApplication::translate("CommandLineParser", "Specify input <file>");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
    } break;
    case CommandLineParser::Positional::SedsConverterOutputFilename: {
        names << "out";
        description = QCoreApplication::translate("CommandLineParser", "Specify main output <file>");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
    } break;
    case CommandLineParser::Positional::SedsConverterModelFrom: {
        names << "from";
        description = QCoreApplication::translate(
                "CommandLineParser", "Specify <list> of source models for conversion from: ASN.1, SEDS, InterfaceView, e.g. model1,model2");
        valueName = QCoreApplication::translate("CommandLineParser", "list");
    } break;
    case CommandLineParser::Positional::SedsConverterModelTo: {
        names << "to";
        description = QCoreApplication::translate(
                "CommandLineParser", "Specify target <model> for conversion from: ASN.1, SDL, InterfaceView");
        valueName = QCoreApplication::translate("CommandLineParser", "model");
    } break;
    case CommandLineParser::Positional::SedsConverterModelAux: {
        names << "aux-models";
        description = QCoreApplication::translate("CommandLineParser", "<list> of auxillary models for conversion, e.g. model1,model2,...");
        valueName = QCoreApplication::translate("CommandLineParser", "list");
    } break;
    case CommandLineParser::Positional::SedsConverterIvConfig: {
        names << "iv-config";
        description = QCoreApplication::translate("CommandLineParser", "Specify config xml <file> for InterfaceView");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
    } break;
    case CommandLineParser::Positional::SedsConverterSedsSchemaFilename: {
        names << "seds-schema";
        description = QCoreApplication::translate("CommandLineParser", "Specify schema <file> for SEDS");
        valueName = QCoreApplication::translate("CommandLineParser", "file");
    } break;
    case CommandLineParser::Positional::SedsConverterSedsExtRef: {
        names << "seds-ext-ref";
        description = QCoreApplication::translate("CommandLineParser", "Specify external <references> for SEDS, e.g. foo:bar,foo2:bar2,foo3:bar3,...");
        valueName = QCoreApplication::translate("CommandLineParser", "references");
    } break;
    case CommandLineParser::Positional::SedsConverterSedsExtRefFilename: {
        names << "seds-ext-ref-file";
        description = QCoreApplication::translate("CommandLineParser", "Specify <file> with an external references for SEDS");
        valueName = QCoreApplication::translate("CommandLineParser", "list");
    } break;
    case CommandLineParser::Positional::SedsConverterSkipValidation: {
        names << "skip-validation";
        description = QCoreApplication::translate("CommandLineParser", "Skip validation for SEDS");
    } break;
    case CommandLineParser::Positional::SedsConverterKeepIntermediateFiles: {
        names << "keep-intermediate-files";
        description = QCoreApplication::translate("CommandLineParser", "Keep SEDS intermediate files");
    } break;
    case CommandLineParser::Positional::SedsConverterAcnFilepathPrefix: {
        names << "acn-filepath-prefix";
        description = QCoreApplication::translate("CommandLineParser", "<prefix> to add at the beginning of the ACN output filepath");
        valueName = QCoreApplication::translate("CommandLineParser", "prefix");
    } break;
    case CommandLineParser::Positional::SedsConverterAsn1FilepathPrefix: {
        names << "asn1-filepath-prefix";
        description = QCoreApplication::translate("CommandLineParser", "<prefix> to add at the beginning of the ASN1 output filepath");
        valueName = QCoreApplication::translate("CommandLineParser", "prefix");
    } break;
    case CommandLineParser::Positional::SedsConverterSdlFilepathPrefix: {
        names << "sdl-filepath-prefix";
        description = QCoreApplication::translate("CommandLineParser", "SDL filepath <prefix>");
        valueName = QCoreApplication::translate("CommandLineParser", "prefix");
    } break;
    case CommandLineParser::Positional::SedsConverterImportXmlFileForAsn1: {
        names << "import-xml-file-for-asn1";
        description = QCoreApplication::translate("CommandLineParser", "Import Xml file for ASN.1 model");
    } break;
    default:
        qWarning() << Q_FUNC_INFO << "It seems the new option type is not handled here.";
        return QCommandLineOption("Unknown option");
    }

    return QCommandLineOption(names, description, valueName);
}

}

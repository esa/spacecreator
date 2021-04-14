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

#include "ivlibrary.h"
#include "commandlineparser.h"
#include "interface/interfacedocument.h"
#include "iveditor.h"
#include "sharedlibrary.h"
#include "xmldocexporter.h"

#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    ivm::initIVLibrary();
    ive::initIVEditor();
    shared::initSharedLibrary();

    QCoreApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(SC_VERSION);
    a.setApplicationName(QObject::tr("IV converter"));

    shared::CommandLineParser cmdParser;
    cmdParser.handlePositional(shared::CommandLineParser::Positional::OpenIVFile);
    cmdParser.handlePositional(shared::CommandLineParser::Positional::OpenStringTemplateFile);
    cmdParser.handlePositional(shared::CommandLineParser::Positional::ExportToFile);
    cmdParser.process(a.arguments());

    const QVector<shared::CommandLineParser::Positional> args = cmdParser.positionalsSet();
    if (args.contains(shared::CommandLineParser::Positional::OpenIVFile)
            && args.contains(shared::CommandLineParser::Positional::OpenStringTemplateFile)
            && args.contains(shared::CommandLineParser::Positional::ExportToFile)) {
        const QString inputFile = cmdParser.value(shared::CommandLineParser::Positional::OpenIVFile);
        const QString templateFile = cmdParser.value(shared::CommandLineParser::Positional::OpenStringTemplateFile);
        const QString outputFile = cmdParser.value(shared::CommandLineParser::Positional::ExportToFile);

        ive::InterfaceDocument doc;
        const bool loadOk = doc.load(inputFile);
        if (!loadOk) {
            qCritical() << "Unable to load file" << inputFile;
            return -1;
        }
        const bool convertOk = ive::XmlDocExporter::exportDocSilently(&doc, outputFile, templateFile);
        if (!convertOk) {
            qCritical() << "Error converting " << inputFile << "to" << outputFile;
            return -1;
        }
        return 0;
    } else {
        cmdParser.showHelp();
    }

    return 0;
}

/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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
#include "msclibrary.h"
#include "mscwriter.h"
#include "sharedlibrary.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    shared::initSharedLibrary();
    msc::initMscLibrary();

    QCoreApplication a(argc, argv);
    a.setApplicationName(QObject::tr("MSC Converter"));
    a.setApplicationVersion("0.0.1");

    shared::CommandLineParser cmdParser;
    cmdParser.setApplicationDescription("MSC file converter");
    cmdParser.handlePositional(shared::CommandLineParser::Positional::OpenFileMsc);
    cmdParser.handlePositional(shared::CommandLineParser::Positional::ExportToFile);
    cmdParser.handlePositional(shared::CommandLineParser::Positional::OpenStringTemplateFile);
    cmdParser.process(a.arguments());

    const QVector<shared::CommandLineParser::Positional> args = cmdParser.positionalsSet();
    if (args.contains(shared::CommandLineParser::Positional::OpenFileMsc)
            && args.contains(shared::CommandLineParser::Positional::OpenStringTemplateFile)
            && args.contains(shared::CommandLineParser::Positional::ExportToFile)) {
        // Convert the .msc file
        const QString inputFile = cmdParser.value(shared::CommandLineParser::Positional::OpenFileMsc);
        const QString templateFile = cmdParser.value(shared::CommandLineParser::Positional::OpenStringTemplateFile);
        const QString outputFile = cmdParser.value(shared::CommandLineParser::Positional::ExportToFile);

        msc::MscWriter writer;
        const bool ok = writer.convertMscFile(inputFile, templateFile, outputFile);
        return ok ? 0 : 1;
    } else {
        cmdParser.showHelp();
    }
}

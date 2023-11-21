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

#include "commandlineparser.h"
#include "dvappmodel.h"
#include "dveditor.h"
#include "dveditorcore.h"
#include "dvexporter.h"
#include "dvlibrary.h"
#include "dvmodel.h"
#include "interfacedocument.h"
#include "iveditorcore.h"
#include "scversion.h"
#include "sharedlibrary.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <spacecreatorsystem/ivsystemqueries.h>


static void setupIvSystemQueriesIfAvailable(const QString &inputFile, dve::DVEditorCore *dvcore)
{
    // If available, load Interface View to enable additional queries
    const QFileInfo inputFileInfo(inputFile);
    const QDir modelDirectory = inputFileInfo.dir();
    const QString interfaceViewFilePath = modelDirectory.filePath(shared::kDefaultInterfaceViewFileName);

    const QFileInfo interfaceViewFileInfo(interfaceViewFilePath);
    if (interfaceViewFileInfo.exists()) {
        IVEditorCorePtr ivcore(new ive::IVEditorCore);
        if (!ivcore->document()->load(interfaceViewFilePath)) {
            qWarning() << "InterfaceView " << interfaceViewFilePath << " could not be loaded";
        } else {
            auto checker = new scs::IvSystemQueries(nullptr, dvcore);
            checker->setIVCore(ivcore);
            dvcore->setSystemChecker(checker);
        }
    }
}

int main(int argc, char *argv[])
{
    shared::initSharedLibrary();
    dvm::initDVLibrary();
    dve::initDvEditor();

    QApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(spaceCreatorVersion);
    a.setApplicationName(QObject::tr("DV converter"));

    shared::CommandLineParser cmdParser;
    cmdParser.handlePositional(shared::CommandLineParser::Positional::OpenXMLFile);
    cmdParser.handlePositional(shared::CommandLineParser::Positional::OpenStringTemplateFile);
    cmdParser.handlePositional(shared::CommandLineParser::Positional::ExportToFile);
    cmdParser.process(a.arguments());

    const QVector<shared::CommandLineParser::Positional> args = cmdParser.positionalsSet();
    if (args.contains(shared::CommandLineParser::Positional::OpenXMLFile)
            && args.contains(shared::CommandLineParser::Positional::OpenStringTemplateFile)
            && args.contains(shared::CommandLineParser::Positional::ExportToFile)) {
        const QString inputFile = cmdParser.value(shared::CommandLineParser::Positional::OpenXMLFile);
        const QString templateFile = cmdParser.value(shared::CommandLineParser::Positional::OpenStringTemplateFile);
        const QString outputFile = cmdParser.value(shared::CommandLineParser::Positional::ExportToFile);

        dve::DVEditorCore dvcore;
        dve::DVAppModel *appModel = dvcore.appModel();

        const bool loadOk = appModel->load(inputFile);
        if (!loadOk) {
            qCritical() << "Unable to load file" << inputFile;
            return -1;
        }

        setupIvSystemQueriesIfAvailable(inputFile, &dvcore);

        const bool convertOk = dvcore.exporter()->exportObjectsSilently(appModel->objectsModel()->objects().values(),
                appModel->creatorGitHash(), outputFile, templateFile, appModel->requirementsURL());
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

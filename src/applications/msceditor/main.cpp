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

#include "asn1modelstorage.h"
#include "asn1systemchecks.h"
#include "commandlineparser.h"
#include "iveditor.h"
#include "ivlibrary.h"
#include "ivsystemchecks.h"
#include "mainmodel.h"
#include "mainwindow.h"
#include "msceditor.h"
#include "msceditorcore.h"
#include "msclibrary.h"
#include "mscproject.h"
#include "mscwriter.h"
#include "scversion.h"
#include "sharedlibrary.h"

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(asn1_resources);
    shared::initSharedLibrary();
    msc::initMscLibrary();
    msc::initMscEditor();
    ive::initIVEditor();
    ivm::initIVLibrary();

    QApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(spaceCreatorVersion);
    a.setApplicationName("MSC Editor");

    QDirIterator dirIt(":/fonts");
    while (dirIt.hasNext())
        QFontDatabase::addApplicationFont(dirIt.next());
    a.setFont(QFont(QLatin1String("Ubuntu"), 10));

    msc::MSCEditorCore editorCore;
    editorCore.setSystemChecker(new scs::IvSystemChecks(nullptr, &editorCore));

    msc::MscProject project;
    project.setModel(editorCore.mainModel());
    Asn1Acn::Asn1ModelStorage asnStorage;
    Asn1Acn::Asn1SystemChecks asnCheck;
    asnCheck.setAsn1Storage(&asnStorage);
    asnCheck.setProject(&project);

    editorCore.mainModel()->setAsn1Check(&asnCheck);

    shared::CommandLineParser cmdParser;
    editorCore.populateCommandLineArguments(&cmdParser);
    cmdParser.process(a.arguments());

    msc::MainWindow w(&editorCore);

    const QVector<shared::CommandLineParser::Positional> args = cmdParser.positionalsSet();
    for (auto it = args.crbegin(); it != args.crend(); ++it) {
        auto arg = *it;
        w.processCommandLineArg(arg, cmdParser.value(arg));
    }

    w.show();

    return a.exec();
}

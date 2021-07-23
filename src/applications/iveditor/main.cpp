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

#include "asn1modelstorage.h"
#include "asn1systemchecks.h"
#include "commandlineparser.h"
#include "interfacedocument.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivlibrary.h"
#include "ivproject.h"
#include "mainwindow.h"
#include "sharedlibrary.h"

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(asn1_resources);

    shared::initSharedLibrary();
    ive::initIVEditor();
    ivm::initIVLibrary();

    QApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(SC_VERSION);
    a.setApplicationName("IV Editor");

    QDirIterator dirIt(":/fonts");
    while (dirIt.hasNext())
        QFontDatabase::addApplicationFont(dirIt.next());
#ifdef Q_OS_OSX
    a.setFont(QFont(QLatin1String("Ubuntu"), 10));
#else
    a.setFont(QFont(QLatin1String("Ubuntu"), 8));
#endif

    ive::IVEditorCore editorCore;
    ive::IvProject project;
    project.setModel(editorCore.document());
    Asn1Acn::Asn1ModelStorage asnStorage;
    Asn1Acn::Asn1SystemChecks asnCheck;
    asnCheck.setAsn1Storage(&asnStorage);
    asnCheck.setProject(&project);

    editorCore.document()->setAsn1Check(&asnCheck);

    ive::MainWindow w(&editorCore);

    shared::CommandLineParser cmdParser;
    editorCore.populateCommandLineArguments(&cmdParser);
    cmdParser.process(a.arguments());

    const auto args = cmdParser.positionalsSet();
    for (auto arg : args) {
        w.processCommandLineArg(arg, cmdParser.value(arg));
    }

    if (!cmdParser.isSet(shared::CommandLineParser::Positional::ListScriptableActions)) {
        w.show();
    } else {
        return 0;
    }

    return a.exec();
}

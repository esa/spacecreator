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

#include "asn1library.h"
#include "commandlineparser.h"
#include "iveditor.h"
#include "iveditorcore.h"
#include "mainwindow.h"
#include "sharedlibrary.h"

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(asn1_resources);
    Asn1Acn::initAsn1Library();
    shared::initSharedLibrary();
    aadlinterface::initIvEditor();

    QApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(SC_VERSION);
    a.setApplicationName("IV Editor");

    QDirIterator dirIt(":/fonts");
    while (dirIt.hasNext())
        QFontDatabase::addApplicationFont(dirIt.next());
    a.setFont(QFont(QLatin1String("Ubuntu"), 10));

    aadlinterface::IVEditorCore plugin;

    aadlinterface::MainWindow w(&plugin);

    shared::CommandLineParser cmdParser;
    plugin.populateCommandLineArguments(&cmdParser);
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

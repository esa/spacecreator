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

#include "datatypesstorage.h"
#include "iveditorplugin.h"
#include "mainwindow.h"
#include "sharedlibrary.h"

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>
#include <QMetaEnum>
#include <QSslSocket>

int main(int argc, char *argv[])
{
    using namespace taste3;

    Q_INIT_RESOURCE(resources);
    Q_INIT_RESOURCE(tab_interface_resources);
    shared::initSharedLibrary();

    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("TASTE Editor 3.0"));

    aadl::DataTypesStorage::init();

    QDirIterator dirIt(":/fonts");
    while (dirIt.hasNext())
        QFontDatabase::addApplicationFont(dirIt.next());
    a.setFont(QFont(QLatin1String("Ubuntu"), 10));

    aadlinterface::IVEditorPlugin plugin;

    MainWindow w(&plugin);

    shared::CommandLineParser cmdParser;
    cmdParser.setApplicationDescription("TASTE editor");
    plugin.populateCommandLineArguments(&cmdParser);
    cmdParser.process(a.arguments());

    const auto args = cmdParser.positionalsSet();
    for (auto arg : args) {
        w.processCommandLineArg(arg, cmdParser.value(arg));
    }

    if (!cmdParser.isSet(shared::CommandLineParser::Positional::ListScriptableActions)) {
        w.show();
    }

    return a.exec();
}

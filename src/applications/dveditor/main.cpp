/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "dveditor.h"
#include "dveditorcore.h"
#include "dvlibrary.h"
#include "mainwindow.h"
#include "sharedlibrary.h"

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    shared::initSharedLibrary();
    dve::initDvEditor();
    dvm::initDVLibrary();

    QApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(SC_VERSION);
    a.setApplicationName("DV Editor");

    QDirIterator dirIt(":/fonts");
    while (dirIt.hasNext())
        QFontDatabase::addApplicationFont(dirIt.next());
#ifdef Q_OS_OSX
    a.setFont(QFont(QLatin1String("Ubuntu"), 10));
#else
    a.setFont(QFont(QLatin1String("Ubuntu"), 8));
#endif

    dve::DVEditorCore plugin;
    dve::MainWindow w(&plugin);
    w.show();

    return a.exec();
}

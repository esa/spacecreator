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
#include "msceditor.h"
#include "msceditorcore.h"
#include "msclibrary.h"
#include "scversion.h"
#include "sharedlibrary.h"
#include "streamingwindow.h"

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(asn1_resources);
    shared::initSharedLibrary();
    msc::initMscLibrary();
    msc::initMscEditor();

    QApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(spaceCreatorVersion);
    a.setApplicationName(QObject::tr("MSC Streaming"));

    QDirIterator dirIt(":/fonts");
    while (dirIt.hasNext())
        QFontDatabase::addApplicationFont(dirIt.next());
    a.setFont(QFont(QLatin1String("Ubuntu"), 10));

    msc::MSCEditorCore plugin;

    shared::CommandLineParser cmdParser;
    cmdParser.handlePositional(shared::CommandLineParser::Positional::StartRemoteControl);
    cmdParser.process(a.arguments());

    const QString portString = cmdParser.value(shared::CommandLineParser::Positional::StartRemoteControl);
    quint16 port = portString.toUShort();
    if (port <= 1000) {
        port = 34622;
    }

    msc::StreamingWindow window(&plugin);
    if (!window.startRemoteControl(port)) {
        return -1;
    }

    window.show();

    return a.exec();
}

/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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
#include "iveditor.h"
#include "ivstreamingwindow.h"
#include "scversion.h"
#include "sharedlibrary.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    shared::initSharedLibrary();
    ive::initIVEditor();

    QApplication a(argc, argv);
    a.setOrganizationName(SC_ORGANISATION);
    a.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    a.setApplicationVersion(spaceCreatorVersion);
    a.setApplicationName(QObject::tr("IV Streaming"));

    shared::CommandLineParser cmdParser;
    cmdParser.handlePositional(shared::CommandLineParser::Positional::StartRemoteControl);
    cmdParser.process(a.arguments());

    const QString portString = cmdParser.value(shared::CommandLineParser::Positional::StartRemoteControl);
    quint16 port = portString.toUShort();
    if (port <= 1000) {
        port = 34633;
        qDebug() << "Using default port" << port;
    }

    iv::IVStreamingWindow window;
    if (!window.startRemoteControl(port)) {
        return -1;
    }

    window.show();

    return a.exec();
}

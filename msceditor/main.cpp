/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mainwindow.h"
#include "commandlineparser.h"

#include <QApplication>
#include <QMetaEnum>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);

    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("MSC Editor"));
    a.setApplicationVersion("0.0.1");

    CommandLineParser cmdParser;
    cmdParser.process(a.arguments());

    MainWindow w;

    const QMetaEnum &e = QMetaEnum::fromType<CommandLineParser::Positional>();
    for (int i = 0; i < e.keyCount(); ++i) {
        const CommandLineParser::Positional posArgType(static_cast<CommandLineParser::Positional>(e.value(i)));

        if (CommandLineParser::Positional::Unknown != posArgType)
            if (cmdParser.isSet(posArgType))
                w.processCommandLineArg(posArgType, cmdParser.value(posArgType));
    }

    w.show();

    return a.exec();
}

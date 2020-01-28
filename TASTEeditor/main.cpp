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

#include "app/datatypes/datatypesstorage.h"
#include "app/mainwindow.h"
#include "logging/loghandler.h"
#include "reports/bugreporthandler.h"
#include "settings/settingsmanager.h"

#include <QApplication>
#include <QDirIterator>
#include <QFontDatabase>
#include <QMetaEnum>
#include <QSslSocket>

int main(int argc, char *argv[])
{
    using namespace taste3;
    LogHandler logHandler;

    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("TASTE Editor 3.0"));

    datatypes::DataTypesStorage::init();

    SettingsManager settings;

    QDirIterator dirIt(":/fonts");
    while (dirIt.hasNext())
        QFontDatabase::addApplicationFont(dirIt.next());
    a.setFont(QFont(QLatin1String("Ubuntu"), 10));

    MainWindow w;

    CommandLineParser cmdParser;
    cmdParser.process(a.arguments());

    QVector<CommandLineParser::Positional> args;
    const QMetaEnum &e = QMetaEnum::fromType<CommandLineParser::Positional>();
    for (int i = 0; i < e.keyCount(); ++i) {
        const CommandLineParser::Positional posArgType(static_cast<CommandLineParser::Positional>(e.value(i)));
        if (CommandLineParser::Positional::Unknown != posArgType) {
            if (cmdParser.isSet(posArgType)) {
                args.append(posArgType);
            }
        }
    }

    for (auto arg : args)
        w.processCommandLineArg(arg, cmdParser.value(arg));

    if (!cmdParser.isSet(CommandLineParser::Positional::ListScriptableActions))
        w.show();

    return a.exec();
}

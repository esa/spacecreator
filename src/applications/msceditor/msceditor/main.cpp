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
#include "mainwindow.h"
#include "mscdocument.h"

#include <QApplication>
#include <QDirIterator>
#include <QMetaEnum>
#include <QPlainTextEdit>
#include <QPointer>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(asn1_resources);
    Q_INIT_RESOURCE(resources);
    qRegisterMetaType<msc::MscDocument::HierarchyType>("HierarchyType");

    QApplication a(argc, argv);
    a.setApplicationName(QObject::tr("MSC Editor"));
    a.setApplicationVersion("0.0.1");

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
        if (CommandLineParser::Positional::Unknown != posArgType)
            if (cmdParser.isSet(posArgType)) {
                if (posArgType == CommandLineParser::Positional::OpenFileMsc)
                    args.prepend(posArgType);
                else
                    args.append(posArgType);
            }
    }
    QVector<CommandLineParser::Positional>::const_reverse_iterator it = args.crbegin();
    while (it != args.crend()) {
        const CommandLineParser::Positional arg = *it;
        w.processCommandLineArg(arg, cmdParser.value(arg));
        ++it;
    }

    w.show();

    return a.exec();
}

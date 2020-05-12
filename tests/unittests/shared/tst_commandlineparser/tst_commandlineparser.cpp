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
#include "mscplugin.h"

#include <QtTest>

using shared::CommandLineParser;

class tst_CommandLineParser : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    /*
     * A test slot name for each option must contain "testCmdArgument" -
     * it's used in the tst_CommandLineParser::testCoverage
     * to detected missed (untested) options.
     * For now it's possible to test all (both) options just by iterating
     * the CommandLineParser::Positional enum,
     * but I'm afraid some options will need some extra config,
     * so lets keep to this way - one option, one dedicated test slot.
     */
    void testCmdArgumentOpenMsc();
    void testCmdArgumentRemoteControl();

    void testCoverage();

private:
    msc::MSCPlugin m_plugin;
};

void tst_CommandLineParser::testCmdArgumentOpenMsc()
{
    const QCommandLineOption cmdOpenMsc = CommandLineParser::positionalArg(CommandLineParser::Positional::OpenFileMsc);
    const QString FileName(QString(EXAMPLES_STORAGE_PATH).append("example01.msc"));
    const QString NoFileName("./no-such-file.msc");

    CommandLineParser parser;
    m_plugin.populateCommandLineArguments(&parser);
    parser.process({ QApplication::instance()->applicationFilePath(),
            QString("-%1=%2").arg(cmdOpenMsc.names().first(), FileName) });

    QCOMPARE(parser.isSet(CommandLineParser::Positional::Unknown), false);
    QCOMPARE(parser.isSet(CommandLineParser::Positional::OpenFileMsc), true);

    const QString argFromParser1(parser.value(CommandLineParser::Positional::OpenFileMsc));
    QCOMPARE(argFromParser1, FileName);

    parser.process({ QApplication::instance()->applicationFilePath(),
            QString("-%1=%2").arg(cmdOpenMsc.names().first(), NoFileName) });
    QCOMPARE(parser.isSet(CommandLineParser::Positional::OpenFileMsc), true);
    const QString argFromParser2(parser.value(CommandLineParser::Positional::OpenFileMsc));
    QCOMPARE(argFromParser2, NoFileName);
}

void tst_CommandLineParser::testCmdArgumentRemoteControl()
{
    const QCommandLineOption cmdRemoteControl =
            CommandLineParser::positionalArg(CommandLineParser::Positional::StartRemoteControl);
    const quint16 inUsePort = 34568;
    const quint16 port = 34567;

    CommandLineParser parser;
    m_plugin.populateCommandLineArguments(&parser);
    parser.process({ QApplication::instance()->applicationFilePath(),
            QString("-%1=%2").arg(cmdRemoteControl.names().first(), QString::number(inUsePort)) });
    QCOMPARE(parser.isSet(CommandLineParser::Positional::StartRemoteControl), true);
    const QString argFromParser2(parser.value(CommandLineParser::Positional::StartRemoteControl));
    QCOMPARE(argFromParser2, QString::number(inUsePort));

    parser.process({ QApplication::instance()->applicationFilePath(),
            QString("-%1=%2").arg(cmdRemoteControl.names().first(), QString::number(port)) });

    QCOMPARE(parser.isSet(CommandLineParser::Positional::Unknown), false);
    QCOMPARE(parser.isSet(CommandLineParser::Positional::StartRemoteControl), true);

    const QString argFromParser1(parser.value(CommandLineParser::Positional::StartRemoteControl));
    QCOMPARE(argFromParser1.toUShort(), port);
}

void tst_CommandLineParser::testCoverage()
{
    const QMetaObject *metaMe = metaObject();
    QStringList testMethods;
    for (int i = metaMe->methodOffset(); i < metaMe->methodCount(); ++i) {
        const QString method = QString::fromLatin1(metaMe->method(i).methodSignature());
        if (method.startsWith("testCmdArgument"))
            testMethods << method;
    }

    const QMetaEnum &e = QMetaEnum::fromType<CommandLineParser::Positional>();
    int ignoredCommands(0);
    ++ignoredCommands; // CommandLineParser::PositionalArg::Unknown
    ++ignoredCommands; // CommandLineParser::PositionalArg::DbgOpenMscExamplesChain
    ++ignoredCommands; // CommandLineParser::PositionalArg::DropUnsavedChangesSilently

    QCOMPARE(testMethods.size(), e.keyCount() - ignoredCommands);
}

QTEST_MAIN(tst_CommandLineParser)

#include "tst_commandlineparser.moc"

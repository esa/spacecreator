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
#include "iveditorcore.h"
#include "mainwindow.h"
#include "msceditorcore.h"

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

    // The MSC only arguments. Skipping DbgOpenMscExamplesChain
    void testCmdArgumentOpenMsc();
    void testCmdArgumentRemoteControl();

    // The AADL only arguments
    void testCmdArgumentOpenAADLXMLFile();
    void testCmdArgumentListScriptableActions();

    // Arguments valid for both. Skipping Unknown and DropUnsavedChangesSilently
    void testCmdArgumentOpenStringTemplateFile();
    void testCmdArgumentExportToFile();

    void testCoverage();

private:
    msc::MSCEditorCore m_pluginMSC;
    aadlinterface::IVEditorCore m_pluginAADL;
};

void tst_CommandLineParser::testCmdArgumentOpenMsc()
{
    const QCommandLineOption cmdOpenMsc = CommandLineParser::positionalArg(CommandLineParser::Positional::OpenFileMsc);
    const QString FileName(QString(EXAMPLES_STORAGE_PATH).append("example01.msc"));
    const QString NoFileName("./no-such-file.msc");

    CommandLineParser parser;
    m_pluginMSC.populateCommandLineArguments(&parser);
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
    parser.handlePositional(shared::CommandLineParser::Positional::StartRemoteControl);
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

void tst_CommandLineParser::testCmdArgumentOpenAADLXMLFile()
{
    const QCommandLineOption cmdOpenAADLXML =
            CommandLineParser::positionalArg(CommandLineParser::Positional::OpenAADLXMLFile);
    const QString fileName(QString(EXAMPLES_STORAGE_PATH).append("sample.xml"));
    const QString noFileName("./no-such-file.xml");

    CommandLineParser parser;
    m_pluginAADL.populateCommandLineArguments(&parser);
    QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("-%1=%2").arg(cmdOpenAADLXML.names().first(), fileName) };
    parser.process(args);

    QVERIFY(!parser.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parser.isSet(CommandLineParser::Positional::OpenAADLXMLFile));

    const QString argFromParser1(parser.value(CommandLineParser::Positional::OpenAADLXMLFile));
    QCOMPARE(argFromParser1, fileName);

    parser.process({ QApplication::instance()->applicationFilePath(),
            QString("-%1=%2").arg(cmdOpenAADLXML.names().first(), noFileName) });
    QVERIFY(parser.isSet(CommandLineParser::Positional::OpenAADLXMLFile));
    const QString argFromParser2(parser.value(CommandLineParser::Positional::OpenAADLXMLFile));
    QCOMPARE(argFromParser2, noFileName);
}

void tst_CommandLineParser::testCmdArgumentListScriptableActions()
{
    const QCommandLineOption cmdListScriptableActions =
            CommandLineParser::positionalArg(CommandLineParser::Positional::ListScriptableActions);

    CommandLineParser parser;
    m_pluginAADL.populateCommandLineArguments(&parser);
    QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("-%1").arg(cmdListScriptableActions.names().first()) };
    parser.process(args);

    QVERIFY(!parser.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parser.isSet(CommandLineParser::Positional::ListScriptableActions));
}

void tst_CommandLineParser::testCmdArgumentOpenStringTemplateFile()
{
    const QCommandLineOption cmdOpenStringTemplateFile =
            CommandLineParser::positionalArg(CommandLineParser::Positional::OpenStringTemplateFile);
    const QString dirName(QString(EXAMPLES_STORAGE_PATH).append("project01"));
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("-%1=%2").arg(cmdOpenStringTemplateFile.names().first(), dirName) };

    CommandLineParser parserMSC;
    m_pluginAADL.populateCommandLineArguments(&parserMSC);
    parserMSC.process(args);

    QVERIFY(!parserMSC.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parserMSC.isSet(CommandLineParser::Positional::OpenStringTemplateFile));

    const QString argFromParserMSC(parserMSC.value(CommandLineParser::Positional::OpenStringTemplateFile));
    QCOMPARE(argFromParserMSC, dirName);

    CommandLineParser parserAADL;
    m_pluginAADL.populateCommandLineArguments(&parserAADL);
    parserAADL.process(args);

    QVERIFY(!parserAADL.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parserAADL.isSet(CommandLineParser::Positional::OpenStringTemplateFile));

    const QString argFromParserAADL(parserAADL.value(CommandLineParser::Positional::OpenStringTemplateFile));
    QCOMPARE(argFromParserAADL, dirName);
}

void tst_CommandLineParser::testCmdArgumentExportToFile()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandLineParser::Positional::ExportToFile);
    const QString fileName("outputfile");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("-%1=%2").arg(cmdExportToFile.names().first(), fileName) };

    CommandLineParser parserMSC;
    m_pluginAADL.populateCommandLineArguments(&parserMSC);
    parserMSC.process(args);

    QVERIFY(!parserMSC.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parserMSC.isSet(CommandLineParser::Positional::ExportToFile));

    const QString argFromParserMSC(parserMSC.value(CommandLineParser::Positional::ExportToFile));
    QCOMPARE(argFromParserMSC, fileName);

    CommandLineParser parserAADL;
    m_pluginAADL.populateCommandLineArguments(&parserAADL);
    parserAADL.process(args);

    QVERIFY(!parserAADL.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parserAADL.isSet(CommandLineParser::Positional::ExportToFile));

    const QString argFromParserAADL(parserAADL.value(CommandLineParser::Positional::ExportToFile));
    QCOMPARE(argFromParserAADL, fileName);
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

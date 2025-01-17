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
#include "iveditor.h"
#include "iveditorcore.h"
#include "ivlibrary.h"
#include "msceditorcore.h"
#include "sharedlibrary.h"

#include <QtTest>

using shared::CommandLineParser;

typedef shared::CommandLineParser::Positional CommandArg;

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

    // The IV only arguments
    void testCmdArgumentOpenIVFile();
    void testCmdArgumentListScriptableActions();

    // Arguments valid for both. Skipping Unknown and DropUnsavedChangesSilently
    void testCmdArgumentOpenStringTemplateFile();
    void testCmdArgumentExportToFile();

    // SedsConverter arguments
    void testCmdArgumentSedsConverterInputFilepaths();
    void testCmdArgumentSedsConverterOutputFilepath();
    void testCmdArgumentSedsConverterModelsFrom();
    void testCmdArgumentSedsConverterModelTo();
    void testCmdArgumentSedsConverterModelsAux();
    void testCmdArgumentSedsConverterIvConfig();
    void testCmdArgumentSedsConverterIvGenerateParentFunctions();
    void testCmdArgumentSedsConverterIvDataTypesSedsFilepath();
    void testCmdArgumentSedsConverterSedsPreprocessedFilepath();
    void testCmdArgumentSedsConverterSedsSchemaFilepath();
    void testCmdArgumentSedsConverterSedsExtRef();
    void testCmdArgumentSedsConverterSedsExtRefFilepath();
    void testCmdArgumentSedsConverterSkipValidation();
    void testCmdArgumentSedsConverterNoManglig();
    void testCmdArgumentSedsConverterMultipleAsnModels();
    void testCmdArgumentSedsConverterFunctionToConvert();
    void testCmdArgumentSedsConverterFlatPackage();
    void testCmdArgumentSkipEmptySequences();
    void testCmdArgumentTasteTranslation();
    void testCmdArgumentSedsConverterKeepIntermediateFiles();
    void testCmdArgumentSedsConverterAcnFilepathPrefix();
    void testCmdArgumentSedsConverterAsn1FilepathPrefix();
    void testCmdArgumentSedsConverterAsn1SequenceSizeThreshold();
    void testCmdArgumentSedsConverterPatcherFunctionsFilepathPrefix();
    void testCmdArgumentSedsConverterMappingFunctionsModuleFileName();
    void testCmdArgumentSedsConverterSdlFilepathPrefix();
    void testCmdArgumentSedsConverterTransactionNameType();
    void testCmdArgumentSedsEnableFailureReporting();
    void testCmdArgumentSedsConverterFailureReportingType();
    void testCmdArgumentSedsConverterArrayDimensionBaseIndexingType();

    void initTestCase();
    void testCoverage();

private:
    msc::MSCEditorCore m_pluginMSC;
    ive::IVEditorCore m_pluginIV;
};

void tst_CommandLineParser::testCmdArgumentOpenMsc()
{
    const QCommandLineOption cmdOpenMsc = CommandLineParser::positionalArg(CommandLineParser::Positional::OpenFileMsc);
    const QString FileName(QString(EXAMPLES_DIR).append("msc/example01.msc"));
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

void tst_CommandLineParser::testCmdArgumentOpenIVFile()
{
    const QCommandLineOption cmdOpenIV = CommandLineParser::positionalArg(CommandLineParser::Positional::OpenXMLFile);
    const QString fileName(QString(EXAMPLES_DIR).append("msc/sample.xml"));
    const QString noFileName("./no-such-file.xml");

    CommandLineParser parser;
    m_pluginIV.populateCommandLineArguments(&parser);
    QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("-%1=%2").arg(cmdOpenIV.names().first(), fileName) };
    parser.process(args);

    QVERIFY(!parser.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parser.isSet(CommandLineParser::Positional::OpenXMLFile));

    const QString argFromParser1(parser.value(CommandLineParser::Positional::OpenXMLFile));
    QCOMPARE(argFromParser1, fileName);

    parser.process({ QApplication::instance()->applicationFilePath(),
            QString("-%1=%2").arg(cmdOpenIV.names().first(), noFileName) });
    QVERIFY(parser.isSet(CommandLineParser::Positional::OpenXMLFile));
    const QString argFromParser2(parser.value(CommandLineParser::Positional::OpenXMLFile));
    QCOMPARE(argFromParser2, noFileName);
}

void tst_CommandLineParser::testCmdArgumentListScriptableActions()
{
    const QCommandLineOption cmdListScriptableActions =
            CommandLineParser::positionalArg(CommandLineParser::Positional::ListScriptableActions);

    CommandLineParser parser;
    m_pluginIV.populateCommandLineArguments(&parser);
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
    const QString dirName(QString(EXAMPLES_DIR).append("msc/project01"));
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("-%1=%2").arg(cmdOpenStringTemplateFile.names().first(), dirName) };

    CommandLineParser parserMSC;
    m_pluginIV.populateCommandLineArguments(&parserMSC);
    parserMSC.process(args);

    QVERIFY(!parserMSC.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parserMSC.isSet(CommandLineParser::Positional::OpenStringTemplateFile));

    const QString argFromParserMSC(parserMSC.value(CommandLineParser::Positional::OpenStringTemplateFile));
    QCOMPARE(argFromParserMSC, dirName);

    CommandLineParser parserIV;
    m_pluginIV.populateCommandLineArguments(&parserIV);
    parserIV.process(args);

    QVERIFY(!parserIV.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parserIV.isSet(CommandLineParser::Positional::OpenStringTemplateFile));

    const QString argFromParserIV(parserIV.value(CommandLineParser::Positional::OpenStringTemplateFile));
    QCOMPARE(argFromParserIV, dirName);
}

void tst_CommandLineParser::testCmdArgumentExportToFile()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandLineParser::Positional::ExportToFile);
    const QString fileName("outputfile");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("-%1=%2").arg(cmdExportToFile.names().first(), fileName) };

    CommandLineParser parserMSC;
    m_pluginIV.populateCommandLineArguments(&parserMSC);
    parserMSC.process(args);

    QVERIFY(!parserMSC.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parserMSC.isSet(CommandLineParser::Positional::ExportToFile));

    const QString argFromParserMSC(parserMSC.value(CommandLineParser::Positional::ExportToFile));
    QCOMPARE(argFromParserMSC, fileName);

    CommandLineParser parserIV;
    m_pluginIV.populateCommandLineArguments(&parserIV);
    parserIV.process(args);

    QVERIFY(!parserIV.isSet(CommandLineParser::Positional::Unknown));
    QVERIFY(parserIV.isSet(CommandLineParser::Positional::ExportToFile));

    const QString argFromParserIV(parserIV.value(CommandLineParser::Positional::ExportToFile));
    QCOMPARE(argFromParserIV, fileName);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterInputFilepaths()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterInputFilepaths);
    const QString fileName("input_file");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("-%1=%2").arg(cmdExportToFile.names().first(), fileName) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterInputFilepaths);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterInputFilepaths));

    const QString value = parser.value(CommandArg::SedsConverterInputFilepaths);
    QCOMPARE(value, fileName);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterOutputFilepath()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterOutputFilepath);
    const QString fileName("output_file");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), fileName) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterOutputFilepath);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterOutputFilepath));

    const QString value = parser.value(CommandArg::SedsConverterOutputFilepath);
    QCOMPARE(value, fileName);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterModelsFrom()
{
    const QCommandLineOption cmdExportToFile = CommandLineParser::positionalArg(CommandArg::SedsConverterModelsFrom);
    const QString models("model1,model2,model.3");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), models) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterModelsFrom);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterModelsFrom));

    const QString value = parser.value(CommandArg::SedsConverterModelsFrom);
    QCOMPARE(value, models);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterModelTo()
{
    const QCommandLineOption cmdExportToFile = CommandLineParser::positionalArg(CommandArg::SedsConverterModelTo);
    const QString models("model1,model2,model.3");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), models) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterModelTo);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterModelTo));

    const QString value = parser.value(CommandArg::SedsConverterModelTo);
    QCOMPARE(value, models);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterModelsAux()
{
    const QCommandLineOption cmdExportToFile = CommandLineParser::positionalArg(CommandArg::SedsConverterModelsAux);
    const QString models("model1,model2,model.3");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), models) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterModelsAux);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterModelsAux));

    const QString value = parser.value(CommandArg::SedsConverterModelsAux);
    QCOMPARE(value, models);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterIvConfig()
{
    const QCommandLineOption cmdExportToFile = CommandLineParser::positionalArg(CommandArg::SedsConverterIvConfig);
    const QString fileName("iv_config");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), fileName) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterIvConfig);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterIvConfig));

    const QString value = parser.value(CommandArg::SedsConverterIvConfig);
    QCOMPARE(value, fileName);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterIvGenerateParentFunctions()
{
    const QCommandLineOption cmdGenerateParentFunctions =
            CommandLineParser::positionalArg(CommandArg::SedsConverterIvGenerateParentFunctions);
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1").arg(cmdGenerateParentFunctions.names().first()) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterIvGenerateParentFunctions);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterIvGenerateParentFunctions));
}

void tst_CommandLineParser::testCmdArgumentSedsConverterIvDataTypesSedsFilepath()
{
    const QCommandLineOption cmdIvDataTypesSedsFilepath =
            CommandLineParser::positionalArg(CommandArg::SedsConverterIvDataTypesSedsFilepath);
    const QString fileName("datatypes.xml");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdIvDataTypesSedsFilepath.names().first(), fileName) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterIvDataTypesSedsFilepath);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterIvDataTypesSedsFilepath));

    const QString value = parser.value(CommandArg::SedsConverterIvDataTypesSedsFilepath);
    QCOMPARE(value, fileName);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterSedsPreprocessedFilepath()
{
    const QCommandLineOption cmdPreprocessedFilepath =
            CommandLineParser::positionalArg(CommandArg::SedsConverterSedsPreprocessedFilepath);
    const QString fileName("preprocessed.xml");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdPreprocessedFilepath.names().first(), fileName) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterSedsPreprocessedFilepath);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterSedsPreprocessedFilepath));

    const QString value = parser.value(CommandArg::SedsConverterSedsPreprocessedFilepath);
    QCOMPARE(value, fileName);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterSedsSchemaFilepath()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterSedsSchemaFilepath);
    const QString fileName("name.xsd");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), fileName) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterSedsSchemaFilepath);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterSedsSchemaFilepath));

    const QString value = parser.value(CommandArg::SedsConverterSedsSchemaFilepath);
    QCOMPARE(value, fileName);
}

/// \SRS  ETB-FUN-40
void tst_CommandLineParser::testCmdArgumentSedsConverterSedsExtRef()
{
    const QCommandLineOption cmdExportToFile = CommandLineParser::positionalArg(CommandArg::SedsConverterSedsExtRef);
    const QString externalReference("foo1:bar2,foo1:bar2");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), externalReference) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterSedsExtRef);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterSedsExtRef));

    const QString value = parser.value(CommandArg::SedsConverterSedsExtRef);
    QCOMPARE(value, externalReference);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterSedsExtRefFilepath()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterSedsExtRefFilepath);
    const QString externalReferenceFile("ext_ref_file");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), externalReferenceFile) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterSedsExtRefFilepath);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterSedsExtRefFilepath));

    const QString value = parser.value(CommandArg::SedsConverterSedsExtRefFilepath);
    QCOMPARE(value, externalReferenceFile);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterSkipValidation()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterSkipValidation);
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1").arg(cmdExportToFile.names().first()) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterSkipValidation);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterSkipValidation));
}

void tst_CommandLineParser::testCmdArgumentSedsConverterNoManglig()
{
    const QCommandLineOption cmdNoMangling = CommandLineParser::positionalArg(CommandArg::SedsConverterNoMangling);
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1").arg(cmdNoMangling.names().first()) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterNoMangling);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterNoMangling));
}

void tst_CommandLineParser::testCmdArgumentSedsConverterFunctionToConvert()
{
    const QCommandLineOption cmdFunctionToConvert =
            CommandLineParser::positionalArg(CommandArg::SedsConverterFunctionToConvert);
    const QString function("dummyFunction");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdFunctionToConvert.names().first(), function) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterFunctionToConvert);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterFunctionToConvert));

    const QString value = parser.value(CommandArg::SedsConverterFunctionToConvert);
    QCOMPARE(value, function);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterMultipleAsnModels()
{
    const QCommandLineOption cmdMultipleAsnModels =
            CommandLineParser::positionalArg(CommandArg::SedsConverterMultipleAsnModels);
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1").arg(cmdMultipleAsnModels.names().first()) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterMultipleAsnModels);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterMultipleAsnModels));
}

void tst_CommandLineParser::testCmdArgumentSedsConverterFlatPackage()
{
    const QCommandLineOption cmdFlatPackage =
            CommandLineParser::positionalArg(CommandArg::SedsConverterFlatPackage);
    const QString packageName("dummyPackage");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdFlatPackage.names().first(), packageName) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterFlatPackage);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterFlatPackage));

    const QString value = parser.value(CommandArg::SedsConverterFlatPackage);
    QCOMPARE(value, packageName);
}

void tst_CommandLineParser::testCmdArgumentSkipEmptySequences()
{
    const QCommandLineOption cmdSkipEmptySequences =
            CommandLineParser::positionalArg(CommandArg::SedsConverterSkipEmptySequences);
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1").arg(cmdSkipEmptySequences.names().first()) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterSkipEmptySequences);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterSkipEmptySequences));
}

void tst_CommandLineParser::testCmdArgumentTasteTranslation()
{
    const QCommandLineOption cmdTasteTranslation =
            CommandLineParser::positionalArg(CommandArg::SedsConverterTasteTranslation);
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1").arg(cmdTasteTranslation.names().first()) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterTasteTranslation);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterTasteTranslation));
}

void tst_CommandLineParser::testCmdArgumentSedsConverterKeepIntermediateFiles()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterKeepIntermediateFiles);
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1").arg(cmdExportToFile.names().first()) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterKeepIntermediateFiles);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterKeepIntermediateFiles));
}

void tst_CommandLineParser::testCmdArgumentSedsConverterAcnFilepathPrefix()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterAcnFilepathPrefix);
    const QString prefix("acn_filepath_preifx_");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), prefix) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterAcnFilepathPrefix);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterAcnFilepathPrefix));

    const QString value = parser.value(CommandArg::SedsConverterAcnFilepathPrefix);
    QCOMPARE(value, prefix);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterAsn1FilepathPrefix()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterAsn1FilepathPrefix);
    const QString prefix("asn1_filepath_prefix_");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), prefix) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterAsn1FilepathPrefix);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterAsn1FilepathPrefix));

    const QString value = parser.value(CommandArg::SedsConverterAsn1FilepathPrefix);
    QCOMPARE(value, prefix);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterAsn1SequenceSizeThreshold()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterAsn1SequenceSizeThreshold);
    const QString threshold("42");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), threshold) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterAsn1SequenceSizeThreshold);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterAsn1SequenceSizeThreshold));

    const QString value = parser.value(CommandArg::SedsConverterAsn1SequenceSizeThreshold);
    QCOMPARE(value, threshold);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterPatcherFunctionsFilepathPrefix()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterPatcherFunctionsFilepathPrefix);
    const QString prefix("patcher_function_prefix_");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), prefix) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterPatcherFunctionsFilepathPrefix);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterPatcherFunctionsFilepathPrefix));

    const QString value = parser.value(CommandArg::SedsConverterPatcherFunctionsFilepathPrefix);
    QCOMPARE(value, prefix);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterMappingFunctionsModuleFileName()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterMappingFunctionsModuleFileName);
    const QString prefix("mfm_module_name");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), prefix) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterMappingFunctionsModuleFileName);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterMappingFunctionsModuleFileName));

    const QString value = parser.value(CommandArg::SedsConverterMappingFunctionsModuleFileName);
    QCOMPARE(value, prefix);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterSdlFilepathPrefix()
{
    const QCommandLineOption cmdExportToFile =
            CommandLineParser::positionalArg(CommandArg::SedsConverterSdlFilepathPrefix);
    const QString prefix("sdl_filepath_prefix_");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdExportToFile.names().first(), prefix) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterSdlFilepathPrefix);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterSdlFilepathPrefix));

    const QString value = parser.value(CommandArg::SedsConverterSdlFilepathPrefix);
    QCOMPARE(value, prefix);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterTransactionNameType()
{
    const QCommandLineOption cmdTransactionNameType =
            CommandLineParser::positionalArg(CommandArg::SedsConverterTransactionNameType);
    const QString transactionNameType("CString");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdTransactionNameType.names().first(), transactionNameType) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterTransactionNameType);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterTransactionNameType));

    const QString value = parser.value(CommandArg::SedsConverterTransactionNameType);
    QCOMPARE(value, transactionNameType);
}

void tst_CommandLineParser::testCmdArgumentSedsEnableFailureReporting()
{
    const QCommandLineOption cmdEnableFailureReporting =
            CommandLineParser::positionalArg(CommandArg::SedsConverterEnableFailureReporting);
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1").arg(cmdEnableFailureReporting.names().first()) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterEnableFailureReporting);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterEnableFailureReporting));
}

void tst_CommandLineParser::testCmdArgumentSedsConverterFailureReportingType()
{
    const QCommandLineOption cmdFailureReportingType =
            CommandLineParser::positionalArg(CommandArg::SedsConverterFailureReportingType);
    const QString failureReportingType("CoolBool");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdFailureReportingType.names().first(), failureReportingType) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterFailureReportingType);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterFailureReportingType));

    const QString value = parser.value(CommandArg::SedsConverterFailureReportingType);
    QCOMPARE(value, failureReportingType);
}

void tst_CommandLineParser::testCmdArgumentSedsConverterArrayDimensionBaseIndexingType()
{
    const QCommandLineOption cmdArrayDimensionIndexType =
            CommandLineParser::positionalArg(CommandArg::SedsConverterArrayDimensionBaseIndexingType);
    const QString arrayDimensionIndexingType("MyIndexingType");
    const QStringList args = { QApplication::instance()->applicationFilePath(),
        QString("--%1=%2").arg(cmdArrayDimensionIndexType.names().first(), arrayDimensionIndexingType) };

    CommandLineParser parser;
    parser.handlePositional(CommandArg::SedsConverterArrayDimensionBaseIndexingType);
    parser.process(args);

    QVERIFY(!parser.isSet(CommandArg::Unknown));
    QVERIFY(parser.isSet(CommandArg::SedsConverterArrayDimensionBaseIndexingType));

    const QString value = parser.value(CommandArg::SedsConverterArrayDimensionBaseIndexingType);
    QCOMPARE(value, arrayDimensionIndexingType);
}

void tst_CommandLineParser::initTestCase()
{
    ivm::initIVLibrary();
    ive::initIVEditor();
    shared::initSharedLibrary();
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

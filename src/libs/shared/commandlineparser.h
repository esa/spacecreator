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

#pragma once

#include <QCommandLineParser>
#include <QObject>
#include <QVector>

namespace shared {

/*!
\class shared::CommandLineParser
\brief The processor of options passed as command line arguments.
*/
class CommandLineParser : public QCommandLineParser
{
    Q_GADGET
public:
    CommandLineParser();

    enum class Positional
    {
        // MSC editor
        OpenFileMsc = 0,
        DbgOpenMscExamplesChain,
        StartRemoteControl,

        // IV editor
        OpenXMLFile,
        ListScriptableActions,

        // MSC & IV
        OpenStringTemplateFile,
        ExportToFile,
        DropUnsavedChangesSilently,

        // Seds Converter
        SedsConverterInputFilepaths,
        SedsConverterOutputFilepath,
        SedsConverterModelsFrom,
        SedsConverterModelTo,
        SedsConverterModelsAux,
        SedsConverterIvConfig,
        SedsConverterIvGenerateParentFunctions,
        SedsConverterIvDataTypesSedsFilepath,
        SedsConverterSedsPreprocessedFilepath,
        SedsConverterSedsSchemaFilepath,
        SedsConverterSedsExtRef,
        SedsConverterSedsExtRefFilepath,
        SedsConverterSkipValidation,
        SedsConverterKeepIntermediateFiles,
        SedsConverterAcnFilepathPrefix,
        SedsConverterAsn1FilepathPrefix,
        SedsConverterAsn1SequenceSizeThreshold,
        SedsConverterPatcherFunctionsFilepathPrefix,
        SedsConverterMappingFunctionsModuleFileName,
        SedsConverterSdlFilepathPrefix,
        SedsConverterNoMangling,
        SedsConverterMultipleAsnModels,
        SedsConverterFunctionToConvert,
        SedsConverterFlatPackage,
        SedsConverterSkipEmptySequences,
        SedsConverterTasteTranslation,
        SedsConverterTransactionNameType,
        SedsConverterEnableFailureReporting,
        SedsConverterFailureReportingType,
        SedsConverterArrayDimensionBaseIndexingType,

        Unknown
    };
    Q_ENUM(Positional)

    bool isSet(CommandLineParser::Positional arg) const;
    QString value(CommandLineParser::Positional arg) const;

    void handlePositional(Positional arg);
    bool isPositionalHandled(Positional arg) const;

    QVector<Positional> positionalsSet() const;

    static QCommandLineOption positionalArg(CommandLineParser::Positional arg);

private:
    QVector<Positional> m_handledPositionals;
};

}

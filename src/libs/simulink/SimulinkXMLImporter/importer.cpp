/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "importer.h"
#include "exceptions.h"

#include <simulink/SimulinkModel/simulinkmodel.h>
#include <simulink/SimulinkOptions/options.h>
#include <conversion/common/import/exceptions.h>
#include <conversion/common/exceptions.h>

#include <QFileInfo>
#include <QXmlStreamReader>

using conversion::Options;
using conversion::FileNotFoundException;
using conversion::importer::ImportException;
using conversion::simulink::SimulinkOptions;

namespace simulink::importer {

std::unique_ptr<conversion::Model> SimulinkXmlImporter::importModel(const Options &options) const
{
    // preprocessor (UTF-8) ?
    // scheme validation ?

    const auto inputFilepath = options.value(SimulinkOptions::inputFilepath);
    if (!inputFilepath) {
        throw ImportException("File to import wasn't specified");
    }

    const QFileInfo inputFileInfo(*inputFilepath);
    if (!inputFileInfo.exists()) {
        throw FileNotFoundException(*inputFilepath, "while importing");
    }

    auto model = parse(*inputFilepath);

    // Cleanup working ?

    return model;
}

std::unique_ptr<conversion::Model> SimulinkXmlImporter::parse(const QString &inputSimulinkFilename)
{
    QFile inputSimulinkFile(inputSimulinkFilename);
    if (!inputSimulinkFile.open(QIODevice::ReadOnly)) {
        throw ParserException(inputSimulinkFile.errorString());
    }

    auto xmlReader = QXmlStreamReader(&inputSimulinkFile);

    xmlReader.readNextStartElement();

    if (xmlReader.name() == QLatin1String("ModelInterface")) {
        return std::make_unique<model::SimulinkModel>(readModelInterface(xmlReader)); 
    } else {
        throw UnhandledElement(xmlReader.name(), "Root");
    }
}

model::ModelInterface SimulinkXmlImporter::readModelInterface(QXmlStreamReader &xmlReader)
{
    model::ModelInterface modelInterface;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Workspace")) {
            modelInterface.setWorkspace(readWorkspace(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("Inports")) {
            modelInterface.setInports(readInports(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("Outports")) {
            modelInterface.setOutports(readOutports(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "ModelInterface");
        }
    }

    return modelInterface;
}

model::Workspace SimulinkXmlImporter::readWorkspace(QXmlStreamReader &xmlReader)
{
    model::Workspace workspace;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("WorkspaceElement")) {
            workspace.addWorkspaceElement(readWorkspaceElement(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Workspace");
        }
    }

    return workspace;
}

model::WorkspaceElement SimulinkXmlImporter::readWorkspaceElement(QXmlStreamReader &xmlReader)
{
    model::WorkspaceElement workspaceElement;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("Name")) {
            workspaceElement.setName(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("WorkspaceDataType")) {
            workspaceElement.setWorkspaceDataType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BaseType")) {
            workspaceElement.setBaseType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("DataScope")) {
            workspaceElement.setDataScope(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Description")) {
            workspaceElement.setDescription(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("HeaderFile")) {
            workspaceElement.setHeaderFile(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    if (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Members")) {
            workspaceElement.setMemberSet(readWorkspaceElementMembers(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "WorkspaceElement");
        }
    }

    return workspaceElement;
}

model::MemberSet SimulinkXmlImporter::readWorkspaceElementMembers(QXmlStreamReader &xmlReader)
{
    model::MemberSet memberSet;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Member")) {
            memberSet.addMember(readMember(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Members");
        }
    }

    return memberSet;
}

model::Member SimulinkXmlImporter::readMember(QXmlStreamReader &xmlReader)
{
    model::Member member;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("Name")) {
            member.setName(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("DataType")) {
            member.setDataType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Complexity")) {
            member.setComplexity(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Description")) {
            member.setDescription(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Dimensions")) {
            member.setDimensions(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("DimensionsMode")) {
            member.setDimensionsMode(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Max")) {
            member.setMax(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Min")) {
            member.setMin(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SampleTime")) {
            member.setSampleTime(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Unit")) {
            member.setUnit(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    return member;
}

model::Inports SimulinkXmlImporter::readInports(QXmlStreamReader &xmlReader)
{
    model::Inports inports;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Inport")) {
            inports.addInport(readInport(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Inports");
        }
    }

    return inports;
}

model::Inport SimulinkXmlImporter::readInport(QXmlStreamReader &xmlReader)
{
    model::Inport inport;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("Name")) {
            inport.setName(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutDataTypeStr")) {
            inport.setOutDataTypeStr(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BusObject")) {
            inport.setBusObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BusOutputAsStruct")) {
            inport.setBusOutputAsStruct(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("IconDisplay")) {
            inport.setIconDisplay(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Interpolate")) {
            inport.setInterpolate(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("LatchByDelayingOutsideSignal")) {
            inport.setLatchByDelayingOutsideSignal(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("LatchInputForFeedbackSignals")) {
            inport.setLatchInputForFeedbackSignals(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("LockScale")) {
            inport.setLockScale(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutMax")) {
            inport.setOutMax(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutMin")) {
            inport.setOutMin(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutputSignalNames")) {
            inport.setOutputSignalNames(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Port")) {
            inport.setPort(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("PortDimensions")) {
            inport.setPortDimensions(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SampleTime")) {
            inport.setSampleTime(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalType")) {
            inport.setSignalType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Unit")) {
            inport.setUnit(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("UnitNoProp")) {
            inport.setUnitNoProp(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("UseBusObject")) {
            inport.setUseBusObject(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    return inport;
}

model::Outports SimulinkXmlImporter::readOutports(QXmlStreamReader &xmlReader)
{
    model::Outports outports;

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Outport")) {
            outports.addOutport(readOutport(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "Outports");
        }
    }

    return outports;
}

model::Outport SimulinkXmlImporter::readOutport(QXmlStreamReader &xmlReader)
{
    model::Outport outport;

        for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("Name")) {
            outport.setName(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutDataTypeStr")) {
            outport.setOutDataTypeStr(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BusObject")) {
            outport.setBusObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("BusOutputAsStruct")) {
            outport.setBusOutputAsStruct(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("IconDisplay")) {
            outport.setIconDisplay(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("InitialOutput")) {
            outport.setInitialOutput(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("InputSignalNames")) {
            outport.setInputSignalNames(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("LockScale")) {
            outport.setLockScale(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("MustResolveToSignalObject")) {
            outport.setMustResolveToSignalObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutMax")) {
            outport.setOutMax(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutMin")) {
            outport.setOutMin(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("OutputWhenDisabled")) {
            outport.setOutputWhenDisabled(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Port")) {
            outport.setPort(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("PortDimensions")) {
            outport.setPortDimensions(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SampleTime")) {
            outport.setSampleTime(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalName")) {
            outport.setSignalName(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalObject")) {
            outport.setSignalObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("SignalType")) {
            outport.setSignalType(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("StorageClass")) {
            outport.setStorageClass(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("Unit")) {
            outport.setUnit(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("UnitNoProp")) {
            outport.setUnitNoProp(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("UseBusObject")) {
            outport.setUseBusObject(attribute.value().toString());
        } else if (attribute.name() == QStringLiteral("VarSizeSig")) {
            outport.setVarSizeSig(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    return outport;
}

} // namespace simulink::importer

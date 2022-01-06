/*
   Copyright (C) 2021-2022 GMV - <tiago.jorge@gmv.com>

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
#include "xmlwriter.h"

struct XmlWriter::IFConfig {
    QString maxScenarios { "" };
    QString errorScenarios { "" };
    QString successScenarios { "" };
    QString timeLimit { "" };
    QString maxEnvironmentCalls { "" };
    QString explorationAlgorithm { "" };
    QString maxStates { "" };
};

XmlWriter::XmlWriter(QStringList propertiesSelected, QStringList subtypesSelected, QStringList functionsSelected, QStringList ifConfiguration)
    : propertiesSelected(propertiesSelected), subtypesSelected(subtypesSelected), functionsSelected(functionsSelected), ifConfig(new IFConfig)
{
    xml.setAutoFormatting(true);

    ifConfig->maxScenarios = ifConfiguration.at(0);
    ifConfig->errorScenarios = ifConfiguration.at(1);
    ifConfig->successScenarios = ifConfiguration.at(2);
    ifConfig->timeLimit = ifConfiguration.at(3);
    ifConfig->maxEnvironmentCalls = ifConfiguration.at(4);
    ifConfig->explorationAlgorithm = ifConfiguration.at(5);
    ifConfig->maxStates = ifConfiguration.at(6);
}

bool XmlWriter::writeFile(QIODevice *device, QString fileName)
{
    xml.setDevice(device);

    xml.writeStartDocument();
    xml.writeDTD(QStringLiteral("<!DOCTYPE xml>"));
    xml.writeStartElement(QStringLiteral("xml"));
    //TODO use instead XmlReader::versionAttributes()
    xml.writeAttribute(QStringLiteral("version"), QStringLiteral("1.0"));

    xml.writeStartElement("ModelCheckingWindow");
    xml.writeAttribute("name", fileName);
    xml.writeStartElement("Configuration");

    xml.writeStartElement("Properties");
    for (int i = 0; i < propertiesSelected.size(); ++i){
            writeItem(propertiesSelected.at(i), "Property");
    }
    xml.writeEndElement();

    //for (int i = 0; i < subtypesSelected.size(); ++i){ // only one subtyping can be selected
    if (0 < subtypesSelected.size()) {writeItem(subtypesSelected.at(0), "Subtyping");}
    //}

    xml.writeStartElement("Submodel");
    for (int i = 0; i < functionsSelected.size(); ++i){
            writeItem(functionsSelected.at(i), "Function");
    }
    xml.writeEndElement();

    // closing Configuration
    xml.writeEndElement();

    //TODO write Native options
    xml.writeStartElement("NativeModelChecker");
    xml.writeEndElement();

    // write IF options
    xml.writeStartElement("IFModelChecker");
    xml.writeAttribute("maxscenarios", ifConfig->maxScenarios);
    xml.writeAttribute("errorscenarios", ifConfig->errorScenarios);
    xml.writeAttribute("successscenarios", ifConfig->successScenarios);
    xml.writeAttribute("timelimit", ifConfig->timeLimit);
    xml.writeAttribute("maxenvironmentcalls", ifConfig->maxEnvironmentCalls);
    xml.writeAttribute("explorationalgorithm", ifConfig->explorationAlgorithm);
    xml.writeAttribute("maxstates", ifConfig->maxStates);
    xml.writeEndElement();

    //TODO write SPIN options
    xml.writeStartElement("SPINModelChecker");
    xml.writeEndElement();

    // closing ModelCheckingWindow
    xml.writeEndElement();
    // closing xml
    xml.writeEndElement();

    xml.writeEndDocument();

    return true;
}

void XmlWriter::writeItem(QString str, QString type){
    xml.writeStartElement(type);
    if (type == "Property"){
        xml.writeAttribute("path", str);
    } else if (type == "Subtyping"){
        xml.writeAttribute("name", str);
    } else if (type == "Function"){
        xml.writeAttribute("name", str);
    }
    xml.writeEndElement();
}

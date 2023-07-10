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

#include "xmelreader.h"

#include "spinconfigsaver.h"

#include <QMessageBox>
#include <QObject>

struct XmelReader::IFConfig {
    QString maxScenarios { "" };
    QString errorScenarios { "" };
    QString successScenarios { "" };
    QString timeLimit { "" };
    QString maxEnvironmentCalls { "" };
    QString explorationAlgorithm { "" };
    QString timeRepresentation { "" };
    QString maxStates { "" };
};

XmelReader::XmelReader()
    : propertiesSelected({})
    , subtypesSelected({})
    , functionsSelected({})
    , ifConfig(new IFConfig)
{
}

int XmelReader::read(QIODevice *device)
{
    int ret = -2;
    xml.setDevice(device);

    if (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("xmel")
                && xml.attributes().value(QStringLiteral("version")) == QLatin1String("1.0")) {
            ret = readXMEL();
        } else {
            xml.raiseError(QObject::tr("The file is not a xmel version 1.0!"));
            ret = -3;
        }
    }

    return ret;
}

int XmelReader::readXMEL()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("xmel"));

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("ModelCheckingWindow")) {
            if (readModelCheckingWindow() != 0)
                return -1;
        } else {
            xml.skipCurrentElement();
        }
    }

    return 0;
}

int XmelReader::readModelCheckingWindow()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("ModelCheckingWindow"));

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("Configuration")) {
            if (readConfiguration() != 0)
                return -1;
        } else if (xml.name() == QLatin1String("NativeModelChecker")) {
            xml.skipCurrentElement();
        } else if (xml.name() == QLatin1String("IFModelChecker")) {
            if (!readIfModelChecker())
                return -2;
        } else if (xml.name() == QLatin1String("SPINModelChecker")) {
            if (!spinConfigSaver.readSpinConfig(xml))
                return -3;
        } else {
            xml.skipCurrentElement();
        }
    }

    return 0;
}

int XmelReader::readConfiguration()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("Configuration"));

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("Properties")) {
            if (!readProperties())
                return -1;
        } else if (xml.name() == QLatin1String("Subtyping")) {
            if (!readSubtyping())
                return -2;
        } else if (xml.name() == QLatin1String("Submodel")) {
            if (!readSubmodel())
                return -3;
        } else {
            xml.skipCurrentElement();
        }
    }

    return 0;
}

bool XmelReader::readProperties()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("Properties"));

    propertiesSelected.clear();

    while (xml.readNextStartElement()) {
        if (xml.name() == QLatin1String("Property")) {
            Q_ASSERT(xml.attributes().hasAttribute("path"));
            propertiesSelected.append(xml.attributes().value("path").toString());
            xml.skipCurrentElement();
        } else {
            xml.skipCurrentElement();
        }
    }

    return true;
}

bool XmelReader::readSubtyping()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("Subtyping"));

    subtypesSelected.clear();

    Q_ASSERT(xml.attributes().hasAttribute("name"));
    subtypesSelected.append(xml.attributes().value("name").toString());

    xml.skipCurrentElement();
    return true;
}

bool XmelReader::readSubmodel()
{
    bool hasFunction = false;

    while (xml.readNextStartElement()) {
        Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("Kept_Functions"));

        functionsSelected.clear();

        while (xml.readNextStartElement()) {
            if (xml.name() == QLatin1String("FunctionKept")){
                Q_ASSERT(xml.attributes().hasAttribute("name"));
                functionsSelected.append(xml.attributes().value("name").toString());
                hasFunction = true;
                xml.skipCurrentElement();
            } else {
                xml.skipCurrentElement();
            }
        }
    }

    return hasFunction;
}

bool XmelReader::readIfModelChecker()
{
    Q_ASSERT(xml.isStartElement() && xml.name() == QLatin1String("IFModelChecker"));

    ifConfig->maxScenarios = "";
    ifConfig->errorScenarios = "";
    ifConfig->successScenarios = "";
    ifConfig->timeLimit = "";
    ifConfig->maxEnvironmentCalls = "";
    ifConfig->explorationAlgorithm = "";
    ifConfig->maxStates = "";

    Q_ASSERT(xml.attributes().hasAttribute("maxscenarios"));
    Q_ASSERT(xml.attributes().hasAttribute("errorscenarios"));
    Q_ASSERT(xml.attributes().hasAttribute("successscenarios"));
    Q_ASSERT(xml.attributes().hasAttribute("timelimit"));
    Q_ASSERT(xml.attributes().hasAttribute("maxenvironmentcalls"));
    Q_ASSERT(xml.attributes().hasAttribute("explorationalgorithm"));
    Q_ASSERT(xml.attributes().hasAttribute("timerepresentation"));
    Q_ASSERT(xml.attributes().hasAttribute("maxstates"));
    ifConfig->maxScenarios = xml.attributes().value("maxscenarios").toString();
    ifConfig->errorScenarios = xml.attributes().value("errorscenarios").toString();
    ifConfig->successScenarios = xml.attributes().value("successscenarios").toString();
    ifConfig->timeLimit = xml.attributes().value("timelimit").toString();
    ifConfig->maxEnvironmentCalls = xml.attributes().value("maxenvironmentcalls").toString();
    ifConfig->explorationAlgorithm = xml.attributes().value("explorationalgorithm").toString();
    ifConfig->timeRepresentation = xml.attributes().value("timerepresentation").toString();
    ifConfig->maxStates = xml.attributes().value("maxstates").toString();

    xml.skipCurrentElement();
    return true;
}

QString XmelReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3").arg(xml.errorString()).arg(xml.lineNumber()).arg(xml.columnNumber());
}

QStringList XmelReader::getPropertiesSelected()
{
    return propertiesSelected;
}
QStringList XmelReader::getSubtypesSelected()
{
    return subtypesSelected;
}
QStringList XmelReader::getFunctionsSelected()
{
    return functionsSelected;
}
QStringList XmelReader::getIfConfig()
{
    QStringList res = {};

    res.append(ifConfig->maxScenarios);
    res.append(ifConfig->errorScenarios);
    res.append(ifConfig->successScenarios);
    res.append(ifConfig->timeLimit);
    res.append(ifConfig->maxEnvironmentCalls);
    res.append(ifConfig->explorationAlgorithm);
    res.append(ifConfig->timeRepresentation);
    res.append(ifConfig->maxStates);

    return res;
}

ive::SpinConfigData XmelReader::getSpinConfig()
{
    return spinConfigSaver.getConfigData();
}

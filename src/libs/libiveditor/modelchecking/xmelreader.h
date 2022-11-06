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

#ifndef XMELREADER_H
#define XMELREADER_H

#include "spinconfigsaver.h"

#include <QIODevice>
#include <QXmlStreamReader>

class XmelReader
{

public:
    XmelReader();

    int read(QIODevice *device);

    QString errorString() const;

    QStringList getPropertiesSelected();
    QStringList getSubtypesSelected();
    QStringList getFunctionsSelected();
    QStringList getIfConfig();
    ive::SpinConfigData getSpinConfig();

private:
    int readXMEL();
    int readConfiguration();
    int readModelCheckingWindow();
    bool readProperties();
    bool readSubtyping();
    bool readSubmodel();
    bool readIfModelChecker();

    QXmlStreamReader xml;

    QStringList propertiesSelected;
    QStringList subtypesSelected;
    QStringList functionsSelected;

    ive::SpinConfigSaver spinConfigSaver;

    struct IFConfig;
    IFConfig *ifConfig;
};

#endif // XMELREADER_H

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

#include "appoptions.h"

#include "interface/properties/dynamicpropertyconfig.h"

#include <QStandardPaths>

namespace aadlinterface {

GroupMainWindow::GroupMainWindow()
    : Geometry(QString("%1/Geometry").arg(localName()))
    , State(QString("%1/State").arg(localName()))
    , LastFilePath(QString("%1/LastFilePath").arg(localName()))
    , LastTab(QString("%1/LastTab").arg(localName()))
{
}

GroupMainWindow AppOptions::MainWindow = {};

GroupAadl::GroupAadl()
    : CustomPropertiesConfig(QString("%1/CustomPropsConf").arg(localName()))
    , DataTypesConfig(QString("%1/DataTypesConf").arg(localName()))
    , ColorSchemeFile(QString("%1/ColorSchemeFile").arg(localName()))
{
}

GroupAadl AppOptions::Aadl = {};

}

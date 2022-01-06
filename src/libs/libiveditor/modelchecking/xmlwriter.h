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
#ifndef XMLWRITER_H
#define XMLWRITER_H

#include "modelcheckingwindow.h"

#include <QTreeWidget>
#include <QXmlStreamWriter>

class XmlWriter
{
public:
    explicit XmlWriter(QStringList propertiesSelected, QStringList subtypesSelected, QStringList functionsSelected,  QStringList ifConfiguration);
    bool writeFile(QIODevice *device, QString fileName);

private:
    void writeItem(QString str, QString type);
    QXmlStreamWriter xml;
    //const QTreeWidget *treeWidget;
    QStringList propertiesSelected;
    QStringList subtypesSelected;
    QStringList functionsSelected;

    struct IFConfig;
    IFConfig *ifConfig;
};

#endif // XMLWRITER_H

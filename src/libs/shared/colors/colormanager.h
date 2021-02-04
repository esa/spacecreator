/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colorhandler.h"

#include <QMap>
#include <QObject>

namespace shared {

class ColorManager : public QObject
{
    Q_OBJECT
public:
    enum HandledColors
    {
        Unhandled = -1,

        FunctionRegular,
        FunctionRoot,
        FunctionPartial,
        FunctionType,
        Iface,
        Connection,
        Comment,
        ConnectionGroup,
        IfaceGroup,
        ConnectionFlow,
        FunctionScale,
    };
    Q_ENUM(HandledColors)

    static QString handledColorTypeName(HandledColors t);
    static ColorManager *instance();

    static QString defaultColorsResourceFile();

public:
    bool setSourceFile(const QString &from);
    QString sourceFile() const;
    bool save(const QString &fileName) const;

    ColorHandler colorsForItem(HandledColors t) const;
    QList<HandledColors> handledColors() const;

Q_SIGNALS:
    void colorsUpdated();

private:
    static ColorManager *m_instance;
    explicit ColorManager(QObject *parent = nullptr);
    QString m_filePath;

    QMap<HandledColors, ColorHandler> m_colors;

private:
    static const QString defaultColorschemeFileName;
    QString prepareDefaultSource() const;
};

}

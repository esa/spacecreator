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

#include <QBrush>
#include <QMap>
#include <QObject>
#include <QPen>

namespace taste3 {
namespace aadl {

struct ColorHandler {

    enum FillType
    {
        Color = 0,
        Gradient
    };

    static ColorHandler fromJson(const QJsonObject &jObj);
    QJsonObject toJson() const;

    QPen pen() const;
    QBrush brush() const;

    FillType m_fillType { FillType::Color };
    qreal m_penWidth { 1.0 };
    QColor m_penColor { Qt::black };
    QColor m_brushColor0 { Qt::black };
    QColor m_brushColor1 { Qt::white };
};

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
    };

    static QString handledColorTypeName(HandledColors t);
    static ColorManager *instance();

    static ColorHandler colorsForItem(HandledColors t);

public:
    bool setSourceFile(const QString &from);
    QString sourceFile() const;

Q_SIGNALS:
    void colorsUpdated();

private:
    static ColorManager *m_instance;
    explicit ColorManager(QObject *parent = nullptr);
    QString m_filePath;

    QMap<HandledColors, ColorHandler> m_colors;

private:
    QString prepareDefaultSource() const;
};

} // ns aadl
} // ns taste3

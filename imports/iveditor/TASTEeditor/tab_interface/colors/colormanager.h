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

namespace aadlinterface {

struct ColorHandlerData;

class ColorHandler
{
public:
    enum FillType
    {
        Color = 0,
        Gradient
    };

    ColorHandler();
    ColorHandler(const ColorHandler &other);

    QPen pen() const;
    QBrush brush() const;

    FillType fillType() const;
    void setFillType(FillType fillType);

    qreal penWidth() const;
    void setPenWidth(qreal width);

    QColor penColor() const;
    void setPenColor(const QColor &color);

    QColor brushColor0() const;
    void setBrushColor0(const QColor &color);

    QColor brushColor1() const;
    void setBrushColor1(const QColor &color);

    static ColorHandler fromJson(const QJsonObject &jObj);
    QJsonObject toJson() const;

private:
    QExplicitlySharedDataPointer<ColorHandlerData> d;
};

struct ColorHandlerData : public QSharedData
{
    ColorHandler::FillType fillType { ColorHandler::FillType::Color };
    qreal penWidth { 1.0 };
    QColor penColor { Qt::black };
    QColor brushColor0 { Qt::black };
    QColor brushColor1 { Qt::white };
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

    static QString defaultColorsResourceFile();

public:
    bool setSourceFile(const QString &from);
    QString sourceFile() const;

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

Q_DECLARE_TYPEINFO(aadlinterface::ColorHandler, Q_MOVABLE_TYPE);

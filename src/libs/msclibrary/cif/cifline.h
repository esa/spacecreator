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

#include <QMap>
#include <QObject>
#include <QPoint>
#include <QSharedPointer>
#include <QVariant>

namespace msc {
namespace cif {

class CifLine : public QObject
{
    Q_OBJECT

public:
    enum class CifType
    {
        Action = 0,
        Call,
        Comment,
        Condition,
        Create,
        Collapsed,
        End,
        HyperLink,
        Instance,
        Import,
        Keep,
        LastModified,
        Message,
        MscDocument,
        Modified,
        MscPageSize,
        Nested,
        Position,
        Preview,
        Reset,
        Set,
        Stop,
        Submsc,
        Specific,
        Text,
        Timeout,
        TextMode,
        TextName,
        GlobalComment,
        Concurrent,
        Unknown
    };
    Q_ENUM(CifType)

    CifLine(QObject *parent = nullptr);
    virtual ~CifLine();

    virtual CifType lineType() const = 0;
    virtual bool initFrom(const QString &sourceLine) = 0;
    virtual QVariant payload() const;
    virtual QVariant &payload();
    virtual void setPayload(const QVariant &p);

    static QString nameForType(CifLine::CifType t);
    static CifLine::CifType typeForName(const QString &name);

    bool operator==(const CifLine &other) const;

    QString sourceLine() const;

    virtual QString toString(int tabsSize) const;
    virtual QString payloadToString() const;

protected:
    QString m_sourceLine;
    QVariant m_payload;

    void setPayloadPoints(const QVector<QPoint> &points);

    static QPoint stringToPoint(const QString &from, bool *ok);
};

typedef QSharedPointer<CifLine> CifLineShared;

// utility class for lines with Point data
class CifLinePointsHolder : public CifLine
{
public:
    static const int AllPoints = -1;

    CifLinePointsHolder(int pointsCount = CifLinePointsHolder::AllPoints);
    bool initFrom(const QString &sourceLine) override;
    int targetPointsCount() const;

    QString payloadToString() const override;

private:
    const int m_pointsCount;
    bool initPoints(const QString &line);
};

class CifLineOnePointHolder : public CifLinePointsHolder
{
public:
    CifLineOnePointHolder()
        : CifLinePointsHolder(1)
    {
    }
};

class CifLineTwoPointsHolder : public CifLinePointsHolder
{
public:
    CifLineTwoPointsHolder()
        : CifLinePointsHolder(2)
    {
    }
};

class CifLineThreePointsHolder : public CifLinePointsHolder
{
public:
    CifLineThreePointsHolder()
        : CifLinePointsHolder(3)
    {
    }
};
} // ns cif
} // ns msc

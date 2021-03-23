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

#include "cif/cifblock.h"

#include <QObject>
#include <QRegularExpression>
#include <QVector>

namespace msc {
namespace cif {

class CifParser : public QObject
{
    Q_OBJECT
public:
    static const QLatin1String CifLineTag;

    explicit CifParser(QObject *parent = nullptr);

    CifLineShared readCifLine(const QString &line) const;
    QVector<CifBlockShared> readCifBlocks(const QStringList &lines);

private:
    static const QString m_typePattern;
    static const QRegularExpression m_typeRx;

    CifLine::CifType readCifType(const QString &from) const;

    CifLineShared readCifLineAction(const QString &from) const;
    CifLineShared readCifLineCall(const QString &from) const;
    CifLineShared readCifLineComment(const QString &from) const;
    CifLineShared readCifLineConcurrent(const QString &from) const;
    CifLineShared readCifLineCondition(const QString &from) const;
    CifLineShared readCifLineCreate(const QString &from) const;
    CifLineShared readCifLineCollapsed(const QString &from) const;
    CifLineShared readCifLineEnd(const QString &from) const;
    CifLineShared readCifLineHyperLink(const QString &from) const;
    CifLineShared readCifLineInstance(const QString &from) const;
    CifLineShared readCifLineImport(const QString &from) const;
    CifLineShared readCifLineKeep(const QString &from) const;
    CifLineShared readCifLineLastModified(const QString &from) const;
    CifLineShared readCifLineMessage(const QString &from) const;
    CifLineShared readCifLineMscDocument(const QString &from) const;
    CifLineShared readCifLineModified(const QString &from) const;
    CifLineShared readCifLineMscPageSize(const QString &from) const;
    CifLineShared readCifLineNested(const QString &from) const;
    CifLineShared readCifLinePosition(const QString &from) const;
    CifLineShared readCifLinePreview(const QString &from) const;
    CifLineShared readCifLineReset(const QString &from) const;
    CifLineShared readCifLineSet(const QString &from) const;
    CifLineShared readCifLineStop(const QString &from) const;
    CifLineShared readCifLineSubmsc(const QString &from) const;
    CifLineShared readCifLineSpecific(const QString &from) const;
    CifLineShared readCifLineText(const QString &from) const;
    CifLineShared readCifLineTimeout(const QString &from) const;
    CifLineShared readCifLineTextMode(const QString &from) const;
    CifLineShared readCifLineTextName(const QString &from) const;
    CifLineShared readCifLineGlobalComment(const QString &from) const;

    typedef QVector<CifLineShared> LinesCollection;
    QVector<CifParser::LinesCollection> prepareCifLines(const QStringList &lines) const;
};

} // ns cif
} // ns msc

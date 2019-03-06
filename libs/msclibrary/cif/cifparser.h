/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QMap>
#include <QObject>
#include <QRegularExpression>
#include <QSharedPointer>

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

    CifLineShared readEntityAction(const QString &from) const;
    CifLineShared readEntityCall(const QString &from) const;
    CifLineShared readEntityComment(const QString &from) const;
    CifLineShared readEntityCondition(const QString &from) const;
    CifLineShared readEntityCreate(const QString &from) const;
    CifLineShared readEntityCollapsed(const QString &from) const;
    CifLineShared readEntityEnd(const QString &from) const;
    CifLineShared readEntityHyperLink(const QString &from) const;
    CifLineShared readEntityInstance(const QString &from) const;
    CifLineShared readEntityImport(const QString &from) const;
    CifLineShared readEntityKeep(const QString &from) const;
    CifLineShared readEntityLastModified(const QString &from) const;
    CifLineShared readEntityMessage(const QString &from) const;
    CifLineShared readEntityMscDocument(const QString &from) const;
    CifLineShared readEntityModified(const QString &from) const;
    CifLineShared readEntityMscPageSize(const QString &from) const;
    CifLineShared readEntityNested(const QString &from) const;
    CifLineShared readEntityPosition(const QString &from) const;
    CifLineShared readEntityPreview(const QString &from) const;
    CifLineShared readEntityReset(const QString &from) const;
    CifLineShared readEntitySet(const QString &from) const;
    CifLineShared readEntityStop(const QString &from) const;
    CifLineShared readEntitySubmsc(const QString &from) const;
    CifLineShared readEntitySpecific(const QString &from) const;
    CifLineShared readEntityText(const QString &from) const;
    CifLineShared readEntityTimeout(const QString &from) const;
    CifLineShared readEntityTextMode(const QString &from) const;
    CifLineShared readEntityTextName(const QString &from) const;

    typedef QVector<CifLineShared> LinesCollection;
    QVector<CifParser::LinesCollection> prepareCifLines(const QStringList &lines) const;
};

} // ns cif
} // ns msc

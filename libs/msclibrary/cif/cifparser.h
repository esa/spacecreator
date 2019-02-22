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

#include "cif/cifentity.h"

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

    CifEntityShared readCIF(const QString &line) const;

private:
    static const QString m_typePattern;
    static const QRegularExpression m_typeRx;

    const QMap<QString, CifEntity::CifType> m_typesByName;
    const QMap<CifEntity::CifType, QString> m_namesByType;

    CifEntity::CifType readCifType(const QString &from) const;

    CifEntityShared readEntityAction(const QString &from) const;
    CifEntityShared readEntityCall(const QString &from) const;
    CifEntityShared readEntityComment(const QString &from) const;
    CifEntityShared readEntityCondition(const QString &from) const;
    CifEntityShared readEntityCreate(const QString &from) const;
    CifEntityShared readEntityCollapsed(const QString &from) const;
    CifEntityShared readEntityEnd(const QString &from) const;
    CifEntityShared readEntityHyperLink(const QString &from) const;
    CifEntityShared readEntityInstance(const QString &from) const;
    CifEntityShared readEntityImport(const QString &from) const;
    CifEntityShared readEntityKeep(const QString &from) const;
    CifEntityShared readEntityLastModified(const QString &from) const;
    CifEntityShared readEntityMessage(const QString &from) const;
    CifEntityShared readEntityMscDocument(const QString &from) const;
    CifEntityShared readEntityModified(const QString &from) const;
    CifEntityShared readEntityMscPageSize(const QString &from) const;
    CifEntityShared readEntityNested(const QString &from) const;
    CifEntityShared readEntityPosition(const QString &from) const;
    CifEntityShared readEntityPreview(const QString &from) const;
    CifEntityShared readEntityReset(const QString &from) const;
    CifEntityShared readEntitySet(const QString &from) const;
    CifEntityShared readEntityStop(const QString &from) const;
    CifEntityShared readEntitySubmsc(const QString &from) const;
    CifEntityShared readEntitySpecific(const QString &from) const;
    CifEntityShared readEntityText(const QString &from) const;
    CifEntityShared readEntityTimeout(const QString &from) const;
    CifEntityShared readEntityTextMode(const QString &from) const;
    CifEntityShared readEntityTextName(const QString &from) const;
};

} // ns cif
} // ns msc

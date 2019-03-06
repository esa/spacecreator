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

#include <QObject>
#include <QString>
#include <QUuid>

namespace msc {

class MscEntity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString comment READ comment WRITE setComment NOTIFY commentChanged)

public:
    enum class EntityType
    {
        Document = 0,
        Chart,
        Instance,
        Message,
        Timer,
        Gate,
        Condition,
        Action,
        Coregion,
        Create
    };
    Q_ENUM(EntityType)

    explicit MscEntity(QObject *parent = nullptr);
    explicit MscEntity(const QString &name, QObject *parent = nullptr);

    const QString &name() const;
    void setName(const QString &name);

    QUuid internalId() const;

    static const QString DefaultName;

    const QString &comment() const;
    void setComment(const QString &comment);

    virtual MscEntity::EntityType entityType() const = 0;

    QVector<cif::CifBlockShared> cifs() const;
    void setCifs(const QVector<cif::CifBlockShared> &cifs);
    void addCif(const cif::CifBlockShared &cif);

Q_SIGNALS:
    void dataChanged();
    void nameChanged(const QString &name);
    void commentChanged(const QString &name);

private:
    QString m_name = MscEntity::DefaultName;
    const QUuid m_id;
    QString m_comment;
    QVector<cif::CifBlockShared> m_cifs;

    void dbgShowCifs() const;
};

} // namespace msc

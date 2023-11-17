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
#include "cif/cifline.h"

#include <QByteArrayList>
#include <QPointer>
#include <QRegularExpression>
#include <QString>
#include <QUuid>
#include <QVector>

namespace msc {

class MscComment;

class MscEntity : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QObject *comment READ commentObj WRITE setCommentObj NOTIFY commentChanged)
    Q_PROPERTY(QString commentString READ commentString WRITE setCommentString NOTIFY commentChanged)
    Q_PROPERTY(msc::MscEntity::EntityType entityType READ entityType CONSTANT)
    Q_PROPERTY(QString cifText READ cifText NOTIFY cifTextChanged)

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
        Create,
        Comment
    };
    Q_ENUM(EntityType)

    explicit MscEntity(QObject *parent = nullptr);
    explicit MscEntity(const QString &name, QObject *parent = nullptr);

    const QString &name() const;
    void setName(const QString &name);

    static const QRegularExpression &nameVerifier();

    QUuid internalId() const;

    static const QString DefaultName;

    MscComment *comment() const;
    void setComment(MscComment *comment);
    QObject *commentObj() const;
    void setCommentObj(QObject *obj);

    QString commentString() const;
    MscComment *setCommentString(const QString &comment);

    virtual MscEntity::EntityType entityType() const = 0;
    QString entityTypeName() const;

    QVector<cif::CifBlockShared> cifs() const;
    void setCifs(const QVector<cif::CifBlockShared> &cifs);
    void addCif(const cif::CifBlockShared &cif);
    cif::CifBlockShared cifBlockByType(cif::CifLine::CifType type) const;
    void clearCifs();
    virtual QString cifText(int tabsSize = 0) const;

    /**
     * Returns the list of requirement IDs associated by this entity
     */
    QByteArrayList requirements() const;
    /**
     * Sets the list of requirement IDs associated by this entity
     */
    void setRequirements(const QByteArrayList &requirements);

#ifdef QT_DEBUG
    void dbgShowCifs() const;
#endif
    virtual QString toDbgString() const;

Q_SIGNALS:
    void dataChanged();
    void nameChanged(const QString &name);
    void commentChanged();
    void cifTextChanged();

private:
    cif::CifBlockShared requirementsCifBlock() const;
    QByteArrayList requirementsFromCifBlock(const cif::CifBlockShared &cif) const;

    QString m_name = MscEntity::DefaultName;
    const QUuid m_id;
    QPointer<MscComment> m_comment;
    QVector<cif::CifBlockShared> m_cifs;
    QByteArrayList m_requirements;

    static const QRegularExpression m_nameVerify;
};

}

Q_DECLARE_METATYPE(msc::MscEntity *)

QDebug operator<<(QDebug debug, const msc::MscEntity &entity);
QDebug operator<<(QDebug debug, const msc::MscEntity *entity);

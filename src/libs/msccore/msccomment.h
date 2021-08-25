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

#include "mscinstanceevent.h"

#include <QPointer>
#include <QRect>

namespace msc {

class MscComment : public MscInstanceEvent
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(bool isGlobal READ isGlobal NOTIFY isGlobalChanged)

public:
    explicit MscComment(QObject *parent = nullptr);

    MscEntity::EntityType entityType() const override;
    cif::CifLine::CifType mainCifType() const;
    bool isGlobal() const;

    QRect rect() const;
    void setRect(const QRect &rect);

    QString text() const;
    void setText(const QString &comment);

    MscEntity *attachedEntity() const;
    void attachTo(MscEntity *entity);

    bool relatesTo(const MscInstance *instance) const override;

    QString cifText(int tabsSize = 0) const override;

    QString toDbgString() const override;

Q_SIGNALS:
    void textChanged();
    void isGlobalChanged();

private:
    void updateCifComment();
    void onDataChanged();

private:
    QPointer<MscEntity> m_attachedEntity;
    QString m_text;
    QRect m_rect;
};

}

Q_DECLARE_METATYPE(msc::MscComment *)

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

#include "mscentity.h"

#include <QPoint>
#include <QVector>

namespace msc {

class MscInstance : public MscEntity
{
    Q_OBJECT
    Q_PROPERTY(QString denominator READ denominator WRITE setDenominator NOTIFY denominatorChanged)
    Q_PROPERTY(QString kind READ kind WRITE setKind NOTIFY kindChanged)
    Q_PROPERTY(QString denominatorAndKind READ denominatorAndKind WRITE setDenominatorAndKind NOTIFY
                    denominatorOrKindChanged)
    Q_PROPERTY(QString inheritance READ inheritance WRITE setInheritance NOTIFY inheritanceChanged)
    Q_PROPERTY(msc::MscInstance *explicitCreator READ explicitCreator WRITE setExplicitCreator NOTIFY
                    explicitCreatorChanged)
    Q_PROPERTY(bool explicitStop READ explicitStop WRITE setExplicitStop NOTIFY explicitStopChanged)

public:
    explicit MscInstance(QObject *parent = nullptr);
    MscInstance(const QString &name, QObject *parent = nullptr);

    const QString &denominator() const;
    void setDenominator(const QString &denominator);

    const QString &kind() const;
    void setKind(const QString &kind);

    QString denominatorAndKind() const;
    void setDenominatorAndKind(const QString &value);
    static void splitDenominatorKind(const QString &text, QString &denominator, QString &kind);

    const QString &inheritance() const;
    void setInheritance(const QString &inheritance);

    MscEntity::EntityType entityType() const override;

    MscInstance *explicitCreator() const;
    void setExplicitCreator(MscInstance *creator);
    bool isCreated() const;

    bool explicitStop() const;
    void setExplicitStop(bool stop);

    void setCifGeometry(const QVector<QPoint> &cifGeometry);
    QVector<QPoint> cifGeometry() const;

Q_SIGNALS:
    void denominatorChanged(const QString &denominator);
    void kindChanged(const QString &kind);
    void denominatorOrKindChanged(const QString &value);
    void inheritanceChanged(const QString &inheritance);
    void explicitCreatorChanged(msc::MscInstance *creator);
    void explicitStopChanged(bool value);
    void cifGeometryChanged();

private:
    QString m_denominator;
    QString m_kind;
    QString m_inheritance;
    MscInstance *m_explicitCreator = nullptr;
    bool m_explicitStop = false;

    cif::CifBlockShared cifInstance() const;
};

}

Q_DECLARE_METATYPE(msc::MscInstance *)
Q_DECLARE_METATYPE(QVector<msc::MscInstance *>)

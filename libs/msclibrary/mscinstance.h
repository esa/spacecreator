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

#ifndef MSCINSTANCE_H
#define MSCINSTANCE_H

#include "mscelement.h"

#include <QObject>
#include <QPointer>
#include <QString>
#include <QStringList>
#include <QVector>

namespace msc {

class MscMessage;

class MscInstance : public MscElement
{
    Q_OBJECT

public:
    using Messages = QVector<msc::MscMessage *>;

    explicit MscInstance(QObject *parent = nullptr);
    MscInstance(const QString &name, QObject *parent = nullptr);
    ~MscInstance();

    const QString &kind() const;
    void setKind(const QString &kind);

    const QString &inheritance() const;
    void setInheritance(const QString &inheritance);

    const Messages &messages() const;
    void addMessage(MscMessage *event);

Q_SIGNALS:
    void kindChanged(const QString &kind);
    void inheritanceChanged(const QString &inheritance);

private:
    QString m_kind;
    QString m_inheritance;
    Messages m_messages;
};

} // namespace msc

#endif // MSCINSTANCE_H

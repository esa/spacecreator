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

#include <QObject>
#include <QStringList>

namespace msc {

class MscMessageDeclaration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList names READ names WRITE setNames NOTIFY namesChanged)
    Q_PROPERTY(QStringList typeRefList READ typeRefList WRITE setTypeRefList NOTIFY typeRefListChanged)
public:
    explicit MscMessageDeclaration(QObject *parent = nullptr);

    const QStringList &names() const;
    const QStringList &typeRefList() const;

public Q_SLOTS:
    void setNames(const QStringList &names);
    void setTypeRefList(const QStringList &typeRefList);

Q_SIGNALS:
    void namesChanged();
    void typeRefListChanged();

private:
    QStringList m_names;
    QStringList m_typeRefList;
};

} // namespace msc

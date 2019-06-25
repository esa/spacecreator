/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "app/common.h"

#include <QObject>
#include <memory>

namespace taste3 {
namespace aadl {

struct AADLObjectPrivate;
class AADLObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(common::Id id READ id WRITE setId NOTIFY idChanged)

public:
    explicit AADLObject(const QString &title = QString(), QObject *parent = nullptr);
    virtual ~AADLObject();

    QString title() const;
    common::Id id() const;

signals:
    void titleChanged(const QString &title);
    void idChanged(const taste3::common::Id &id);

public slots:
    bool setTitle(const QString &title);
    bool setId(const common::Id &id);

protected:
    const std::unique_ptr<AADLObjectPrivate> d;
};

} // ns aadl
} // ns taste3

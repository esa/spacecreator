/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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
#include <QSharedPointer>
#include <QVector>

namespace dve {
class DVEditorCore;
}
namespace ive {
class IVEditorCore;
}
namespace msc {
class MSCEditorCore;
}

namespace scs {

class EditorCoreQuery : public QObject
{
    Q_OBJECT
public:
    EditorCoreQuery(QObject *parent = nullptr);
    virtual ~EditorCoreQuery();

    virtual QSharedPointer<ive::IVEditorCore> ivCore() const = 0;
    virtual QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores() const = 0;

Q_SIGNALS:
    void ivCoreAdded(QSharedPointer<ive::IVEditorCore> ivCore);
    void mscCoreAdded(QSharedPointer<msc::MSCEditorCore> mscCore);
};

} // namespace scs

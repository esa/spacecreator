/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "editorcorequery.h"

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <QStringList>
#include <QVector>

#pragma once

namespace dve {
class DVEditorCore;
}
namespace ive {
class IVEditorCore;
}
namespace msc {
class MSCEditorCore;
}
namespace shared {
class EditorCore;
}

namespace spctr {

/*!
   Stores shared pointers to all aadl/msc file objects. And creates a new one if needed
 */
class ModelStorage : public scs::EditorCoreQuery
{
    Q_OBJECT

public:
    explicit ModelStorage(QObject *parent = nullptr);

    // Load / access functions
    QSharedPointer<dve::DVEditorCore> dvData(const QString &fileName) const;
    QSharedPointer<ive::IVEditorCore> ivData(const QString &fileName) const;
    QSharedPointer<msc::MSCEditorCore> mscData(const QString &fileName) const;

    void remove(const QString &fileName);

    // Query functions
    QSharedPointer<ive::IVEditorCore> ivCore() const override;
    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores() const override;

    bool contains(QSharedPointer<shared::EditorCore> core) const;

    static QStringList allAadlFiles();
    static QStringList allMscFiles();
    static QStringList allAsn1Files();
    static QStringList projectFiles(const QString &suffix);

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);

private:
    void setIvData(const QString &fileName, QSharedPointer<ive::IVEditorCore> ivData);
    void setMscData(const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData);

    QHash<QString, QSharedPointer<ive::IVEditorCore>> m_ivStore;
    QHash<QString, QSharedPointer<msc::MSCEditorCore>> m_mscStore;
};

}

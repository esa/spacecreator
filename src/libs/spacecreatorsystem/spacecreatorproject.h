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
namespace shared {
class EditorCore;
}

namespace scs {

class SpaceCreatorProject : public QObject
{
    Q_OBJECT
public:
    SpaceCreatorProject(QObject *parent = nullptr);
    virtual ~SpaceCreatorProject();

    // Load / access functions
    QSharedPointer<dve::DVEditorCore> dvData(const QString &fileName) const;
    QSharedPointer<ive::IVEditorCore> ivData(const QString &fileName) const;
    QSharedPointer<msc::MSCEditorCore> mscData(const QString &fileName) const;

    void remove(const QString &fileName);

    // Query functions
    virtual QSharedPointer<ive::IVEditorCore> ivCore() const;
    virtual QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores() const;

    bool contains(QSharedPointer<shared::EditorCore> core) const;

    virtual QStringList allAadlFiles() const;
    virtual QStringList allMscFiles() const;
    virtual QStringList allAsn1Files() const;
    virtual QStringList projectFiles(const QString &suffix) const;

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);
    void ivCoreAdded(QSharedPointer<ive::IVEditorCore> ivCore);
    void mscCoreAdded(QSharedPointer<msc::MSCEditorCore> mscCore);

private:
    void setIvData(const QString &fileName, QSharedPointer<ive::IVEditorCore> ivData);
    void setMscData(const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData);

    QHash<QString, QSharedPointer<ive::IVEditorCore>> m_ivStore;
    QHash<QString, QSharedPointer<msc::MSCEditorCore>> m_mscStore;
};

} // namespace scs

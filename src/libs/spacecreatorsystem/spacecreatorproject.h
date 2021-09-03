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

#include "abstractproject.h"
#include "dveditorcore.h"
#include "iveditorcore.h"
#include "msceditorcore.h"

#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include <memory>

namespace Asn1Acn {
class Asn1ModelStorage;
class Asn1SystemChecks;
}
namespace shared {
class EditorCore;
}

namespace scs {
class DvSystemChecks;
class IvSystemChecks;
class MscSystemChecks;
class IvSystemQueries;

/*!
   \brief Contains all data of project (of all files).

   - Has functions to query for all space creator related data and files.
   - Contains checks for the system
 */
class SpaceCreatorProject : public shared::AbstractProject
{
    Q_OBJECT
public:
    SpaceCreatorProject(QObject *parent = nullptr);
    virtual ~SpaceCreatorProject();

    // Load / access functions
    DVEditorCorePtr dvData(const QString &fileName) const;
    IVEditorCorePtr ivData(const QString &fileName) const;
    MSCEditorCorePtr mscData(const QString &fileName) const;

    // Query functions
    virtual IVEditorCorePtr ivCore() const;
    virtual QVector<DVEditorCorePtr> allDVCores() const;
    virtual QVector<MSCEditorCorePtr> allMscCores() const;

    bool contains(QSharedPointer<shared::EditorCore> core) const;

    QStringList allDVFiles() const override;
    QStringList allIVFiles() const override;
    QStringList allMscFiles() const override;
    QStringList allAsn1Files() const override;
    virtual QStringList projectFiles(const QString &suffix) const;

    // System checks
    Asn1Acn::Asn1SystemChecks *asn1Checks() const;
    /*!
       Access to the msc checks done from iv
       */
    scs::MscSystemChecks *mscChecks() const { return m_mscChecks.get(); }
    QVector<scs::IvSystemChecks *> ivChecks() const;
    QVector<scs::IvSystemQueries *> ivQueries() const;
    scs::IvSystemQueries *ivQuery() const;
    // Access to check for the DV files
    scs::DvSystemChecks *dvChecks() const;

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);
    void dvCoreAdded(DVEditorCorePtr dvCore);
    void ivCoreAdded(IVEditorCorePtr ivCore);
    void mscCoreAdded(MSCEditorCorePtr mscCore);

protected Q_SLOTS:
    void purgeNonProjectData();

protected:
    void setDvData(const QString &fileName, DVEditorCorePtr dvData);
    void setIvData(const QString &fileName, IVEditorCorePtr ivData);
    void setMscData(const QString &fileName, MSCEditorCorePtr mscData);

    QHash<QString, DVEditorCorePtr> m_dvStore;
    QHash<QString, IVEditorCorePtr> m_ivStore;
    QHash<QString, MSCEditorCorePtr> m_mscStore;
    std::unique_ptr<scs::MscSystemChecks> m_mscChecks;
    std::unique_ptr<scs::DvSystemChecks> m_dvChecks;
    std::unique_ptr<Asn1Acn::Asn1ModelStorage> m_asn1Storage;
    std::unique_ptr<Asn1Acn::Asn1SystemChecks> m_asnChecks;
};

} // namespace scs

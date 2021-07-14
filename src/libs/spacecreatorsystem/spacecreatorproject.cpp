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

#include "spacecreatorproject.h"

#include "asn1modelstorage.h"
#include "asn1systemchecks.h"
#include "baseitems/common/ivutils.h"
#include "common.h"
#include "dvappmodel.h"
#include "dveditorcore.h"
#include "dvsystemchecks.h"
#include "errorhub.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "ivsystemchecks.h"
#include "mainmodel.h"
#include "msceditorcore.h"
#include "mscsystemchecks.h"

#include <QDebug>
#include <QMessageBox>

namespace scs {

SpaceCreatorProject::SpaceCreatorProject(QObject *parent)
    : AbstractProject(parent)
    , m_mscChecks(new MscSystemChecks)
    , m_asn1Storage(new Asn1Acn::Asn1ModelStorage)
    , m_asnChecks(new Asn1Acn::Asn1SystemChecks)
{
    m_mscChecks->setStorage(this);

    m_asnChecks->setAsn1Storage(m_asn1Storage.get());
    m_asnChecks->setProject(this);
}

SpaceCreatorProject::~SpaceCreatorProject() { }

/*!
   Returns the DVEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<dve::DVEditorCore> SpaceCreatorProject::dvData(const QString &fileName) const
{
    if (!m_dvStore.contains(fileName)) {
        QSharedPointer<dve::DVEditorCore> data(new dve::DVEditorCore());
        data->loadHWLibrary(shared::hwLibraryPath());
        data->appModel()->load(fileName);
        const_cast<SpaceCreatorProject *>(this)->setDvData(fileName, data);
        return data;
    }

    return m_dvStore[fileName];
}

/*!
   Returns the IVEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<ive::IVEditorCore> SpaceCreatorProject::ivData(const QString &fileName) const
{
    if (!m_ivStore.contains(fileName)) {
        QSharedPointer<ive::IVEditorCore> data(new ive::IVEditorCore());
        data->registerBasicActions();
        data->document()->customActions(); // There some further actions are registered
        data->document()->setAsn1Check(m_asnChecks.get());

        data->document()->load(fileName);
        const_cast<SpaceCreatorProject *>(this)->setIvData(fileName, data);
        return data;
    }

    return m_ivStore[fileName];
}

/*!
   Returns the MSCEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<msc::MSCEditorCore> SpaceCreatorProject::mscData(const QString &fileName) const
{
    if (!m_mscStore.contains(fileName)) {
        QSharedPointer<msc::MSCEditorCore> data(new msc::MSCEditorCore());
        data->showToolbars(false);
        data->mainModel()->setAsn1Check(m_asnChecks.get());
        data->mainModel()->loadFile(fileName);
        const_cast<SpaceCreatorProject *>(this)->setMscData(fileName, data);
        return data;
    }

    return m_mscStore[fileName];
}

/*!
   Returns the iv data of the project
 */
QSharedPointer<ive::IVEditorCore> SpaceCreatorProject::ivCore() const
{
    const QStringList ivFiles = allIVFiles();
    if (ivFiles.empty()) {
        shared::ErrorHub::addError(shared::ErrorItem::Warning, tr("No IV file in the project"));
        return {};
    }

    return ivData(ivFiles.first());
}

/*!
   Returns all deployment view objects, that are used in the current project
 */
QVector<QSharedPointer<dve::DVEditorCore>> SpaceCreatorProject::allDVCores() const
{
    const QStringList dvFiles = allDVFiles();
    QVector<QSharedPointer<dve::DVEditorCore>> allDvCores;
    for (const QString &dvFile : dvFiles) {
        QSharedPointer<dve::DVEditorCore> core = dvData(dvFile);
        if (core) {
            allDvCores.append(core);
        }
    }
    return allDvCores;
}

/*!
   Returns all MSCEditorCore objects, that are used in the current project
 */
QVector<QSharedPointer<msc::MSCEditorCore>> SpaceCreatorProject::allMscCores() const
{
    const QStringList mscFiles = allMscFiles();
    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores;
    for (const QString &mscFile : mscFiles) {
        QSharedPointer<msc::MSCEditorCore> core = mscData(mscFile);
        if (core) {
            allMscCores.append(core);
        }
    }
    return allMscCores;
}

/*!
   Returns if the given \p core is in this storage
 */
bool SpaceCreatorProject::contains(QSharedPointer<shared::EditorCore> core) const
{
    for (QSharedPointer<dve::DVEditorCore> dvCore : m_dvStore) {
        if (core == dvCore) {
            return true;
        }
    }
    for (QSharedPointer<ive::IVEditorCore> ivCore : m_ivStore) {
        if (core == ivCore) {
            return true;
        }
    }
    for (QSharedPointer<msc::MSCEditorCore> mscCore : m_mscStore) {
        if (core == mscCore) {
            return true;
        }
    }
    return false;
}

/*!
   Returns all deployment view files of the current project
 */
QStringList SpaceCreatorProject::allDVFiles() const
{
    return projectFiles(".dv.xml");
}

/*!
   Returns all aald files of the current project
 */
QStringList SpaceCreatorProject::allIVFiles() const
{
    return projectFiles(shared::kDefaultInterfaceViewFileName);
}

/*!
   Returns all msc files of the current project
 */
QStringList SpaceCreatorProject::allMscFiles() const
{
    return projectFiles(".msc");
}

/*!
   Returns all asn files of the current project
 */
QStringList SpaceCreatorProject::allAsn1Files() const
{
    return projectFiles(".asn");
}

/*!
   \brief SpaceCreatorProject::projectFiles
   \param suffix
   \return
 */
QStringList SpaceCreatorProject::projectFiles(const QString &suffix) const
{
    QStringList files;
    for (QSharedPointer<ive::IVEditorCore> ivCore : m_ivStore) {
        if (ivCore->filePath().endsWith(suffix)) {
            files.append(ivCore->filePath());
        }
    }
    for (const QSharedPointer<msc::MSCEditorCore> &mscCore : m_mscStore) {
        if (mscCore->filePath().endsWith(suffix)) {
            files.append(mscCore->filePath());
        }
    }
    return files;
}

/*!
   Returns the Asn1SystemChecks object to access project wide asn1 data
 */
Asn1Acn::Asn1SystemChecks *SpaceCreatorProject::asn1Checks() const
{
    return m_asnChecks.get();
}

/*!
   Access to the list of iv checks done from msc
 */
QVector<IvSystemChecks *> SpaceCreatorProject::ivChecks() const
{
    QVector<IvSystemChecks *> checks;
    for (const QSharedPointer<msc::MSCEditorCore> &core : m_mscStore) {
        if (auto ivChecker = qobject_cast<scs::IvSystemChecks *>(core->systemChecker())) {
            checks.append(ivChecker);
        }
    }
    return checks;
}

QVector<DvSystemChecks *> SpaceCreatorProject::dvChecks() const
{
    QVector<DvSystemChecks *> checks;
    for (const QSharedPointer<dve::DVEditorCore> &core : m_dvStore) {
        if (auto dvChecker = qobject_cast<scs::DvSystemChecks *>(core->systemChecker())) {
            checks.append(dvChecker);
        }
    }
    return checks;
}

/*!
   Removes all data that is stored here, but is not part of the project
 */
void SpaceCreatorProject::purgeNonProjectData()
{
    const QStringList ivFiles = allIVFiles();
    auto ivIt = m_ivStore.begin();
    while (ivIt != m_ivStore.end()) {
        if (!ivFiles.contains(ivIt.key())) {
            ivIt = m_ivStore.erase(ivIt);
        } else {
            ++ivIt;
        }
    }

    const QStringList mscFiles = allMscFiles();
    auto mscIt = m_mscStore.begin();
    while (mscIt != m_mscStore.end()) {
        if (!mscFiles.contains(mscIt.key())) {
            mscIt = m_mscStore.erase(mscIt);
        } else {
            ++mscIt;
        }
    }
}

/*!
   Sets the DVEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void SpaceCreatorProject::setDvData(const QString &fileName, QSharedPointer<dve::DVEditorCore> dvData)
{
    const QString oldKey = m_dvStore.key(dvData, "");
    if (!oldKey.isEmpty()) {
        if (m_dvStore[fileName] == dvData) {
            return;
        }
        QSharedPointer<dve::DVEditorCore> oldData = m_dvStore.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_dvStore[fileName] = dvData;
    connect(dvData.data(), &shared::EditorCore::editedExternally, this, &scs::SpaceCreatorProject::editedExternally);
    auto checker = new scs::DvSystemChecks(dvData.data());
    checker->setDVCore(dvData.data());
    checker->setIVCore(ivCore());
    dvData->setSystemChecker(checker);
    Q_EMIT dvCoreAdded(dvData);
}

/*!
   Sets the IVEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void SpaceCreatorProject::setIvData(const QString &fileName, QSharedPointer<ive::IVEditorCore> ivData)
{
    const QString oldKey = m_ivStore.key(ivData, "");
    if (!oldKey.isEmpty()) {
        if (m_ivStore[fileName] == ivData) {
            return;
        }
        QSharedPointer<ive::IVEditorCore> oldData = m_ivStore.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_ivStore[fileName] = ivData;
    connect(ivData.data(), &shared::EditorCore::editedExternally, this, &scs::SpaceCreatorProject::editedExternally);
    Q_EMIT ivCoreAdded(ivData);
}

/*!
   Sets the MSCEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void SpaceCreatorProject::setMscData(const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData)
{
    const QString oldKey = m_mscStore.key(mscData, "");
    if (!oldKey.isEmpty()) {
        if (m_mscStore[fileName] == mscData) {
            return;
        }
        QSharedPointer<msc::MSCEditorCore> oldData = m_mscStore.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_mscStore[fileName] = mscData;
    connect(mscData.data(), &shared::EditorCore::editedExternally, this, &scs::SpaceCreatorProject::editedExternally);
    auto checker = new scs::IvSystemChecks(mscData.data());
    checker->setMscCore(mscData.data());
    checker->setIvCore(ivCore());
    mscData->setSystemChecker(checker);
    Q_EMIT mscCoreAdded(mscData);
}

} // namespace scs

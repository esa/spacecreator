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
#include "commandsstack.h"
#include "common.h"
#include "dvappmodel.h"
#include "dveditorcore.h"
#include "dvrefactor.h"
#include "dvsystemqueries.h"
#include "errorhub.h"
#include "interfacedocument.h"
#include "iveditorcore.h"
#include "ivfunction.h"
#include "ivrefactorhandler.h"
#include "ivsystemchecks.h"
#include "ivsystemqueries.h"
#include "mainmodel.h"
#include "mscmodel.h"
#include "mscrefactor.h"
#include "mscrefactorhandler.h"

#include <QDebug>
#include <QMessageBox>

namespace scs {

static const QString GLOBAL_ASN_FILE = { "dataview-uniq.asn" };

SpaceCreatorProject::SpaceCreatorProject(QObject *parent)
    : AbstractProject(parent)
    , m_mscChecks(new MscRefactorHandler)
    , m_dvChecks(new DvSystemQueries)
    , m_asn1Storage(new Asn1Acn::Asn1ModelStorage)
    , m_asnChecks(new Asn1Acn::Asn1SystemChecks)
    , m_ivRefactorHandler(std::make_unique<scs::IVRefactorHandler>())
    , m_mscRefactor(std::make_unique<scs::MSCRefactor>())
    , m_dvRefactor(std::make_unique<scs::DVRefactor>())
{
    m_mscChecks->setStorage(this);
    m_dvChecks->setStorage(this);

    m_asnChecks->setAsn1Storage(m_asn1Storage.get());
    m_asnChecks->setProject(this);

    m_mscRefactor->setStorage(this);
    m_ivRefactorHandler->registerRefactor(m_mscRefactor.get());
    m_dvRefactor->setStorage(this);
    m_ivRefactorHandler->registerRefactor(m_dvRefactor.get());

    connect(this, &scs::SpaceCreatorProject::ivCoreAdded, this, [this](IVEditorCorePtr ivCore) {
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::nameChanged, m_ivRefactorHandler.get(),
                &scs::IVRefactorHandler::onIVEntityNameChanged);
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::entitiesRemoved, m_ivRefactorHandler.get(),
                &scs::IVRefactorHandler::onEntitiesRemoved);
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::implementationChanged, m_ivRefactorHandler.get(),
                &scs::IVRefactorHandler::onImplementationChanged);
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::defaultImplementationChanged,
                m_ivRefactorHandler.get(), &scs::IVRefactorHandler::onDefaultImplementationChanged);
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::implementationListChanged, m_ivRefactorHandler.get(),
                &scs::IVRefactorHandler::onImplementationListChanged);
        connect(ivCore->commandsStack(), &ive::cmd::CommandsStack::attributeChanged, m_ivRefactorHandler.get(),
                &scs::IVRefactorHandler::onAttributeChanged);
    });
}

SpaceCreatorProject::~SpaceCreatorProject() { }

QString SpaceCreatorProject::projectPath() const
{
    return QFileInfo(ivCore()->document()->path()).absolutePath();
}

/*!
   Returns the DVEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
DVEditorCorePtr SpaceCreatorProject::dvData(const QString &fileName) const
{
    if (!m_dvStore.contains(fileName)) {
        auto me = const_cast<SpaceCreatorProject *>(this);
        DVEditorCorePtr data(new dve::DVEditorCore());
        data->setAsn1Check(m_asnChecks.get());
        auto checker = new scs::IvSystemQueries(me, data.data());
        data->setSystemChecker(checker);
        data->loadHWLibrary(shared::hwLibraryPath());
        data->appModel()->load(fileName);

        me->setDvData(fileName, data);
        return data;
    }

    return m_dvStore[fileName];
}

/*!
   Returns the IVEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
IVEditorCorePtr SpaceCreatorProject::ivData(const QString &fileName) const
{
    if (!m_ivStore.contains(fileName)) {
        IVEditorCorePtr data(new ive::IVEditorCore());
        data->registerBasicActions();
        data->document()->customActions(); // There some further actions are registered
        data->document()->setAsn1Check(m_asnChecks.get());
        data->document()->setIvCheck(m_dvChecks.get());

        data->document()->load(fileName);
        for (const QString &asnFileName : allAsn1Files()) {
            data->document()->setAsn1FileName(QFileInfo(asnFileName).canonicalFilePath());
        }
        if (data->document()->asn1FilesNames().isEmpty()) {
            data->document()->setAsn1FileName(GLOBAL_ASN_FILE);
        }
        const_cast<SpaceCreatorProject *>(this)->setIvData(fileName, data);
        return data;
    }

    return m_ivStore[fileName];
}

/*!
   Returns the MSCEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
MSCEditorCorePtr SpaceCreatorProject::mscData(const QString &fileName) const
{
    if (!m_mscStore.contains(fileName)) {
        MSCEditorCorePtr data(new msc::MSCEditorCore());
        data->mainModel()->setAsn1Check(m_asnChecks.get());
        data->mainModel()->loadFile(fileName);
        if (data->mainModel()->mscModel() && data->mainModel()->mscModel()->dataLanguage().isEmpty()) {
            data->mainModel()->mscModel()->setDataLanguage("ASN.1");
        }
        if (data->mainModel()->mscModel() && data->mainModel()->mscModel()->dataDefinitionString().isEmpty()) {
            data->mainModel()->mscModel()->setDataDefinitionString(GLOBAL_ASN_FILE);
            data->mainModel()->mscModel()->checkAllMessages();
        }
        const_cast<SpaceCreatorProject *>(this)->setMscData(fileName, data);
        return data;
    }

    return m_mscStore[fileName];
}

/*!
   Returns the iv data of the project
 */
IVEditorCorePtr SpaceCreatorProject::ivCore() const
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
QVector<DVEditorCorePtr> SpaceCreatorProject::allDVCores() const
{
    const QStringList dvFiles = allDVFiles();
    QVector<DVEditorCorePtr> allDvCores;
    for (const QString &dvFile : dvFiles) {
        DVEditorCorePtr core = dvData(dvFile);
        if (core) {
            allDvCores.append(core);
        }
    }
    return allDvCores;
}

/*!
   Returns all MSCEditorCore objects, that are used in the current project
 */
QVector<MSCEditorCorePtr> SpaceCreatorProject::allMscCores() const
{
    const QStringList mscFiles = allMscFiles();
    QVector<MSCEditorCorePtr> allMscCores;
    for (const QString &mscFile : mscFiles) {
        MSCEditorCorePtr core = mscData(mscFile);
        if (core) {
            allMscCores.append(core);
        }
    }
    return allMscCores;
}

/*!
 * Returns all editor core objects
 */
QVector<EditorCorePtr> SpaceCreatorProject::allCores() const
{
    QVector<EditorCorePtr> cores;
    auto getCores = [&cores](const auto &collection) {
        for (const auto &core : collection) {
            if (core) {
                cores.append(core);
            }
        }
    };

    getCores(QList<IVEditorCorePtr> { ivCore() });
    getCores(allDVCores());
    getCores(allMscCores());
    return cores;
}

/*!
   Returns if the given \p core is in this storage
 */
bool SpaceCreatorProject::contains(QSharedPointer<shared::EditorCore> core) const
{
    for (const DVEditorCorePtr &dvCore : m_dvStore) {
        if (core == dvCore) {
            return true;
        }
    }
    for (const IVEditorCorePtr &ivCore : m_ivStore) {
        if (core == ivCore) {
            return true;
        }
    }
    for (const MSCEditorCorePtr &mscCore : m_mscStore) {
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
    return projectFiles(".asn") + projectFiles(".asn1") + projectFiles(".acn");
}

/*!
   \brief SpaceCreatorProject::projectFiles
   \param suffix
   \return
 */
QStringList SpaceCreatorProject::projectFiles(const QString &suffix) const
{
    QStringList files;
    for (IVEditorCorePtr ivCore : m_ivStore) {
        if (ivCore->filePath().endsWith(suffix)) {
            files.append(ivCore->filePath());
        }
    }
    for (const MSCEditorCorePtr &mscCore : m_mscStore) {
        if (mscCore->filePath().endsWith(suffix)) {
            files.append(mscCore->filePath());
        }
    }
    for (DVEditorCorePtr dvCore : m_dvStore) {
        if (dvCore->filePath().endsWith(suffix)) {
            files.append(dvCore->filePath());
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
    for (const MSCEditorCorePtr &core : m_mscStore) {
        if (auto ivChecker = qobject_cast<scs::IvSystemChecks *>(core->systemChecker())) {
            checks.append(ivChecker);
        }
    }
    return checks;
}

QVector<IvSystemQueries *> SpaceCreatorProject::ivQueries() const
{
    QVector<IvSystemQueries *> checks;
    for (const DVEditorCorePtr &core : m_dvStore) {
        if (auto dvChecker = qobject_cast<scs::IvSystemQueries *>(core->systemChecker())) {
            checks.append(dvChecker);
        }
    }
    return checks;
}

/*!
   Returns an instance of the IV queries
 */
IvSystemQueries *SpaceCreatorProject::ivQuery() const
{
    for (const DVEditorCorePtr &core : m_dvStore) {
        if (auto dvChecker = qobject_cast<scs::IvSystemQueries *>(core->systemChecker())) {
            return dvChecker;
        }
    }
    return nullptr;
}

DvSystemQueries *SpaceCreatorProject::dvChecks() const
{
    return m_dvChecks.get();
}

void SpaceCreatorProject::setRequirementsURL(const QUrl &url)
{
    if (url == requirementsURL()) {
        return;
    }

    m_requirementsURL = url;
    for (const EditorCorePtr &core : allCores()) {
        core->setRequirementsURL(m_requirementsURL);
    }
}

QUrl SpaceCreatorProject::requirementsURL() const
{
    if (m_requirementsURL.isValid()) {
        return m_requirementsURL;
    }

    IVEditorCorePtr iCore = ivCore();
    if (iCore) {
        iCore->requirementsURL();
    }

    return QUrl();
}

void SpaceCreatorProject::setReviewsURL(const QUrl &url)
{
    if (url == reviewsURL()) {
        return;
    }

    m_reviewsURL = url;
    for (const EditorCorePtr &core : allCores()) {
        core->setReviewsURL(m_reviewsURL);
    }
}

QUrl SpaceCreatorProject::reviewsURL() const
{
    if (m_reviewsURL.isValid()) {
        return m_reviewsURL;
    }

    IVEditorCorePtr iCore = ivCore();
    if (iCore) {
        iCore->reviewsURL();
    }

    return QUrl();
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
void SpaceCreatorProject::setDvData(const QString &fileName, DVEditorCorePtr dvData)
{
    const QString oldKey = m_dvStore.key(dvData, "");
    if (!oldKey.isEmpty()) {
        if (m_dvStore[fileName] == dvData) {
            return;
        }
        DVEditorCorePtr oldData = m_dvStore.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_dvStore[fileName] = dvData;
    connect(dvData.data(), &shared::EditorCore::editedExternally, this, &scs::SpaceCreatorProject::editedExternally);
    connect(dvData.data(), &shared::EditorCore::requirementsURLChanged, this,
            &scs::SpaceCreatorProject::setRequirementsURL);
    Q_EMIT dvCoreAdded(dvData);
}

/*!
   Sets the IVEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void SpaceCreatorProject::setIvData(const QString &fileName, IVEditorCorePtr ivData)
{
    const QString oldKey = m_ivStore.key(ivData, "");
    if (!oldKey.isEmpty()) {
        if (m_ivStore[fileName] == ivData) {
            return;
        }
        IVEditorCorePtr oldData = m_ivStore.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_ivStore[fileName] = ivData;
    connect(ivData.data(), &shared::EditorCore::editedExternally, this, &scs::SpaceCreatorProject::editedExternally);
    connect(ivData.data(), &shared::EditorCore::requirementsURLChanged, this,
            &scs::SpaceCreatorProject::setRequirementsURL);
    ivData->setDvChecks(m_dvChecks.get());
    Q_EMIT ivCoreAdded(ivData);
}

/*!
   Sets the MSCEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void SpaceCreatorProject::setMscData(const QString &fileName, MSCEditorCorePtr mscData)
{
    const QString oldKey = m_mscStore.key(mscData, "");
    if (!oldKey.isEmpty()) {
        if (m_mscStore[fileName] == mscData) {
            return;
        }
        MSCEditorCorePtr oldData = m_mscStore.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_mscStore[fileName] = mscData;
    connect(mscData.data(), &shared::EditorCore::editedExternally, this, &scs::SpaceCreatorProject::editedExternally);
    connect(mscData.data(), &shared::EditorCore::requirementsURLChanged, this,
            &scs::SpaceCreatorProject::setRequirementsURL);
    auto checker = new scs::IvSystemChecks(this, mscData.data());
    checker->setMscCore(mscData.data());
    mscData->setSystemChecker(checker);
    Q_EMIT mscCoreAdded(mscData);
}

} // namespace scs

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

#include "spacecreatorprojectimpl.h"

#include "asn1modelstorage.h"
#include "editorcore.h"
#include "errormessageparser.h"
#include "iveditorcore.h"
#include "msceditorcore.h"
#include "mscsystemchecks.h"

#include <editormanager/documentmodel.h>
#include <projectexplorer/project.h>
#include <projectexplorer/taskhub.h>

namespace spctr {

const char TASK_CATEGORY_ASN_COMPILE[] = "Task.Category.ASNCompile";

SpaceCreatorProjectImpl::SpaceCreatorProjectImpl(ProjectExplorer::Project *project, QObject *parent)
    : scs::SpaceCreatorProject(parent)
    , m_project(project)
{
    Q_ASSERT(m_project);

    connect(this, &scs::SpaceCreatorProject::editedExternally, this, &spctr::SpaceCreatorProjectImpl::saveIfNotOpen);

    connect(m_project, &ProjectExplorer::Project::fileListChanged, this,
            &spctr::SpaceCreatorProjectImpl::purgeNonProjectData);

    m_asnFiles = allAsn1Files();
    connect(m_project, &ProjectExplorer::Project::fileListChanged, this,
            &spctr::SpaceCreatorProjectImpl::checkAsnFileRename);

    connect(m_asn1Storage.get(), &Asn1Acn::Asn1ModelStorage::error, this, &SpaceCreatorProjectImpl::reportAsn1Error);

    static bool hubInitialized = false;
    if (!hubInitialized) {
        ProjectExplorer::TaskHub::instance()->addCategory(TASK_CATEGORY_ASN_COMPILE, tr("ASN error"));
        hubInitialized = true;
    }
}

SpaceCreatorProjectImpl::~SpaceCreatorProjectImpl() { }

/*!
   Returns all files of the current project endig with the given \p suffix
 */
QStringList SpaceCreatorProjectImpl::projectFiles(const QString &suffix) const
{
    QStringList result;
    for (const Utils::FileName &fileName : m_project->files(ProjectExplorer::Project::AllFiles)) {
        if (fileName.toString().endsWith(suffix, Qt::CaseInsensitive)) {
            result.append(fileName.toString());
        }
    }

    return result;
}

/*!
   Checks if one asn1 file was renamed. If yes, update the filename in all msc and aadl files.
 */
void SpaceCreatorProjectImpl::checkAsnFileRename()
{
    QStringList asnFiles = allAsn1Files();

    QStringList newAsnFiles;
    for (const QString &file : qAsConst(asnFiles)) {
        if (!m_asnFiles.contains(file)) {
            newAsnFiles.append(file);
        }
    }
    QStringList lostAsnFiles;
    for (const QString &file : qAsConst(m_asnFiles)) {
        if (!asnFiles.contains(file)) {
            lostAsnFiles.append(file);
        }
    }

    if (newAsnFiles.size() == 1 && lostAsnFiles.size() == 1) {
        for (QSharedPointer<msc::MSCEditorCore> &mscCore : allMscCores()) {
            mscCore->renameAsnFile(lostAsnFiles[0], newAsnFiles[0]);
        }
        ivCore()->renameAsnFile(lostAsnFiles[0], newAsnFiles[0]);
    }

    m_asnFiles = asnFiles;
}

/*!
   Saves the data of the given \p core in case it is not open in one of the QtCretaor editors
 */
void SpaceCreatorProjectImpl::saveIfNotOpen(shared::EditorCore *core)
{
    if (!isOpenInEditor(core)) {
        core->save();
    }
}

void SpaceCreatorProjectImpl::reportAsn1Error(const QString &fileName, const QStringList &errors)
{
    if (errors.isEmpty()) {
        return;
    }

    ProjectExplorer::TaskHub::instance()->clearTasks(TASK_CATEGORY_ASN_COMPILE);

    Asn1Acn::ErrorMessageParser parser;
    for (const QString &message : errors) {
        const Asn1Acn::ErrorMessage error = parser.parse(message);
        if (error.isValid()) {
            ProjectExplorer::TaskHub::addTask(ProjectExplorer::Task::Error, error.message(), TASK_CATEGORY_ASN_COMPILE,
                    Utils::FileName::fromString(fileName), error.location().line());
        }
    }

    ProjectExplorer::TaskHub::requestPopup();
}

/*!
   Returns true, if the file of the given /p core is open in an editor
 */
bool SpaceCreatorProjectImpl::isOpenInEditor(shared::EditorCore *core) const
{
    QList<Core::IDocument *> openDocuments = Core::DocumentModel::openedDocuments();
    for (Core::IDocument *doc : qAsConst(openDocuments)) {
        if (doc->filePath().toString() == core->filePath()) {
            return true;
        }
    }
    return false;
}

} // namespace spctr

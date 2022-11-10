/****************************************************************************
**
** Copyright (C) 2017-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a program and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "parseddatastorage.h"

#include "utils/qtcassert.h"

#include "asn1/file.h"
#include "asn1/project.h"

using namespace Asn1Acn::Internal;
#if (QT_VERSION <= QT_VERSION_CHECK(5, 13, 0))
ParsedDataStorage::ParsedDataStorage()
    : m_root(std::make_unique<Asn1Acn::Root>())
{}

ParsedDataStorage *ParsedDataStorage::instance()
{
    static ParsedDataStorage instance_;
    return &instance_;
}

Asn1Acn::File *ParsedDataStorage::getAnyFileForPath(const Utils::FileName &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getFileForPathInternal(filePath);
}

Asn1Acn::File *ParsedDataStorage::getFileForPathFromProject(const QString &projectName,
                                                            const Utils::FileName &filePath)
{
    QMutexLocker locker(&m_documentsMutex);

    const auto &project = m_root->project(projectName);
    if (project == nullptr)
        return nullptr;

    return project->file(filePath.toString());
}

void ParsedDataStorage::addProject(const QString &projectName)
{
    QMutexLocker locker(&m_documentsMutex);

    m_root->add(std::make_unique<Asn1Acn::Project>(projectName));
}

void ParsedDataStorage::removeProject(const QString &projectName)
{
    QMutexLocker locker(&m_documentsMutex);

    m_root->remove(projectName);
}

const QStringList ParsedDataStorage::getProjectsForFile(const Utils::FileName &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getProjectsForFileInternal(filePath);
}

const Utils::FileNameList ParsedDataStorage::getFilesPathsFromProject(const QString &project) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getFilesPathsFromProjectInternal(project);
}

Asn1Acn::SourceLocation ParsedDataStorage::getDefinitionLocation(const Utils::FileName &path,
                                                                 const QString &typeAssignmentName,
                                                                 const QString &definitionsName) const
{
    const auto type = getTypeAssignment(path, typeAssignmentName, definitionsName);
    return (type == nullptr) ? Asn1Acn::SourceLocation() : type->location();
}

const Asn1Acn::TypeAssignment *ParsedDataStorage::getTypeAssignment(
    const Utils::FileName &path,
    const QString &typeAssignmentName,
    const QString &definitionsName) const
{
    QMutexLocker locker(&m_documentsMutex);

    const auto file = getFileForPathInternal(path);
    if (file == nullptr)
        return nullptr;

    const auto definitions = file->definitions(definitionsName);
    if (definitions == nullptr)
        return nullptr;

    return definitions->type(typeAssignmentName);
}

int ParsedDataStorage::getProjectBuildersCount(const QString &projectName) const
{
    QMutexLocker locker(&m_documentsMutex);

    const auto project = m_root->project(projectName);
    if (project == nullptr)
        return -1;

    return project->buildersCount();
}

void ParsedDataStorage::setProjectBuildersCount(const QString &projectName, const int version) const
{
    QMutexLocker locker(&m_documentsMutex);

    auto project = m_root->project(projectName);
    if (project == nullptr)
        return;

    return project->setBuildersCount(version);
}

void ParsedDataStorage::resetProjectBuildersCount()
{
    QMutexLocker locker(&m_documentsMutex);

    const auto &projects = m_root->projects();
    for (const auto &project : projects)
        project->setBuildersCount(0);
}

void ParsedDataStorage::addFileToProject(const QString &projectName,
                                         std::unique_ptr<Asn1Acn::File> file)
{
    QMutexLocker locker(&m_documentsMutex);

    auto project = m_root->project(projectName);
    if (project == nullptr)
        return;

    project->add(std::move(file));
}

void ParsedDataStorage::removeFileFromProject(const QString &projectName,
                                              const Utils::FileName &filePath)
{
    QMutexLocker locker(&m_documentsMutex);

    removeFileFromProjectInternal(projectName, filePath);
}

int ParsedDataStorage::getProjectsCount()
{
    QMutexLocker locker(&m_documentsMutex);

    return static_cast<int>(m_root->projects().size());
}

int ParsedDataStorage::getDocumentsCount()
{
    int cnt = 0;

    QMutexLocker locker(&m_documentsMutex);

    for (auto it = m_root->projects().begin(); it != m_root->projects().end(); it++)
        cnt += static_cast<int>((*it)->files().size());

    return cnt;
}

void ParsedDataStorage::removeFileFromProjectInternal(const QString &projectName,
                                                      const Utils::FileName &filePath)
{
    auto prj = m_root->project(projectName);
    if (prj == nullptr)
        return;

    prj->remove(filePath.toString());
}

const QStringList ParsedDataStorage::getProjectsForFileInternal(const Utils::FileName &filePath) const
{
    QStringList res;

    const auto &projects = m_root->projects();
    for (const auto &project : projects) {
        if (project->file(filePath.toString()) != nullptr)
            res.append(project->name());
    }

    return res;
}

const Utils::FileNameList ParsedDataStorage::getFilesPathsFromProjectInternal(
    const QString &projectName) const
{
    const auto project = m_root->project(projectName);
    if (project == nullptr)
        return {};

    Utils::FileNameList ret;
    for (const auto &file : project->files())
        ret.append(Utils::FileName::fromString(file->name()));

    return ret;
}

Asn1Acn::File *ParsedDataStorage::getFileForPathInternal(const Utils::FileName &filePath) const
{
    for (auto it = m_root->projects().begin(); it != m_root->projects().end(); it++)
        if ((*it)->file(filePath.toString()) != nullptr)
            return (*it)->file(filePath.toString());

    return nullptr;
}

Asn1Acn::SourceLocation ParsedDataStorage::getLocationFromModule(
    const Asn1Acn::Definitions &moduleDefinition, const QString &typeAssignmentName) const
{
    const auto type = moduleDefinition.type(typeAssignmentName);
    if (!type)
        return {};
    return type->location();
}
#else
ParsedDataStorage::ParsedDataStorage()
    : m_root(std::make_unique<Asn1Acn::Root>())
{}

ParsedDataStorage *ParsedDataStorage::instance()
{
    static ParsedDataStorage instance_;
    return &instance_;
}

Asn1Acn::File *ParsedDataStorage::getAnyFileForPath(const Utils::FilePath &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getFileForPathInternal(filePath);
}

Asn1Acn::File *ParsedDataStorage::getFileForPathFromProject(const QString &projectName,
                                                            const Utils::FilePath &filePath)
{
    QMutexLocker locker(&m_documentsMutex);

    const auto &project = m_root->project(projectName);
    if (project == nullptr)
        return nullptr;

    return project->file(filePath.toString());
}

void ParsedDataStorage::addProject(const QString &projectName)
{
    QMutexLocker locker(&m_documentsMutex);

    m_root->add(std::make_unique<Asn1Acn::Project>(projectName));
}

void ParsedDataStorage::removeProject(const QString &projectName)
{
    QMutexLocker locker(&m_documentsMutex);

    m_root->remove(projectName);
}

const QStringList ParsedDataStorage::getProjectsForFile(const Utils::FilePath &filePath) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getProjectsForFileInternal(filePath);
}

const Utils::FilePaths ParsedDataStorage::getFilesPathsFromProject(const QString &project) const
{
    QMutexLocker locker(&m_documentsMutex);

    return getFilesPathsFromProjectInternal(project);
}

Asn1Acn::SourceLocation ParsedDataStorage::getDefinitionLocation(const Utils::FilePath &path,
                                                                 const QString &typeAssignmentName,
                                                                 const QString &definitionsName) const
{
    const auto type = getTypeAssignment(path, typeAssignmentName, definitionsName);
    return (type == nullptr) ? Asn1Acn::SourceLocation() : type->location();
}

const Asn1Acn::TypeAssignment *ParsedDataStorage::getTypeAssignment(
    const Utils::FilePath &path,
    const QString &typeAssignmentName,
    const QString &definitionsName) const
{
    QMutexLocker locker(&m_documentsMutex);

    const auto file = getFileForPathInternal(path);
    if (file == nullptr)
        return nullptr;

    const auto definitions = file->definitions(definitionsName);
    if (definitions == nullptr)
        return nullptr;

    return definitions->type(typeAssignmentName);
}

int ParsedDataStorage::getProjectBuildersCount(const QString &projectName) const
{
    QMutexLocker locker(&m_documentsMutex);

    const auto project = m_root->project(projectName);
    if (project == nullptr)
        return -1;

    return project->buildersCount();
}

void ParsedDataStorage::setProjectBuildersCount(const QString &projectName, const int version) const
{
    QMutexLocker locker(&m_documentsMutex);

    auto project = m_root->project(projectName);
    if (project == nullptr)
        return;

    return project->setBuildersCount(version);
}

void ParsedDataStorage::resetProjectBuildersCount()
{
    QMutexLocker locker(&m_documentsMutex);

    const auto &projects = m_root->projects();
    for (const auto &project : projects)
        project->setBuildersCount(0);
}

void ParsedDataStorage::addFileToProject(const QString &projectName,
                                         std::unique_ptr<Asn1Acn::File> file)
{
    QMutexLocker locker(&m_documentsMutex);

    auto project = m_root->project(projectName);
    if (project == nullptr)
        return;

    project->add(std::move(file));
}

void ParsedDataStorage::removeFileFromProject(const QString &projectName,
                                              const Utils::FilePath &filePath)
{
    QMutexLocker locker(&m_documentsMutex);

    removeFileFromProjectInternal(projectName, filePath);
}

int ParsedDataStorage::getProjectsCount()
{
    QMutexLocker locker(&m_documentsMutex);

    return static_cast<int>(m_root->projects().size());
}

int ParsedDataStorage::getDocumentsCount()
{
    int cnt = 0;

    QMutexLocker locker(&m_documentsMutex);

    auto projects = m_root->projects();
    for (auto it = projects.begin(); it != projects.end(); it++) {
        cnt += static_cast<int>((*it)->files().size());
    }

    return cnt;
}

void ParsedDataStorage::removeFileFromProjectInternal(const QString &projectName,
                                                      const Utils::FilePath &filePath)
{
    auto prj = m_root->project(projectName);
    if (prj == nullptr)
        return;

    prj->remove(filePath.toString());
}

const QStringList ParsedDataStorage::getProjectsForFileInternal(const Utils::FilePath &filePath) const
{
    QStringList res;

    const auto &projects = m_root->projects();
    for (const auto &project : projects) {
        if (project->file(filePath.toString()) != nullptr)
            res.append(project->name());
    }

    return res;
}

const Utils::FilePaths ParsedDataStorage::getFilesPathsFromProjectInternal(
    const QString &projectName) const
{
    const auto project = m_root->project(projectName);
    if (project == nullptr)
        return {};

    Utils::FilePaths ret;
    for (const auto &file : project->files())
        ret.append(Utils::FilePath::fromString(file->name()));

    return ret;
}

Asn1Acn::File *ParsedDataStorage::getFileForPathInternal(const Utils::FilePath &filePath) const
{
    auto projects = m_root->projects();
    for (auto it = projects.begin(); it != projects.end(); it++)
        if ((*it)->file(filePath.toString()) != nullptr)
            return (*it)->file(filePath.toString());

    return nullptr;
}

Asn1Acn::SourceLocation ParsedDataStorage::getLocationFromModule(
    const Asn1Acn::Definitions &moduleDefinition, const QString &typeAssignmentName) const
{
    const auto type = moduleDefinition.type(typeAssignmentName);
    if (!type)
        return {};
    return type->location();
}
#endif

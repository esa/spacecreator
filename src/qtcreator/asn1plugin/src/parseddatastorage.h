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
#pragma once

#include <QHash>
#include <QMutex>
#include <QObject>
#include <QString>

#include <memory>

#include <utils/fileutils.h>

#if QTC_VERSION == 408

#include <utils/filepath.h>

#include "asn1/file.h"
#include "asn1/root.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDataStorage : public QObject
{
    Q_OBJECT

public:
    ParsedDataStorage();
    ~ParsedDataStorage() = default;

    static ParsedDataStorage *instance();

    const Asn1Acn::Root *root() const { return m_root.get(); }

    void addProject(const QString &projectName);
    void removeProject(const QString &projectName);

    void addFileToProject(const QString &projectName, std::unique_ptr<Asn1Acn::File> file);
    void removeFileFromProject(const QString &projectName, const Utils::FileName &filePath);

    Asn1Acn::File *getAnyFileForPath(const Utils::FileName &filePath) const;
    Asn1Acn::File *getFileForPathFromProject(const QString &project, const Utils::FileName &path);

    const QStringList getProjectsForFile(const Utils::FileName &filePath) const;
    const Utils::FileNameList getFilesPathsFromProject(const QString &project) const;

    Asn1Acn::SourceLocation getDefinitionLocation(const Utils::FileName &path,
                                                  const QString &typeAssignmentName,
                                                  const QString &definitionsName) const;
    const Asn1Acn::TypeAssignment *getTypeAssignment(const Utils::FileName &path,
                                                     const QString &typeAssignmentName,
                                                     const QString &definitionsName) const;

    int getProjectBuildersCount(const QString &projectName) const;
    void setProjectBuildersCount(const QString &projectName, const int version) const;
    void resetProjectBuildersCount();

    int getProjectsCount();
    int getDocumentsCount();

private:
    const QStringList getProjectsForFileInternal(const Utils::FileName &filePath) const;
    const Utils::FileNameList getFilesPathsFromProjectInternal(const QString &projectName) const;
    void removeFileFromProjectInternal(const QString &projectName, const Utils::FileName &filePath);

    Asn1Acn::File *getFileForPathInternal(const Utils::FileName &filePath) const;

    Asn1Acn::SourceLocation getLocationFromModule(const Asn1Acn::Definitions &moduleDefinition,
                                                  const QString &typeAssignmentName) const;

    std::unique_ptr<Asn1Acn::Root> m_root;
    mutable QMutex m_documentsMutex;
};

#endif

#if QTC_VERSION == 800

#include <utils/filepath.h>

#include "asn1/file.h"
#include "asn1/root.h"

namespace Asn1Acn {
namespace Internal {

class ParsedDataStorage : public QObject
{
    Q_OBJECT

public:
    ParsedDataStorage();
    ~ParsedDataStorage() = default;

    static ParsedDataStorage *instance();

    const Asn1Acn::Root *root() const { return m_root.get(); }

    void addProject(const QString &projectName);
    void removeProject(const QString &projectName);

    void addFileToProject(const QString &projectName, std::unique_ptr<Asn1Acn::File> file);
    void removeFileFromProject(const QString &projectName, const Utils::FilePath &filePath);

    Asn1Acn::File *getAnyFileForPath(const Utils::FilePath &filePath) const;
    Asn1Acn::File *getFileForPathFromProject(const QString &project, const Utils::FilePath &path);

    const QStringList getProjectsForFile(const Utils::FilePath &filePath) const;
    const Utils::FilePaths getFilesPathsFromProject(const QString &project) const;

    Asn1Acn::SourceLocation getDefinitionLocation(const Utils::FilePath &path,
                                                  const QString &typeAssignmentName,
                                                  const QString &definitionsName) const;
    const Asn1Acn::TypeAssignment *getTypeAssignment(const Utils::FilePath &path,
                                                     const QString &typeAssignmentName,
                                                     const QString &definitionsName) const;

    int getProjectBuildersCount(const QString &projectName) const;
    void setProjectBuildersCount(const QString &projectName, const int version) const;
    void resetProjectBuildersCount();

    int getProjectsCount();
    int getDocumentsCount();

private:
    const QStringList getProjectsForFileInternal(const Utils::FilePath &filePath) const;
    const Utils::FilePaths getFilesPathsFromProjectInternal(const QString &projectName) const;
    void removeFileFromProjectInternal(const QString &projectName, const Utils::FilePath &filePath);

    Asn1Acn::File *getFileForPathInternal(const Utils::FilePath &filePath) const;

    Asn1Acn::SourceLocation getLocationFromModule(const Asn1Acn::Definitions &moduleDefinition,
                                                  const QString &typeAssignmentName) const;

    std::unique_ptr<Asn1Acn::Root> m_root;
    mutable QMutex m_documentsMutex;
};

#endif

} // namespace Internal
} // namespace Asn1Acn

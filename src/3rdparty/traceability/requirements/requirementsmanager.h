/*
   Copyright (C) 2023 European Space Agency - <maxime.perrotin@esa.int>

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

#include "issuesmanager.h"
#include "requirement.h"

#include <memory>

namespace requirement {

/*!
 * The RequirementsManager is responsible to communicate to the server/storage that handles the requirments for
 * components
 */
class RequirementsManager : public tracecommon::IssuesManager
{
    Q_OBJECT

public:
    RequirementsManager(REPO_TYPE repoType = tracecommon::IssuesManager::REPO_TYPE::GITLAB, QObject *parent = nullptr);
    ~RequirementsManager();

    bool requestAllRequirements();
    bool createRequirement(
            const QString &title, const QString &reqIfId, const QString &description, const QString &testMethod) const;
    bool removeRequirement(const Requirement &requirement) const;

Q_SIGNALS:
    void startingFetchingRequirements();
    void fetchingRequirementsEnded();
    void listOfRequirements(const QList<requirement::Requirement> &);
    void requirementCreated();
    void requirementClosed();

private:
    class RequirementsManagerPrivate;
    std::unique_ptr<RequirementsManagerPrivate> d;
};

}

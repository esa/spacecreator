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

#include "requirement.h"

#include <QObject>
#include <QUrl>
#include <memory>

namespace requirement {

/*!
 * The RequirementsManager is responsible to communicate to the server/storage that handles the requirments for
 * components
 */
class RequirementsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int projectID READ projectID WRITE setProjectID NOTIFY projectIDChanged)
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(QStringList tagsBuffer READ tagsBuffer)

public:
    enum class REPO_TYPE
    {
        GITLAB
    };

    RequirementsManager(REPO_TYPE repoType, QObject *parent = nullptr);
    ~RequirementsManager();

    bool setCredentials(const QString &url, const QString &token);
    QString projectUrl() const;
    const QString &token() const;

    bool isBusy() const;
    const int &projectID() const;

    bool requestAllRequirements();
    bool createRequirement(
            const QString &title, const QString &reqIfId, const QString &description, const QString &testMethod) const;
    bool removeRequirement(const Requirement &requirement) const;
    bool requestTags();

    QStringList tagsBuffer();

public Q_SLOTS:
    void setProjectID(const int &newProjectID);

Q_SIGNALS:
    void busyChanged();
    void projectIDChanged();
    void startingFetchingRequirements();
    void fetchingRequirementsEnded();
    void listOfRequirements(const QList<requirement::Requirement> &);
    void listOfTags(const QStringList &);
    void connectionError(QString errorString);
    void requirementCreated();
    void requirementClosed();

private:
    bool requestProjectID(const QUrl &url);

    int m_projectID = -1;
    QUrl m_projectUrl = {};
    QString m_token = "";
    QStringList m_tagsBuffer;
    class RequirementsManagerPrivate;
    std::unique_ptr<RequirementsManagerPrivate> d;
};

}

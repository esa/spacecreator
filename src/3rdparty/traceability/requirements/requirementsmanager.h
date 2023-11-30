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
#include <memory>

namespace requirement {

class RequirementsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString projectID READ projectID WRITE setProjectID NOTIFY connectionReady)
public:

    enum class REPO_TYPE
    {
        GITLAB
    };

    RequirementsManager(REPO_TYPE repoType, QObject *parent = nullptr);
    ~RequirementsManager();
    void setCredentials(const QString &url, const QString &token);
    void requestRequirements(const QString &assignee, const QString &author);
    void createRequirement(const QString &title, const QString &reqIfId, const QString &description) const;
    void requestProjectID(const QUrl &url);
    QString projectID() const;

public Q_SLOTS:
    void setProjectID(const QString &newProjectID);

Q_SIGNALS:
    void startfetchingRequirements();
    void listOfRequirements(const QList<requirement::Requirement> &);
    void connectionReady();
    void connectionError(QString errorString);
    void requirementCreated();

private:
    QString m_projectID = "";
    QUrl m_projectUrl = {};
    QString m_token = "";
    class RequirementsManagerPrivate;
    std::unique_ptr<RequirementsManagerPrivate> d;
};

}

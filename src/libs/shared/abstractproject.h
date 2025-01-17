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
#include <QUrl>

namespace shared {

/*!
 * \brief The AbstractProject class is the base class for projects in all SpaceCreator applications/editors
 */
class AbstractProject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl requirementsURL READ requirementsURL WRITE setRequirementsURL NOTIFY requirementsURLChanged)

public:
    explicit AbstractProject(QObject *parent = nullptr);

    virtual QString projectPath() const = 0;

    virtual QStringList allDVFiles() const = 0;
    virtual QStringList allIVFiles() const = 0;
    virtual QStringList allMscFiles() const = 0;
    virtual QStringList allAsn1Files() const = 0;

    virtual void setRequirementsURL(const QUrl &url) = 0;
    virtual QUrl requirementsURL() const = 0;
    virtual void setReviewsURL(const QUrl &url) = 0;
    virtual QUrl reviewsURL() const = 0;

Q_SIGNALS:
    void requirementsURLChanged(const QUrl &url);
};

} // namespace shared

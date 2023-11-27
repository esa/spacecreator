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

#include <QObject>
#include <QUrl>

namespace shared {

/*!
 * Basic model for the data of for example a .msc file or a interfaceview file.
 */
class DataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl requirementsURL READ requirementsURL WRITE setRequirementsURL NOTIFY requirementsURLChanged)
    Q_PROPERTY(QString creatorGitHash READ creatorGitHash WRITE setCreatorGitHash NOTIFY creatorGitHashChanged)

public:
    explicit DataModel(QObject *parent = nullptr);

    virtual void clear() = 0;

    void setRequirementsURL(const QUrl &url);
    const QUrl &requirementsURL() const;

    void setCreatorGitHash(const QString &hashStr);
    QString creatorGitHash() const;

Q_SIGNALS:
    void requirementsURLChanged(const QUrl &);
    void creatorGitHashChanged(const QString &);

protected:
    QUrl m_requirementsURL;
    QString m_creatorGitHash;
};

} // namespace shared


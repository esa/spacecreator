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

#include "datamodel.h"

namespace shared {

DataModel::DataModel(QObject *parent)
    : QObject{parent}
{
}

/*!
 * Sets the URL where requirements are stored
 */
void DataModel::setRequirementsURL(const QUrl &url)
{
    if (!url.isValid() || url == m_requirementsURL) {
        return;
    }

    m_requirementsURL = url;
    Q_EMIT requirementsURLChanged(m_requirementsURL);
}

/*!
 * Returns the URL where requirements are stored
 */
const QUrl &DataModel::requirementsURL() const
{
    return m_requirementsURL;
}

/*!
 * Returns the git has for this SpaceCreator build
 */
void DataModel::setCreatorGitHash(const QString &hashStr)
{
    if (hashStr == m_creatorGitHash) {
        return;
    }

    m_creatorGitHash = hashStr;
    Q_EMIT creatorGitHashChanged(m_creatorGitHash);
}

/*!
 * Returns the git has for this SpaceCreator build
 */
QString DataModel::creatorGitHash() const
{
    return m_creatorGitHash;
}

} // namespace shared

/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "scversion.h"

#include <QCoreApplication>
#include <QDebug>
#include <QRegularExpression>
#include <QString>
#include <iostream>
#include <string.h>

struct SpinError {
    int errorCode;
    int errorDepth;
    QString errorDetails;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersion);
    app.setApplicationName(QObject::tr("Spin Error Parser"));

    const QStringList arguments = app.arguments();
    // check if spin message exists
    if (arguments.size() < 2) {
        qCritical("No spin message passed");
        exit(EXIT_FAILURE);
    }
    // get passed spin message
    const auto spinMessage = arguments.at(1);
    // build pattern for error matching
    const QRegularExpression regex("pan:(\\d+):\\s+(.+?)\\s+\\(at depth (\\d+)\\)\\n");
    // convert all matches to spin errors
    QList <SpinError> spinErrors;
    auto globalMatch = regex.globalMatch(spinMessage);
    while (globalMatch.hasNext()) {
        const auto match = globalMatch.next();
        // extract match tokens
        int errorCode = match.captured(1).toInt();
        int errorDepth = match.captured(3).toInt();
        const QString errorDetails = match.captured(2);
        // build error
        SpinError spinError { errorCode, errorDepth, errorDetails };
        spinErrors.append(spinError);
    }
    return EXIT_SUCCESS;
}

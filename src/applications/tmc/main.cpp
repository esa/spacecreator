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
#include <QString>
#include <ivcore/ivlibrary.h>
#include <shared/sharedlibrary.h>
#include <tmc/TmcVerifier/verifier.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    shared::initSharedLibrary();
    ivm::initIVLibrary();

    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersion);
    app.setApplicationName(QObject::tr("SedsConverter"));

    std::optional<QString> inputIvFilepath;
    std::optional<QString> outputDirectory;

    const QStringList args = app.arguments();

    for (int i = 1; i < args.size(); ++i) {
        const QString &arg = args[i];
        if (arg == "-iv") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -iv");
                exit(EXIT_FAILURE);
            }
            if (inputIvFilepath.has_value()) {
                qCritical("Duplicated -iv argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            inputIvFilepath = args[i];
        } else if (arg == "-o") {
            if (i + 1 == args.size()) {
                qCritical("Missing filename after -o");
                exit(EXIT_FAILURE);
            }
            if (outputDirectory.has_value()) {
                qCritical("Duplicated -o argument");
                exit(EXIT_FAILURE);
            }
            ++i;
            outputDirectory = args[i];
        } else if (arg == "-h" || arg == "--help") {
            qInfo("tmc: TASTE Model Chcecker");
            qInfo("Usage: tmc [OPTIONS]");
            qInfo("  -iv <filepath>       Use <filepath> as input InterfaceView");
            qInfo("  -o <dir>             Use <dir> as output directory.");
            qInfo("  -h, --help           Print this message and exit.");
            exit(EXIT_SUCCESS);
        } else {
            qCritical("Unrecognized command line option");
            exit(EXIT_FAILURE);
        }
    }

    if (!inputIvFilepath.has_value()) {
        qCritical("Missing mandatory argument: input InterfaceView");
        exit(EXIT_FAILURE);
    }
    if (!outputDirectory.has_value()) {
        qCritical("Missing mandatory argument: output directory");
        exit(EXIT_FAILURE);
    }

    tmc::verifier::TmcVerifier verifier(inputIvFilepath.value(), outputDirectory.value());

    if (verifier.execute()) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

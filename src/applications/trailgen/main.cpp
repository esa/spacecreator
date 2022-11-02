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

#include "executor.h"
#include "scversion.h"

#include <QCoreApplication>
#include <QTimer>
#include <QtGlobal>
#include <ivcore/ivlibrary.h>
#include <msccore/msclibrary.h>
#include <shared/sharedlibrary.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(asn1_resources);

    ivm::initIVLibrary();
    shared::initSharedLibrary();
    msc::initMscLibrary();

    QCoreApplication app(argc, argv);

    app.setOrganizationName(SC_ORGANISATION);
    app.setOrganizationDomain(SC_ORGANISATION_DOMAIN);
    app.setApplicationVersion(spaceCreatorVersion);
    app.setApplicationName(QObject::tr("TASTE Model Checker trace generator"));

    tmc::TrailgenExecutor *executor = new tmc::TrailgenExecutor();

    QTimer::singleShot(0, executor, SLOT(execute()));

    return app.exec();
}

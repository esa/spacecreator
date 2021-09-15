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

#include "dvlibrary.h"

#include "dvboard.h"
#include "dvbus.h"
#include "dvconnection.h"
#include "dvdevice.h"
#include "dvfunction.h"
#include "dvmessage.h"
#include "dvnode.h"
#include "dvpartition.h"
#include "dvport.h"
#include "dvsystemfunction.h"
#include "dvsysteminterface.h"

#include <qglobal.h>

static void init_dv_library()
{
    Q_INIT_RESOURCE(dvresources);
}

namespace dvm {

/**
   Initializes the library resources and Qt meta types.
 */
void initDVLibrary()
{
    init_dv_library();

    qRegisterMetaType<QList<dvm::DVBoard *>>("QList<dvm::DVBoard*>");
    qRegisterMetaType<QList<dvm::DVBus *>>("QList<dvm::DVBus*>");
    qRegisterMetaType<QList<dvm::DVConnection *>>("QList<dvm::DVConnection*>");
    qRegisterMetaType<QList<dvm::DVDevice *>>("QList<dvm::DVDevice*>");
    qRegisterMetaType<QList<dvm::DVFunction *>>("QList<dvm::DVFunction*>");
    qRegisterMetaType<QList<dvm::DVMessage *>>("QList<dvm::DVMessage*>");
    qRegisterMetaType<QList<dvm::DVNode *>>("QList<dvm::DVNode*>");
    qRegisterMetaType<QList<dvm::DVPartition *>>("QList<dvm::DVPartition*>");
    qRegisterMetaType<QList<dvm::DVPort *>>("QList<dvm::DVPort*>");
    qRegisterMetaType<QList<dvm::DVSystemFunction *>>("QList<dvm::DVSystemFunction*>");
    qRegisterMetaType<QList<dvm::DVSystemInterface *>>("QList<dvm::DVSystemInterface*>");
}

} // namespace dvm

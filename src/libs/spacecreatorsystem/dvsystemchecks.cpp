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

#include "dvsystemchecks.h"

#include "dveditorcore.h"
#include "errorhub.h"
#include "interfacedocument.h"
#include "iveditorcore.h"
#include "ivmodel.h"

#include <QtDebug>

namespace scs {

DvSystemChecks::DvSystemChecks(QObject *parent)
    : dve::AbstractSystemChecks(parent)
{
}

void DvSystemChecks::setIVCore(QSharedPointer<ive::IVEditorCore> ivCore)
{
    if (ivCore == m_ivCore) {
        return;
    }

    if (ivm::IVModel *model = ivModel()) {
        disconnect(model, nullptr, this, nullptr);
    }

    m_ivCore = ivCore;
    Q_EMIT ivDataReset();
}

QSharedPointer<ive::IVEditorCore> DvSystemChecks::ivCore() const
{
    return m_ivCore;
}

void DvSystemChecks::setDVCore(dve::DVEditorCore *dvCore)
{
    m_dvCore = dvCore;
}

dve::DVEditorCore *DvSystemChecks::dvCore() const
{
    return m_dvCore;
}

QStringList DvSystemChecks::functionsNames() const
{
    if (!m_ivCore) {
        return {};
    }

    return m_ivCore->ivFunctionsNames();
}

/*!
   Returns a pointer to the IV model of the in-core
 */
ivm::IVModel *DvSystemChecks::ivModel() const
{
    if (!m_ivCore) {
        return {};
    }

    if (!m_ivCore->document() || !m_ivCore->document()->objectsModel()) {
        shared::ErrorHub::addError(shared::ErrorItem::Warning, tr("No IV model"));
        return {};
    }

    return m_ivCore->document()->objectsModel();
}

} // namespace scs

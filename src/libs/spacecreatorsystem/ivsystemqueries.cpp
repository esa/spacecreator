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

#include "ivsystemqueries.h"

#include "dveditorcore.h"
#include "errorhub.h"
#include "interfacedocument.h"
#include "ivconnectionchain.h"
#include "iveditorcore.h"
#include "ivmodel.h"
#include "spacecreatorproject.h"

#include <QtDebug>

namespace scs {

IvSystemQueries::IvSystemQueries(SpaceCreatorProject *project, QObject *parent)
    : dve::AbstractSystemChecks(parent)
    , m_project(project)
{
}

void IvSystemQueries::setIVCore(QSharedPointer<ive::IVEditorCore> ivCore)
{
    if (ivCore == m_ivCore) {
        return;
    }

    m_ivCore = ivCore;
    if (ivm::IVModel *model = ivModel()) {
        disconnect(model, nullptr, this, nullptr);
    }

    Q_EMIT ivDataReset();
}

QSharedPointer<ive::IVEditorCore> IvSystemQueries::ivCore() const
{
    if (!m_ivCore && m_project) {
        IvSystemQueries *me = const_cast<IvSystemQueries *>(this);
        me->setIVCore(m_project->ivCore());
    }
    return m_ivCore;
}

QStringList IvSystemQueries::functionsNames() const
{
    if (!ivCore()) {
        return {};
    }

    return ivCore()->ivFunctionsNames();
}

/*!
   Returns all messages/connections from function \p sourceFunction to \p targetFunction
   Result is a list of pairs. The first of the pais is the name of the source, the second, the name of the target
   interface
 */
QList<QPair<QString, QString>> IvSystemQueries::messages(
        const QString &sourceFunction, const QString &targetFunction) const
{
    if (!ivCore()) {
        return {};
    }

    QList<QPair<QString, QString>> connectionNames;
    QList<ivm::IVConnectionChain *> chains = ivm::IVConnectionChain::build(*ivModel());
    for (const ivm::IVConnectionChain *chain : qAsConst(chains)) {
        QList<QPair<QString, QString>> names = chain->connectionIfNames(sourceFunction, targetFunction);
        for (const QPair<QString, QString> &name : names) {
            if (!connectionNames.contains(name)) {
                connectionNames.append(name);
            }
        }
    }
    return connectionNames;
}

/*!
   Returns a pointer to the IV model of the in-core
 */
ivm::IVModel *IvSystemQueries::ivModel() const
{
    if (!ivCore()) {
        return {};
    }

    if (!ivCore()->document() || !ivCore()->document()->objectsModel()) {
        shared::ErrorHub::addError(shared::ErrorItem::Warning, tr("No IV model"));
        return {};
    }

    return ivCore()->document()->objectsModel();
}

} // namespace scs

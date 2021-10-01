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
#include "ivconnection.h"
#include "ivconnectionchain.h"
#include "ivfunction.h"
#include "ivmodel.h"
#include "spacecreatorproject.h"

#include <QtDebug>
#include <ivfunction.h>

namespace scs {

IvSystemQueries::IvSystemQueries(SpaceCreatorProject *project, QObject *parent)
    : dvm::AbstractSystemChecks(parent)
    , m_project(project)
{
}

void IvSystemQueries::setIVCore(IVEditorCorePtr ivCore)
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

IVEditorCorePtr IvSystemQueries::ivCore() const
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
   Returns the names of all functions names using "pseudo function" as implementation
 */
QStringList IvSystemQueries::pseudoFunctionsNames() const
{
    if (!ivCore()) {
        return {};
    }

    QStringList functionNames;
    for (const ivm::IVFunction *ivFunction : ivCore()->allIVFunctions()) {
        if (ivFunction && !ivFunction->title().isEmpty() && ivFunction->isPseudoFunction()) {
            functionNames << ivFunction->title();
        }
    }
    return functionNames;
}

/*!
   Returns all pseudo/system functions that are connected to the \p function
 */
QList<ivm::IVFunction *> IvSystemQueries::connectedPseudoFunctions(const QString &functionName) const
{
    if (!ivModel()) {
        return {};
    }

    ivm::IVFunction *ivFunction = functionByName(functionName);
    if (!ivFunction) {
        return {};
    }

    ivm::IVModel *model = ivModel();
    QVector<ivm::IVConnection *> connections = model->getConnectionsForFunction(ivFunction->id());

    QList<ivm::IVFunction *> functions;
    auto addFunction = [&](ivm::IVObject *obj) {
        ivm::IVFunction *f = qobject_cast<ivm::IVFunction *>(obj);
        if (f && f != ivFunction && f->isPseudoFunction() && !functions.contains(f)) {
            functions.append(f);
        }
    };

    for (ivm::IVConnection *connection : connections) {
        addFunction(connection->source());
        addFunction(connection->target());
    }

    return functions;
}

/*!
   Returns the function with the name \p functionName
 */
ivm::IVFunction *IvSystemQueries::functionByName(const QString &functionName) const
{
    if (!ivModel()) {
        return nullptr;
    }

    ivm::IVModel *model = ivModel();
    return model->getFunction(functionName, m_caseCheck);
}

/*!
   Return all functions that are connected with \p functionName via a proteced connection/interface
 */
QList<ivm::IVFunction *> IvSystemQueries::connectedProtectedFunctions(const QString &functionName) const
{
    if (!ivModel()) {
        return {};
    }

    ivm::IVFunction *ivFunction = functionByName(functionName);
    if (!ivFunction) {
        return {};
    }

    ivm::IVModel *model = ivModel();
    QVector<ivm::IVConnection *> connections = model->getConnectionsForFunction(ivFunction->id());

    QList<ivm::IVFunction *> functions;
    auto addFunction = [&](ivm::IVObject *obj) {
        ivm::IVFunction *f = qobject_cast<ivm::IVFunction *>(obj);
        if (f && f != ivFunction && !functions.contains(f)) {
            functions.append(f);
        }
    };

    for (ivm::IVConnection *connection : connections) {
        if (connection->isProtected()) {
            addFunction(connection->source());
            addFunction(connection->target());
        }
    }

    return functions;
}

/*!
   Returns the names of all functions that are connected with \p functionName via a proteced connection/interface
 */
QStringList IvSystemQueries::connectedProtectedFunctionNames(const QString &functionName) const
{
    QList<ivm::IVFunction *> functions = connectedProtectedFunctions(functionName);
    QStringList names;
    for (ivm::IVFunction *function : functions) {
        names.append(function->title());
    }
    return names;
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
   Returns if a IV connection with the given parameters esists
 */
bool IvSystemQueries::connectionExists(const QString &sourceFunction, const QString &sourceInterface,
        const QString &targetFunction, const QString &targetInterface) const
{
    QList<QPair<QString, QString>> connections = messages(sourceFunction, targetFunction);
    QPair<QString, QString> msgIf { sourceInterface, targetInterface };
    return connections.contains(msgIf);
}

QString IvSystemQueries::defaultImplementationForFunction(const QString &function) const
{
    qDebug() << Q_FUNC_INFO;
    ivm::IVFunction *fn = functionByName(function);
    if (!fn) {
        return {};
    }

    return fn->defaultLanguage();
}

QStringList IvSystemQueries::implementationsForFunction(const QString &function) const
{
    ivm::IVFunction *fn = functionByName(function);
    if (!fn) {
        return {};
    }

    QStringList implementations;
    for (const EntityAttribute &lang : fn->languages()) {
        implementations.append(lang.name());
    }
    return implementations;
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

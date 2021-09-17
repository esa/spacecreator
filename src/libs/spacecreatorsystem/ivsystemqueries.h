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

#include "abstractsystemchecks.h"
#include "iveditorcore.h"

#include <QPointer>
#include <QSharedPointer>

namespace ivm {
class IVModel;
}

namespace scs {
class SpaceCreatorProject;

/*!
   Queries for the IV system only
 */
class IvSystemQueries : public dve::AbstractSystemChecks
{
    Q_OBJECT
public:
    explicit IvSystemQueries(SpaceCreatorProject *project, QObject *parent = nullptr);
    ~IvSystemQueries() override = default;

    void setIVCore(IVEditorCorePtr ivCore);
    IVEditorCorePtr ivCore() const;

    QStringList functionsNames() const override;
    QStringList pseudoFunctionsNames() const override;

    QList<QPair<QString, QString>> messages(
            const QString &sourceFunction, const QString &targetFunction) const override;

    bool connectionExists(const QString &sourceFunction, const QString &sourceInterface, const QString &targetFunction,
            const QString &targetInterface) const;

    ivm::IVModel *ivModel() const;

Q_SIGNALS:
    void ivDataReset();

private:
    IVEditorCorePtr m_ivCore;
    QPointer<scs::SpaceCreatorProject> m_project;
};

} // namespace scs

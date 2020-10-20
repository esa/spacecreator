/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "mscchecksbase.h"

#include <QPointer>
#include <QSharedPointer>
#include <QVector>

namespace msc {
class MSCEditorCore;
}

namespace spctr {
class MscModelStorage;

/*!
   Class to perform consitency checks an corrections between MSC and AADL
 */
class MscChecks : public shared::MscChecksBase
{
public:
    MscChecks(QObject *parent = nullptr);

    void setMscStorage(MscModelStorage *mscStorage);

    bool mscInstancesExists(const QString &name) override;
    void changeMscInstanceName(const QString &oldName, const QString &name) override;

    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores() const;

    QStringList allAadlFiles() const;
    QStringList allMscFiles() const;
    QStringList allAsn1Files() const;
    QStringList projectFiles(const QString &suffix) const;

private:
    QPointer<MscModelStorage> m_mscStorage;
};

}

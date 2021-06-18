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

#include <QPointer>
#include <QSharedPointer>

namespace dve {
class DVEditorCore;
}

namespace ive {
class IVEditorCore;
}

namespace ivm {
class IVModel;
}

namespace scs {

class DvSystemChecks : public dve::AbstractSystemChecks
{
    Q_OBJECT
public:
    explicit DvSystemChecks(QObject *parent = nullptr);
    ~DvSystemChecks() override = default;

    void setIVCore(QSharedPointer<ive::IVEditorCore> ivCore);
    QSharedPointer<ive::IVEditorCore> ivCore() const;

    void setDVCore(dve::DVEditorCore *dvCore);
    dve::DVEditorCore *dvCore() const;

    QStringList functionsNames() const override;

    ivm::IVModel *ivModel() const;

Q_SIGNALS:
    void ivDataReset();

private:
    QSharedPointer<ive::IVEditorCore> m_ivCore;
    QPointer<dve::DVEditorCore> m_dvCore;
};

} // namespace scs

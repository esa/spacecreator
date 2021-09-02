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

#include "dveditorcore.h"

#include <QObject>
#include <QPointer>

namespace dvm {
class DVFunction;
}

namespace scs {
class SpaceCreatorProject;

class DvSystemChecks : public QObject
{
    Q_OBJECT
public:
    explicit DvSystemChecks(QObject *parent = nullptr);

    // Setup functions
    void setStorage(scs::SpaceCreatorProject *storage);

public Q_SLOTS:
    bool checkDVFile(DVEditorCorePtr dvCore) const;

    bool checkFunctionBindings() const;
    bool checkFunctionBindings(DVEditorCorePtr dvCore) const;

Q_SIGNALS:

private:
    bool checkMessageIvValidity(const DVEditorCorePtr &dvCore) const;
    bool checkUniqueMessageBindings(const DVEditorCorePtr &dvCore) const;

    QPointer<SpaceCreatorProject> m_storage;
    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
};

} // namespace scs

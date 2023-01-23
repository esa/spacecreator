/*
   Copyright (C) 2022 European Space Agency - <maxime.perrotin@esa.int>

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

#include "chartlayoutmanagerbase.h"

#include "msccommandsstack.h"

namespace msc {

ChartLayoutManagerBase::ChartLayoutManagerBase(MscCommandsStack *undoStack, QObject *parent)
    : QObject { parent }
    , m_undoStack(undoStack)
{
    Q_ASSERT(undoStack != nullptr);
}

/**
   Returns the undo stack used for the modles displayed by this ChartLayoutManager
   \note The MscCommandsStack object is owned by some other class
 */
MscCommandsStack *ChartLayoutManagerBase::undoStack() const
{
    return m_undoStack;
}

/**
   Sets the object to check if the msc entities correspond to the iv model
 */
void ChartLayoutManagerBase::setSystemChecker(SystemChecks *checker)
{
    if (checker == m_systemChecker) {
        return;
    }
    m_systemChecker = checker;
    Q_EMIT systemCheckerChanged(m_systemChecker);
}

/**
   Returns the object to check if msc entities correspond to the iv model
 */
SystemChecks *ChartLayoutManagerBase::systemChecker() const
{
    return m_systemChecker;
}

/**
 * Returns if an update/recalculation is pending
 */
bool ChartLayoutManagerBase::layoutUpdatePending() const
{
    return false;
}

} // namespace msc

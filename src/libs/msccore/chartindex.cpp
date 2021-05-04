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

#include "chartindex.h"

#include <algorithm>

namespace msc {

ChartIndex::ChartIndex()
    : m_instance(nullptr)
    , m_index(-1)
{
}

ChartIndex::ChartIndex(MscInstance *inst, int idx)
    : m_instance(inst)
    , m_index(idx)
{
}

void ChartIndex::set(MscInstance *inst, int idx)
{
    m_instance = inst;
    m_index = idx;
}

bool ChartIndex::operator==(const ChartIndex &other) const
{
    return m_instance == other.instance() && m_index == other.index();
}

ChartIndexList::ChartIndexList() { }

ChartIndexList::ChartIndexList(std::initializer_list<ChartIndex> args)
{
    if (args.size() == 0) {
        return;
    }
    reserve(args.size());
    for (const ChartIndex &it : args) {
        append(it);
    }
}

void ChartIndexList::set(MscInstance *inst, int idx)
{
    set(ChartIndex(inst, idx));
}

void ChartIndexList::set(ChartIndex newIdx)
{
    int ii = instanceIndex(newIdx.instance());
    if (ii >= 0) {
        begin()[ii] = newIdx;
    } else {
        append(newIdx);
    }
}

bool ChartIndexList::operator==(const ChartIndexList &other) const
{
    if (size() != other.size()) {
        return false;
    }

    for (const ChartIndex &idx : other) {
        if (!contains(idx)) {
            return false;
        }
    }

    return true;
}

int ChartIndexList::instanceIndex(MscInstance *inst) const
{
    for (int i = 0; i < size(); ++i) {
        if (inst == at(i).instance()) {
            return i;
        }
    }
    return -1;
}

} // namespace msc

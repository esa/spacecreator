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

#include <QVector>
#include <initializer_list>

namespace msc {
class MscInstance;

/*!
   \brief The ChartIndex class represents one index (instance/horizontal and index/vertical) in a chart
 */
class ChartIndex
{
public:
    ChartIndex();
    ChartIndex(MscInstance *inst, int idx);

    void set(MscInstance *inst, int idx);
    void setIndex(int idx) { m_index = idx; }

    MscInstance *instance() const { return m_instance; }
    MscInstance *rowIndex() const { return m_instance; }
    int index() const { return m_index; }
    int columnIndex() const { return m_index; }

    bool operator==(const ChartIndex &other) const;

private:
    MscInstance *m_instance = nullptr;
    int m_index = -1;
};

/*!
   \brief The ChartIndexList class all the indexes one event has in a chart
 */
class ChartIndexList : public QVector<ChartIndex>
{
public:
    ChartIndexList();
    ChartIndexList(std::initializer_list<ChartIndex> args);
    void set(MscInstance *inst, int idx);
    void set(ChartIndex newIdx);

    bool operator==(const ChartIndexList &other) const;

private:
    int instanceIndex(MscInstance *inst) const;
};

} // namespace msc

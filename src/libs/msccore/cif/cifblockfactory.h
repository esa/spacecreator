/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "cif/cifblock.h"

#include <QVector>

namespace msc {
namespace cif {

class CifBlockFactory
{
public:
    static QVector<CifBlockShared> createBlocks(const QVector<QVector<CifLineShared>> &lines);
    static CifBlockShared createBlock(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlock(
            const CifLine::CifType cifType, const QVector<CifLineShared> &lines = QVector<CifLineShared>());

private:
    CifBlockFactory() = delete;
    CifBlockFactory(const CifBlockFactory &ot) = delete;

    struct CifBlockCreatorHelper {
        template<class T>
        CifBlockShared create(const QVector<CifLineShared> &lines)
        {
            return CifBlockShared(new T(lines));
        }
    };
};

} // ns cif
} // ns msc

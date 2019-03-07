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

#include "cifblock.h"

#include <QVector>

namespace msc {
namespace cif {

class CifBlockFactory
{
public:
    static QVector<CifBlockShared> createBlocks(const QVector<QVector<CifLineShared>> &lines);
    static CifBlockShared createBlock(const QVector<CifLineShared> &lines);

    static CifBlockShared createBlockAction(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockCall(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockComment(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockCondition(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockCreate(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockInstance(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockImport(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockMessage(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockMscDocument(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockReset(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockSet(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockStop(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockSubmsc(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockText(const QVector<CifLineShared> &lines);
    static CifBlockShared createBlockTimeout(const QVector<CifLineShared> &lines);

private:
    CifBlockFactory() = delete;
    CifBlockFactory(const CifBlockFactory &ot) = delete;
};

} // ns cif
} // ns msc

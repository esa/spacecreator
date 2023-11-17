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

#include "cifblockfactory.h"

#include "cifblocks.h"

#include <QDebug>

namespace msc {
namespace cif {

QVector<CifBlockShared> CifBlockFactory::createBlocks(const QVector<QVector<CifLineShared>> &lines)
{
    QVector<CifBlockShared> blocks;
    for (const QVector<CifLineShared> &blockLines : lines) {
        if (const CifBlockShared &block = createBlock(blockLines)) {
            blocks.append(block);
        }
    }
    return blocks;
}

CifBlockShared CifBlockFactory::createBlock(const QVector<CifLineShared> &lines)
{
    return lines.isEmpty() ? CifBlockShared() : createBlock(lines.first()->lineType(), lines);
}

/*static*/ CifBlockShared CifBlockFactory::createBlock(
        const CifLine::CifType cifType, const QVector<CifLineShared> &lines)
{
    switch (cifType) {
    case CifLine::CifType::Action:
        return CifBlockCreatorHelper().create<CifBlockAction>(lines);
    case CifLine::CifType::Call:
        return CifBlockCreatorHelper().create<CifBlockCall>(lines);
    case CifLine::CifType::Comment:
        return CifBlockCreatorHelper().create<CifBlockComment>(lines);
    case CifLine::CifType::Concurrent:
        return CifBlockCreatorHelper().create<CifBlockConcurrent>(lines);
    case CifLine::CifType::Condition:
        return CifBlockCreatorHelper().create<CifBlockCondition>(lines);
    case CifLine::CifType::Create:
        return CifBlockCreatorHelper().create<CifBlockCreate>(lines);
    case CifLine::CifType::Instance:
        return CifBlockCreatorHelper().create<CifBlockInstance>(lines);
    case CifLine::CifType::Import:
        return CifBlockCreatorHelper().create<CifBlockImport>(lines);
    case CifLine::CifType::Message:
        return CifBlockCreatorHelper().create<CifBlockMessage>(lines);
    case CifLine::CifType::MscDocument:
        return CifBlockCreatorHelper().create<CifBlockMscDocument>(lines);
    case CifLine::CifType::Reset:
        return CifBlockCreatorHelper().create<CifBlockReset>(lines);
    case CifLine::CifType::Set:
        return CifBlockCreatorHelper().create<CifBlockSet>(lines);
    case CifLine::CifType::Stop:
        return CifBlockCreatorHelper().create<CifBlockStop>(lines);
    case CifLine::CifType::Submsc:
        return CifBlockCreatorHelper().create<CifBlockSubmsc>(lines);
    case CifLine::CifType::Text:
        return CifBlockCreatorHelper().create<CifBlockText>(lines);
    case CifLine::CifType::Timeout:
        return CifBlockCreatorHelper().create<CifBlockTimeout>(lines);
    case CifLine::CifType::Requirement:
        return CifBlockCreatorHelper().create<CifBlockRequirement>(lines);
    default:
        break;
    }
    return CifBlockShared();
}

} // ns cif
} // ns msc

/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QDebug>

namespace msc {
namespace cif {

QVector<CifBlockShared> CifBlockFactory::createBlocks(const QVector<QVector<CifLineShared>> &lines)
{
    QVector<CifBlockShared> blocks;
    for (const QVector<CifLineShared> &blockLines : lines)
        if (const CifBlockShared &block = createBlock(blockLines))
            blocks.append(block);
    return blocks;
}

CifBlockShared CifBlockFactory::createBlock(const QVector<CifLineShared> &lines)
{
    for (const CifLineShared &line : lines) {
        switch (line->entityType()) {
        case CifLine::CifType::Action:
            return createBlockAction(lines);
        case CifLine::CifType::Call:
            return createBlockCall(lines);
        case CifLine::CifType::Comment:
            return createBlockComment(lines);
        case CifLine::CifType::Condition:
            return createBlockCondition(lines);
        case CifLine::CifType::Create:
            return createBlockCreate(lines);
        case CifLine::CifType::Instance:
            return createBlockInstance(lines);
        case CifLine::CifType::Import:
            return createBlockImport(lines);
        case CifLine::CifType::Message:
            return createBlockMessage(lines);
        case CifLine::CifType::MscDocument:
            return createBlockMscDocument(lines);
        case CifLine::CifType::Reset:
            return createBlockReset(lines);
        case CifLine::CifType::Set:
            return createBlockSet(lines);
        case CifLine::CifType::Stop:
            return createBlockStop(lines);
        case CifLine::CifType::Submsc:
            return createBlockSubmsc(lines);
        case CifLine::CifType::Text:
            return createBlockText(lines);
        case CifLine::CifType::Timeout:
            return createBlockTimeout(lines);
        default:
            break;
        }
    }
    return CifBlockShared();
}

CifBlockShared CifBlockFactory::createBlockAction(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockCall(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockComment(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockCondition(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockCreate(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockInstance(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockImport(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockMessage(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockMscDocument(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockReset(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockSet(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockStop(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockSubmsc(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockText(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}
CifBlockShared CifBlockFactory::createBlockTimeout(const QVector<CifLineShared> &lines)
{
    Q_UNUSED(lines);
    qWarning() << Q_FUNC_INFO << "Not implemented yet";
    return CifBlockShared();
}

} // ns cif
} // ns msc

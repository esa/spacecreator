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

namespace msc {
namespace cif {

using SharedCifLines = QVector<CifLineShared>;

class CifBlockAction : public CifBlock
{
public:
    explicit CifBlockAction(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockCall : public CifBlock
{
public:
    explicit CifBlockCall(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockComment : public CifBlock
{
public:
    explicit CifBlockComment(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockConcurrent : public CifBlock
{
public:
    explicit CifBlockConcurrent(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockCondition : public CifBlock
{
public:
    explicit CifBlockCondition(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockCreate : public CifBlock
{
public:
    explicit CifBlockCreate(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockInstance : public CifBlock
{
public:
    explicit CifBlockInstance(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
    static constexpr int AxisWidth { 800 };
};

class CifBlockImport : public CifBlock
{
public:
    explicit CifBlockImport(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockMessage : public CifBlock
{
public:
    explicit CifBlockMessage(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockMscDocument : public CifBlock
{
public:
    explicit CifBlockMscDocument(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockReset : public CifBlock
{
public:
    explicit CifBlockReset(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockSet : public CifBlock
{
public:
    explicit CifBlockSet(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockStop : public CifBlock
{
public:
    explicit CifBlockStop(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockSubmsc : public CifBlock
{
public:
    explicit CifBlockSubmsc(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockText : public CifBlock
{
public:
    explicit CifBlockText(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockTimeout : public CifBlock
{
public:
    explicit CifBlockTimeout(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }
    CifLine::CifType blockType() const override;
};

class CifBlockRequirement : public CifBlock
{
public:
    explicit CifBlockRequirement(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }

    CifLine::CifType blockType() const override;
};

class CifBlockRequirementsUrl : public CifBlock
{
public:
    explicit CifBlockRequirementsUrl(const SharedCifLines &lines = SharedCifLines())
        : CifBlock(lines)
    {
    }

    CifLine::CifType blockType() const override;
};

} // ns cif
} // ns msc

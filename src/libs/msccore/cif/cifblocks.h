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

class CifBlockAction : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockCall : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockComment : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockConcurrent : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockCondition : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockCreate : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockInstance : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
    static constexpr int AxisWidth { 800 };
};

class CifBlockImport : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockMessage : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockMscDocument : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockReset : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockSet : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockStop : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockSubmsc : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockText : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

class CifBlockTimeout : public CifBlock
{
public:
    CifLine::CifType blockType() const override;
};

} // ns cif
} // ns msc

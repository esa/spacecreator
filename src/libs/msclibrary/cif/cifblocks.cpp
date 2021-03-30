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

#include "cifblocks.h"

#include "ciflines.h"

namespace msc {
namespace cif {

CifLine::CifType CifBlockAction::blockType() const
{
    return CifLine::CifType::Action;
}
CifLine::CifType CifBlockCall::blockType() const
{
    return CifLine::CifType::Call;
}

CifLine::CifType CifBlockComment::blockType() const
{
    return CifLine::CifType::Comment;
}

CifLine::CifType CifBlockConcurrent::blockType() const
{
    return CifLine::CifType::Concurrent;
}

CifLine::CifType CifBlockCondition::blockType() const
{
    return CifLine::CifType::Condition;
}

CifLine::CifType CifBlockCreate::blockType() const
{
    return CifLine::CifType::Create;
}

CifLine::CifType CifBlockInstance::blockType() const
{
    return CifLine::CifType::Instance;
}

CifLine::CifType CifBlockImport::blockType() const
{
    return CifLine::CifType::Import;
}

CifLine::CifType CifBlockMessage::blockType() const
{
    return CifLine::CifType::Message;
}

CifLine::CifType CifBlockMscDocument::blockType() const
{
    return CifLine::CifType::MscDocument;
}

CifLine::CifType CifBlockReset::blockType() const
{
    return CifLine::CifType::Reset;
}

CifLine::CifType CifBlockSet::blockType() const
{
    return CifLine::CifType::Set;
}

CifLine::CifType CifBlockStop::blockType() const
{
    return CifLine::CifType::Stop;
}

CifLine::CifType CifBlockSubmsc::blockType() const
{
    return CifLine::CifType::Submsc;
}

CifLine::CifType CifBlockText::blockType() const
{
    return CifLine::CifType::Text;
}

CifLine::CifType CifBlockTimeout::blockType() const
{
    return CifLine::CifType::Timeout;
}

} // ns cif
} // ns msc

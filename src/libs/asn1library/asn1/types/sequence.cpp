/****************************************************************************
**
** Copyright (C) 2017-2022 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a programme and funded by
** European Space Agency.
**
** This Library is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "sequence.h"

#include "typemutatingvisitor.h"
#include "typereadingvisitor.h"

using namespace Asn1Acn;
using namespace Asn1Acn::Types;

void Sequence::accept(TypeMutatingVisitor &visitor)
{
    visitor.visit(*this);
}

void Sequence::accept(TypeReadingVisitor &visitor) const
{
    visitor.visit(*this);
}

std::unique_ptr<Type> Sequence::clone() const
{
    return std::make_unique<Sequence>(*this);
}

std::optional<QString> Sequence::postEncodingFunction() const
{
    return m_postEncodingFunction;
}

void Sequence::setPostEncodingFunction(QString postEncodingFunction)
{
    m_postEncodingFunction = std::move(postEncodingFunction);
}

std::optional<QString> Sequence::postDecodingValidator() const
{
    return m_postDecodingValidator;
}

void Sequence::setPostDecodingValidator(QString postDecodingValidator)
{
    m_postDecodingValidator = std::move(postDecodingValidator);
}

std::vector<PatcherSnippet> Sequence::patcherSnippets() const
{
    return m_patcherSnippets;
}

void Sequence::addPatcherSnippet(PatcherSnippet patcherSnippet)
{
    m_patcherSnippets.push_back(std::move(patcherSnippet));
}

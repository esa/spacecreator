/****************************************************************************
**
** Copyright (C) 2017-2021 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Library.
**
** Library was developed under a program and funded by
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
#pragma once

namespace Asn1Acn {

class Definitions;
class File;
class Root;
class TypeAssignment;
class ValueAssignment;
class Project;

class Visitor
{
protected:
    Visitor() {}

public:
    virtual ~Visitor();

    virtual void visit(const Root &root) = 0;
    virtual void visit(const Definitions &defs) = 0;
    virtual void visit(const File &file) = 0;
    virtual void visit(const TypeAssignment &type) = 0;
    virtual void visit(const ValueAssignment &value) = 0;
    virtual void visit(const Project &project) = 0;
};

}

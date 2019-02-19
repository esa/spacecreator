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

#ifndef ASN1PARSERVISITOR_H
#define ASN1PARSERVISITOR_H

#include "parser/ASNBaseVisitor.h"

namespace asn1 {

class Asn1ParserVisitor : public ASNBaseVisitor
{
public:
    explicit Asn1ParserVisitor();

    antlrcpp::Any visitAssignment(ASNParser::AssignmentContext *ctx) override;
    antlrcpp::Any visitBuiltinType(ASNParser::BuiltinTypeContext *context) override;
    antlrcpp::Any visitReferencedType(ASNParser::ReferencedTypeContext *context) override;

    antlrcpp::Any visitConstraint(ASNParser::ConstraintContext *context) override;
    antlrcpp::Any visitSubtypeElements(ASNParser::SubtypeElementsContext *context) override;

    antlrcpp::Any visitSequenceType(ASNParser::SequenceTypeContext *context) override;
    antlrcpp::Any visitComponentType(ASNParser::ComponentTypeContext *context) override;
    antlrcpp::Any visitNamedType(ASNParser::NamedTypeContext *context) override;

    antlrcpp::Any visitSequenceOfType(ASNParser::SequenceOfTypeContext *context) override;

    antlrcpp::Any visitChoiceType(ASNParser::ChoiceTypeContext *context) override;
};

} // namespace asn1

#endif // ASN1PARSERVISITOR_H

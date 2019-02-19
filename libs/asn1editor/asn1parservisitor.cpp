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

#include "asn1parservisitor.h"

#include <QDebug>

namespace asn1 {

Asn1ParserVisitor::Asn1ParserVisitor() {}

antlrcpp::Any Asn1ParserVisitor::visitAssignment(ASNParser::AssignmentContext *ctx)
{
    if (ctx) {
        qDebug() << "assigment: " << QString::fromStdString(ctx->IDENTIFIER()->getText());
    }

    return visitChildren(ctx);
}

antlrcpp::Any Asn1ParserVisitor::visitBuiltinType(ASNParser::BuiltinTypeContext *context)
{
    if (context) {
        qDebug() << "visitBuiltinType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitReferencedType(ASNParser::ReferencedTypeContext *context)
{
    if (context) {
        qDebug() << "visitReferencedType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitConstraint(ASNParser::ConstraintContext *context)
{
    if (context) {
        qDebug() << "visitConstraint: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitSequenceType(ASNParser::SequenceTypeContext *context)
{
    if (context) {
        qDebug() << "visitSequenceType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitComponentType(ASNParser::ComponentTypeContext *context)
{
    if (context) {
        qDebug() << "visitComponentType: " << QString::fromStdString(context->getText());

        if (context->OPTIONAL_LITERAL())
            qDebug() << "visitSequenceType OPTIONAL_LITERAL: "
                     << QString::fromStdString(context->OPTIONAL_LITERAL()->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitNamedType(ASNParser::NamedTypeContext *context)
{
    if (context && context->IDENTIFIER()) {
        qDebug() << "visitNamedType: " << QString::fromStdString(context->IDENTIFIER()->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitSequenceOfType(ASNParser::SequenceOfTypeContext *context)
{
    if (context) {
        qDebug() << "visitSequenceOfType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitChoiceType(ASNParser::ChoiceTypeContext *context)
{
    if (context) {
        qDebug() << "visitChoiceType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

} // namespace asn1

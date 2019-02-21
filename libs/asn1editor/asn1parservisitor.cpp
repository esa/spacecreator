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

#include "asn1const.h"

#include <QDebug>

namespace asn1 {

Asn1ParserVisitor::Asn1ParserVisitor() {}

antlrcpp::Any Asn1ParserVisitor::visitAssignment(ASNParser::AssignmentContext *ctx)
{
    if (ctx) {
        updateAns1TypesData();

        m_currentType[ASN1_NAME] = QString::fromStdString(ctx->IDENTIFIER()->getText());
        m_currentType[ASN1_IS_OPTIONAL] = false;
        m_currentType[ASN1_TYPE] = ASN1_TYPE_STRING;

        qDebug() << "assigment: " << QString::fromStdString(ctx->IDENTIFIER()->getText());
    }

    return visitChildren(ctx);
}

antlrcpp::Any Asn1ParserVisitor::visitIntegerType(ASNParser::IntegerTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_INTEGER);

        qDebug() << "visitIntegerType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitRealType(ASNParser::RealTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_DOUBLE);

        qDebug() << "visitRealType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitBooleanType(ASNParser::BooleanTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_BOOL);

        qDebug() << "visitBooleanType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitEnumeratedType(ASNParser::EnumeratedTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_ENUMERATED);

        qDebug() << "visitEnumeratedType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitSequenceType(ASNParser::SequenceTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_SEQUENCE);

        qDebug() << "visitSequenceType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitSequenceOfType(ASNParser::SequenceOfTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_SEQUENCEOF);

        qDebug() << "visitSequenceOfType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitChoiceType(ASNParser::ChoiceTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_CHOICE);
        m_choiceType = true;

        qDebug() << "visitChoiceType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitSetType(ASNParser::SetTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_SET);

        qDebug() << "visitSetType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitSetOfType(ASNParser::SetOfTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_SETOF);

        qDebug() << "visitSetOfType: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

// antlrcpp::Any Asn1ParserVisitor::visitBuiltinType(ASNParser::BuiltinTypeContext *context)
//{
//    if (context) {
//        qDebug() << "visitBuiltinType: " << QString::fromStdString(context->getText());
//    }

//    return visitChildren(context);
//}

antlrcpp::Any Asn1ParserVisitor::visitReferencedType(ASNParser::ReferencedTypeContext *context)
{
    if (context) {
        setAns1Type(QString::fromStdString(context->getText()));

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

antlrcpp::Any Asn1ParserVisitor::visitSubtypeElements(ASNParser::SubtypeElementsContext *context)
{
    if (context) {
        if (context->MIN_LITERAL() && context->value().size() == 1) {
            setAns1TypeConstraint(ASN1_MAX, QString::fromStdString(context->value(0)->getText()));
        } else if (context->MAX_LITERAL() && context->value().size() == 1) {
            setAns1TypeConstraint(ASN1_MIN, QString::fromStdString(context->value(0)->getText()));
        } else if (context->value().size() == 2) {
            setAns1TypeConstraint(ASN1_MIN, QString::fromStdString(context->value(0)->getText()));
            setAns1TypeConstraint(ASN1_MAX, QString::fromStdString(context->value(1)->getText()));
        }

        qDebug() << "visitSubtypeElements: " << QString::fromStdString(context->getText()) << context->value().size();
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitComponentType(ASNParser::ComponentTypeContext *context)
{
    if (context) {
        qDebug() << "visitComponentType: " << QString::fromStdString(context->getText());

        if (context->OPTIONAL_LITERAL()) {
            setAns1TypeOptional(true);

            qDebug() << "visitSequenceType OPTIONAL_LITERAL: "
                     << QString::fromStdString(context->OPTIONAL_LITERAL()->getText());
        }
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitNamedType(ASNParser::NamedTypeContext *context)
{
    if (context && context->IDENTIFIER()) {
        updateNamedTypeData();

        m_namedTypeData[ASN1_NAME] = QString::fromStdString(context->IDENTIFIER()->getText());
        setAns1TypeOptional(false);

        qDebug() << "visitNamedType: " << QString::fromStdString(context->IDENTIFIER()->getText());
    }

    return visitChildren(context);
}

// antlrcpp::Any Asn1ParserVisitor::visitEnumerations(ASNParser::EnumerationsContext *context)
//{
//    if (context) {
//        qDebug() << "visitEnumerations: " << QString::fromStdString(context->getText());
//    }

//    return visitChildren(context);
//}

antlrcpp::Any Asn1ParserVisitor::visitEnumerationItem(ASNParser::EnumerationItemContext *context)
{
    if (context) {
        m_enumValues.append(QString::fromStdString(context->getText()));

        qDebug() << "visitEnumerationItem: " << QString::fromStdString(context->getText());
    }

    return visitChildren(context);
}

QVariantList Asn1ParserVisitor::detachTypesData()
{
    updateAns1TypesData();

    return m_asn1TypesData;
}

void Asn1ParserVisitor::setAns1Type(const QString &asn1Type)
{
    if (m_choiceType) {
        m_namedTypeData[ASN1_TYPE] = asn1Type;
    } else {
        m_currentType[ASN1_TYPE] = asn1Type;
    }
}

void Asn1ParserVisitor::setAns1TypeConstraint(const QString &constraintKey, const QString &constraintValue)
{
    if (m_choiceType) {
        m_namedTypeData[constraintKey] = constraintValue;
    } else {
        m_currentType[constraintKey] = constraintValue;
    }
}

void Asn1ParserVisitor::setAns1TypeOptional(bool value)
{
    if (m_choiceType) {
        m_namedTypeData[ASN1_IS_OPTIONAL] = value;
    } else {
        m_currentType[ASN1_IS_OPTIONAL] = value;
    }
}

void Asn1ParserVisitor::updateAns1TypesData()
{
    updateNamedTypeData();

    if (!m_currentType.empty()) {
        m_choiceType = false;

        if (!m_enumValues.empty()) {
            m_currentType[ASN1_VALUES] = m_enumValues;
            m_enumValues.clear();
        }

        m_asn1TypesData.append(m_currentType);
        m_currentType.clear();
    }
}

void Asn1ParserVisitor::updateNamedTypeData()
{
    if (!m_namedTypeData.empty()) {
        if (m_choiceType) {
            QVariantList choices = m_currentType[ASN1_CHOICES].toList();
            choices.append(m_namedTypeData);
            m_currentType[ASN1_CHOICES] = choices;
        }

        m_namedTypeData.clear();
    }
}

} // namespace asn1

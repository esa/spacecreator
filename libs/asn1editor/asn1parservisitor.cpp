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

#include <algorithm>

namespace asn1 {

Asn1ParserVisitor::Asn1ParserVisitor() {}

antlrcpp::Any Asn1ParserVisitor::visitAssignment(ASNParser::AssignmentContext *ctx)
{
    if (ctx) {
        m_currentType = createAsn1TypeData(QString::fromStdString(ctx->IDENTIFIER()->getText()));
        m_asn1TypesData.append(m_currentType);
    }

    return visitChildren(ctx);
}

antlrcpp::Any Asn1ParserVisitor::visitIntegerType(ASNParser::IntegerTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_INTEGER);
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitRealType(ASNParser::RealTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_DOUBLE);
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitBooleanType(ASNParser::BooleanTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_BOOL);
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitEnumeratedType(ASNParser::EnumeratedTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_ENUMERATED);
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitSequenceType(ASNParser::SequenceTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_SEQUENCE);
        m_parentList.append(m_currentType);
    }

    auto result = visitChildren(context);

    takeLastParent();

    return result;
}

antlrcpp::Any Asn1ParserVisitor::visitSequenceOfType(ASNParser::SequenceOfTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_SEQUENCEOF);
        m_parentList.append(m_currentType);

        m_currentType = createAsn1TypeData();
        m_parentList.last()->m_children.append(m_currentType);
        m_segOfType = true;
    }

    auto result = visitChildren(context);

    takeLastParent();

    return result;
}

antlrcpp::Any Asn1ParserVisitor::visitChoiceType(ASNParser::ChoiceTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_CHOICE);
        m_parentList.append(m_currentType);
    }

    auto result = visitChildren(context);

    takeLastParent();

    return result;
}

antlrcpp::Any Asn1ParserVisitor::visitSetType(ASNParser::SetTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_SEQUENCE);
        m_parentList.append(m_currentType);
    }

    auto result = visitChildren(context);

    takeLastParent();

    return result;
}

antlrcpp::Any Asn1ParserVisitor::visitSetOfType(ASNParser::SetOfTypeContext *context)
{
    if (context) {
        setAns1Type(ASN1_TYPE_SEQUENCEOF);
        m_parentList.append(m_currentType);

        m_currentType = createAsn1TypeData();
        m_parentList.last()->m_children.append(m_currentType);
        m_segOfType = true;
    }

    auto result = visitChildren(context);

    takeLastParent();

    return result;
}

antlrcpp::Any Asn1ParserVisitor::visitReferencedType(ASNParser::ReferencedTypeContext *context)
{
    if (context) {
        // referenced type should be already defined
        auto find_it = std::find_if(m_asn1TypesData.begin(), m_asn1TypesData.end(), [&](const Asn1TypeDataPtr &item) {
            return item->m_name == QString::fromStdString(context->getText());
        });

        if (find_it != m_asn1TypesData.end()) {
            m_currentType->m_type = (*find_it)->m_type;
            m_currentType->m_min = (*find_it)->m_min;
            m_currentType->m_max = (*find_it)->m_max;
            m_currentType->m_optional = (*find_it)->m_optional;
            m_currentType->m_children = (*find_it)->m_children;
        }
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
        } else if (context->value().size()) {
            setAns1TypeConstraint(ASN1_MIN, QString::fromStdString(context->value(0)->getText()));
            setAns1TypeConstraint(ASN1_MAX,
                                  QString::fromStdString(context->value(context->value().size() - 1)->getText()));
        }

        if (context->value().size()) {
            m_segOfType = false;
        }
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitNamedType(ASNParser::NamedTypeContext *context)
{
    if (context && context->IDENTIFIER()) {
        if (!m_parentList.isEmpty()) {
            m_currentType = createAsn1TypeData(QString::fromStdString(context->IDENTIFIER()->getText()));
            m_parentList.last()->m_children.append(m_currentType);

            if (auto parent = dynamic_cast<ASNParser::ComponentTypeContext *>(context->parent)) {
                if (parent->OPTIONAL_LITERAL()) {
                    setAns1TypeOptional(true);
                }
            }
        } else {
            throw QObject::tr("No parent item");
        }
    }

    return visitChildren(context);
}

antlrcpp::Any Asn1ParserVisitor::visitEnumerationItem(ASNParser::EnumerationItemContext *context)
{
    if (context) {
        m_currentType->m_children.append(createAsn1TypeData(QString::fromStdString(context->getText())));
    }

    return visitChildren(context);
}

QVariantList Asn1ParserVisitor::detachTypesData()
{
    QVariantList typeDataList;

    auto enumValues = [&](const Asn1TypeDataList dataList) {
        QVariantList result;

        for (const auto &item : dataList) {
            result.append(item->m_name);
        }

        return result;
    };

    std::function<QVariantMap(const Asn1TypeDataPtr &)> typeMap = [&](const Asn1TypeDataPtr &asn1Type) -> QVariantMap {
        QVariantMap result;

        result[ASN1_NAME] = asn1Type->m_name;
        result[ASN1_TYPE] = asn1Type->m_type;
        result[ASN1_IS_OPTIONAL] = asn1Type->m_optional;

        if (!asn1Type->m_min.isEmpty()) {
            result[ASN1_MIN] = asn1Type->m_min;
        }
        if (!asn1Type->m_min.isEmpty()) {
            result[ASN1_MAX] = asn1Type->m_max;
        }

        if (!asn1Type->m_children.isEmpty()) {
            if (asn1Type->m_type == ASN1_TYPE_ENUMERATED) {
                result[ASN1_VALUES] = enumValues(asn1Type->m_children);
            } else {
                QVariantList itemList;

                for (const auto &item : asn1Type->m_children) {
                    itemList.append(typeMap(item));
                }

                result[asn1Type->m_type == ASN1_TYPE_CHOICE
                               ? ASN1_CHOICES
                               : (asn1Type->m_type == ASN1_TYPE_SEQUENCE ? ASN1_CHILDREN : ASN1_SEQOFTYPE)] = itemList;
            }
        }

        return result;
    };

    for (const auto &item : m_asn1TypesData) {
        typeDataList.append(typeMap(item));
    }

    return typeDataList;
}

void Asn1ParserVisitor::setAns1Type(const QString &asn1Type)
{
    if (m_currentType) {
        m_currentType->m_type = asn1Type;
    }
}

void Asn1ParserVisitor::setAns1TypeConstraint(const QString &constraintKey, const QString &constraintValue)
{
    auto itemType = m_segOfType ? (!m_parentList.isEmpty() ? m_parentList.last() : Asn1TypeDataPtr()) : m_currentType;

    if (!itemType.isNull()) {
        if (constraintKey == ASN1_MIN) {
            itemType->m_min = constraintValue;
        } else {
            itemType->m_max = constraintValue;
        }
    }
}

void Asn1ParserVisitor::setAns1TypeOptional(bool value)
{
    if (m_currentType) {
        m_currentType->m_optional = value;
    }
}

void Asn1ParserVisitor::takeLastParent()
{
    if (!m_parentList.isEmpty()) {
        m_parentList.takeLast();
    }
}

Asn1ParserVisitor::Asn1TypeDataPtr Asn1ParserVisitor::createAsn1TypeData(const QString &name)
{
    auto asn1Data = Asn1TypeDataPtr(new Asn1TypeData(name));
    asn1Data->m_optional = false;
    asn1Data->m_type = ASN1_TYPE_STRING;

    return asn1Data;
}

} // namespace asn1

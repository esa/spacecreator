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

#include "asn1const.h"
#include "parser/ASNBaseVisitor.h"

#include <QSharedPointer>
#include <QVariant>

namespace asn1 {

class Asn1ParserVisitor : public ASNBaseVisitor
{
public:
    explicit Asn1ParserVisitor();

    antlrcpp::Any visitAssignment(ASNParser::AssignmentContext *ctx) override;

    antlrcpp::Any visitIntegerType(ASNParser::IntegerTypeContext *context) override;
    antlrcpp::Any visitRealType(ASNParser::RealTypeContext *context) override;
    antlrcpp::Any visitBooleanType(ASNParser::BooleanTypeContext *context) override;
    antlrcpp::Any visitEnumeratedType(ASNParser::EnumeratedTypeContext *context) override;
    antlrcpp::Any visitSequenceType(ASNParser::SequenceTypeContext *context) override;
    antlrcpp::Any visitSequenceOfType(ASNParser::SequenceOfTypeContext *context) override;
    antlrcpp::Any visitChoiceType(ASNParser::ChoiceTypeContext *context) override;
    antlrcpp::Any visitSetType(ASNParser::SetTypeContext *context) override;
    antlrcpp::Any visitSetOfType(ASNParser::SetOfTypeContext *context) override;
    antlrcpp::Any visitReferencedType(ASNParser::ReferencedTypeContext *context) override;
    antlrcpp::Any visitSubtypeElements(ASNParser::SubtypeElementsContext *context) override;
    antlrcpp::Any visitNamedType(ASNParser::NamedTypeContext *context) override;
    antlrcpp::Any visitEnumerationItem(ASNParser::EnumerationItemContext *context) override;

    QVariantList detachTypesData();

private:
    struct Asn1TypeData;

    using Asn1TypeDataPtr = QSharedPointer<Asn1TypeData>;
    using Asn1TypeDataList = QList<Asn1TypeDataPtr>;

    void setAns1Type(const ASN1Type &asn1Type);
    void setAns1TypeConstraint(const QString &constraintKey, const QString &constraintValue);
    void setAns1TypeOptional(bool value);
    void takeLastParent();

    Asn1TypeDataPtr createAsn1TypeData(const QString &name = {});

private:
    struct Asn1TypeData {
        Asn1TypeData(const QString &name = {})
            : m_name(name)
        {
        }

        QString m_name;
        ASN1Type m_type;
        bool m_optional;
        QString m_min;
        QString m_max;

        Asn1TypeDataList m_children;
    };

    Asn1TypeDataList m_asn1TypesData;
    Asn1TypeDataPtr m_currentType;
    Asn1TypeDataList m_parentList;

    bool m_segOfType = false;
};

} // namespace asn1

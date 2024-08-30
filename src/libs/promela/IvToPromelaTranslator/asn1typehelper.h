/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2024 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include <QList>
#include <QString>
#include <QStringList>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/types/type.h>
#include <promela/PromelaModel/expression.h>
#include <promela/PromelaModel/variableref.h>

namespace promela::translator {
class Asn1TypeHelper
{
public:
    struct PrintfTemplate {
        QString m_fmt;
        QList<promela::model::VariableRef> m_args;
    };

public:
    Asn1TypeHelper(const Asn1Acn::Asn1Model *asn1Model, QString target, QString source, size_t nestingLevel);

    QString createAssignmentTemplateFromPromelaToC(const QString &typeName);
    QString createAssignmentTemplateFromCToPromela(const QString &typeName);
    QList<PrintfTemplate> generatePrintfTemplate(const QString &typeName);

    QString createAssignmentTemplateFromPromelaToC(const QString &cTypeName, const Asn1Acn::Types::Type *type);
    QString createAssignmentTemplateFromCToPromela(const QString &cTypeName, const Asn1Acn::Types::Type *type);
    QList<promela::model::VariableRef> generateListOfFields(const Asn1Acn::Types::Type *type);

private:
    const Asn1Acn::TypeAssignment *findType(const QString &name);

    QString sequenceAssignmentFromPromelaToC(const QString &cTypeName, const Asn1Acn::Types::Sequence *type);
    QString sequenceOfAssignmentFromPromelaToC(const QString &cTypeName, const Asn1Acn::Types::SequenceOf *type);
    QString choiceAssignmentFromPromelaToC(const QString &cTypeName, const Asn1Acn::Types::Choice *type);
    QString octetStringAssignmentFromPromelaToC(const Asn1Acn::Types::OctetString *type);
    QString ia5StringAssignmentFromPromelaToC(const Asn1Acn::Types::IA5String *type);

    QString sequenceAssignmentFromCToPromela(const QString &cTypeName, const Asn1Acn::Types::Sequence *type);
    QString sequenceOfAssignmentFromCToPromela(const QString &cTypeName, const Asn1Acn::Types::SequenceOf *type);
    QString choiceAssignmentFromCToPromela(const QString &cTypeName, const Asn1Acn::Types::Choice *type);
    QString octetStringAssignmentFromCToPromela(const Asn1Acn::Types::OctetString *type);
    QString ia5StringAssignmentFromCToPromela(const Asn1Acn::Types::IA5String *type);

    QList<promela::model::VariableRef> sequenceListOfFields(const Asn1Acn::Types::Sequence *type);
    QList<promela::model::VariableRef> sequenceOfListOfFields(const Asn1Acn::Types::SequenceOf *type);
    QList<promela::model::VariableRef> choiceListOfFields(const Asn1Acn::Types::Choice *type);
    QList<promela::model::VariableRef> octetStringListOfFields(const Asn1Acn::Types::OctetString *type);
    QList<promela::model::VariableRef> ia5StringListOfFields(const Asn1Acn::Types::IA5String *type);

    QString addIndent(const QString &text);
    QString createIteratorVariableName();

private:
    const Asn1Acn::Asn1Model *m_asn1Model;
    const QString m_target;
    const QString m_source;
    const size_t m_nestingLevel;
};
}

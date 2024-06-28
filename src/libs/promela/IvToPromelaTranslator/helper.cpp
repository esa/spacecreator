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

#include "helper.h"

#include "sequencecomponentvisitor.h"

#include <QDebug>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/sequence.h>
#include <asn1library/asn1/types/sequenceof.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/types/userdefinedtype.h>
#include <conversion/common/translation/exceptions.h>
#include <promela/Asn1ToPromelaTranslator/visitors/sizeconstraintvisitor.h>
#include <promela/PromelaModel/constant.h>

using Asn1Acn::IntegerValue;
using conversion::translator::TranslationException;
using promela::model::Constant;
using promela::model::Expression;
using promela::model::VariableRef;

namespace promela::translator {
Helper::Helper(const Asn1Acn::Asn1Model *asn1Model, QString target, QString source)
    : m_asn1Model(asn1Model)
    , m_target(std::move(target))
    , m_source(std::move(source))
{
}

QString Helper::createAssignmentTemplateFromPromelaToC(const QString &typeName)
{
    const Asn1Acn::TypeAssignment *type = findType(typeName);
    if (type == nullptr) {
        QString message = QString("Cannot find type %1").arg(typeName);
        throw TranslationException(std::move(message));
    }

    return createAssignmentTemplateFromPromelaToC(type->type());
}

QString Helper::createAssignmentTemplateFromCToPromela(const QString &typeName)
{
    const Asn1Acn::TypeAssignment *type = findType(typeName);
    if (type == nullptr) {
        QString message = QString("Cannot find type %1").arg(typeName);
        throw TranslationException(std::move(message));
    }

    return createAssignmentTemplateFromCToPromela(type->type());
}

QList<Helper::PrintfTemplate> Helper::generatePrintfTemplate(const QString &typeName)
{
    const Asn1Acn::TypeAssignment *type = findType(typeName);
    if (type == nullptr) {
        QString message = QString("Cannot find type %1").arg(typeName);
        throw TranslationException(std::move(message));
    }

    QList<VariableRef> fields = generateListOfFields(type->type());

    QList<PrintfTemplate> result;
    while (!fields.empty()) {
        const qsizetype maxGroupLength = 10;
        qsizetype groupLength = std::min(fields.size(), maxGroupLength);

        QList<VariableRef> group = fields.sliced(0, groupLength);
        fields.remove(0, groupLength);

        QString fmt = QString("%d, ").repeated(groupLength - 1);
        fmt.append("%d");

        result.append(PrintfTemplate {
                fmt,
                group,
        });
    }

    return result;
}

QString Helper::createAssignmentTemplateFromPromelaToC(const Asn1Acn::Types::Type *type)
{
    switch (type->typeEnum()) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER:
    case Asn1Acn::Types::Type::ASN1Type::REAL:
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN:
    case Asn1Acn::Types::Type::ASN1Type::ENUMERATED:
        // ordinary assignment
        return m_target + " = " + m_source + ";\n";

    case Asn1Acn::Types::Type::ASN1Type::SEQUENCE:
        return sequenceAssignmentFromPromelaToC(dynamic_cast<const Asn1Acn::Types::Sequence *>(type));

    case Asn1Acn::Types::Type::ASN1Type::SEQUENCEOF:
        return sequenceOfAssignmentFromPromelaToC(dynamic_cast<const Asn1Acn::Types::SequenceOf *>(type));

    case Asn1Acn::Types::Type::ASN1Type::CHOICE:
        return choiceAssignmentFromPromelaToC(dynamic_cast<const Asn1Acn::Types::Choice *>(type));

    case Asn1Acn::Types::Type::ASN1Type::USERDEFINED: {
        const Asn1Acn::Types::UserdefinedType *t = dynamic_cast<const Asn1Acn::Types::UserdefinedType *>(type);
        return createAssignmentTemplateFromPromelaToC(t->type());
    }

    default:
        QString message = QString("Not implemented %1").arg(type->typeEnum());
        throw TranslationException(std::move(message));
        // not implemented yet
        break;
    }
}

QString Helper::createAssignmentTemplateFromCToPromela(const Asn1Acn::Types::Type *type)
{
    switch (type->typeEnum()) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER:
    case Asn1Acn::Types::Type::ASN1Type::REAL:
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN:
    case Asn1Acn::Types::Type::ASN1Type::ENUMERATED:
        // ordinary assignment
        return m_target + " = " + m_source + ";\n";

    case Asn1Acn::Types::Type::ASN1Type::SEQUENCE:
        return sequenceAssignmentFromCToPromela(dynamic_cast<const Asn1Acn::Types::Sequence *>(type));

    case Asn1Acn::Types::Type::ASN1Type::SEQUENCEOF:
        return sequenceOfAssignmentFromCToPromela(dynamic_cast<const Asn1Acn::Types::SequenceOf *>(type));

    case Asn1Acn::Types::Type::ASN1Type::CHOICE:
        return choiceAssignmentFromCToPromela(dynamic_cast<const Asn1Acn::Types::Choice *>(type));

    case Asn1Acn::Types::Type::ASN1Type::USERDEFINED: {
        const Asn1Acn::Types::UserdefinedType *t = dynamic_cast<const Asn1Acn::Types::UserdefinedType *>(type);
        return createAssignmentTemplateFromCToPromela(t->type());
    }

    default:
        QString message = QString("Not implemented %1").arg(type->typeEnum());
        throw TranslationException(std::move(message));
        // not implemented yet
        break;
    }
}

QList<VariableRef> Helper::generateListOfFields(const Asn1Acn::Types::Type *type)
{
    switch (type->typeEnum()) {
    case Asn1Acn::Types::Type::ASN1Type::INTEGER:
    case Asn1Acn::Types::Type::ASN1Type::REAL:
    case Asn1Acn::Types::Type::ASN1Type::BOOLEAN:
    case Asn1Acn::Types::Type::ASN1Type::ENUMERATED:
        return QList<VariableRef>() << VariableRef(m_target);

    case Asn1Acn::Types::Type::ASN1Type::SEQUENCE:
        return sequenceListOfFields(dynamic_cast<const Asn1Acn::Types::Sequence *>(type));

    case Asn1Acn::Types::Type::ASN1Type::SEQUENCEOF:
        return sequenceOfListOfFields(dynamic_cast<const Asn1Acn::Types::SequenceOf *>(type));

    case Asn1Acn::Types::Type::ASN1Type::CHOICE:
        return choiceListOfFields(dynamic_cast<const Asn1Acn::Types::Choice *>(type));

    case Asn1Acn::Types::Type::ASN1Type::USERDEFINED: {
        const Asn1Acn::Types::UserdefinedType *t = dynamic_cast<const Asn1Acn::Types::UserdefinedType *>(type);
        return generateListOfFields(t->type());
    }

    default:
        QString message = QString("Not implemented %1").arg(type->typeEnum());
        throw TranslationException(std::move(message));
        // not implemented yet
        break;
    }
}

const Asn1Acn::TypeAssignment *Helper::findType(const QString &name)
{
    for (const std::unique_ptr<Asn1Acn::File> &file : m_asn1Model->data()) {
        const Asn1Acn::File::DefinitionsList &definitionList = file->definitionsList();
        for (const std::unique_ptr<Asn1Acn::Definitions> &definitions : definitionList) {
            const Asn1Acn::TypeAssignment *assignment = definitions->type(name);
            if (assignment != nullptr) {
                return assignment;
            }
        }
    }
    return nullptr;
}

QString Helper::sequenceAssignmentFromPromelaToC(const Asn1Acn::Types::Sequence *type)
{
    QString result;
    QList<QString> optionalFields;
    for (const std::unique_ptr<Asn1Acn::SequenceComponent> &component : type->components()) {
        SequenceComponentVisitor visitor(
                SequenceComponentVisitor::Direction::FROM_PROMELA_TO_C, m_asn1Model, m_target, m_source);
        component->accept(visitor);
        if (visitor.hasComponent()) {
            result.append(visitor.getContent());
            if (visitor.isOptional()) {
                optionalFields.append(visitor.getComponentName());
            }
        }
    }
    for (const QString &field : optionalFields) {
        result.append(QString("%1.exist.%3 = %2.exist.%3;\n").arg(m_target, m_source, field));
    }

    return result;
}

QString Helper::sequenceOfAssignmentFromPromelaToC(const Asn1Acn::Types::SequenceOf *type)
{
    SizeConstraintVisitor<IntegerValue> constraintVisitor;
    type->constraints().accept(constraintVisitor);

    Helper helper(m_asn1Model, m_target + ".arr[i]", m_source + ".data[i]");
    QString itemAssignment = addIndent(helper.createAssignmentTemplateFromPromelaToC(type->itemsType()));

    QString result = QString("int i = 0;\n"
                             "for(i = 0; i < %1; ++i)\n"
                             "{\n"
                             "%2"
                             "}\n")
                             .arg(constraintVisitor.getMaxSize())
                             .arg(itemAssignment);

    if (constraintVisitor.getMinSize() != constraintVisitor.getMaxSize()) {
        result.append(QString("%1.nCount = %2.length;\n").arg(m_target, m_source));
    }

    return result;
}

QString Helper::choiceAssignmentFromPromelaToC(const Asn1Acn::Types::Choice *type)
{
    QString result;
    int32_t index = 1;
    for (const std::unique_ptr<Asn1Acn::Types::ChoiceAlternative> &component : type->components()) {
        Helper helper(m_asn1Model, m_target + ".u." + component->name(), m_source + ".data." + component->name());
        QString assignment = addIndent(helper.createAssignmentTemplateFromPromelaToC(component->type()));
        QString prefix = index == 1 ? "" : "else ";
        result += QString("%1if(%3.selection == %4_%5_PRESENT)\n"
                          "{\n"
                          "%6"
                          "    %2.kind = %4_%5_PRESENT;\n"
                          "}\n")
                          .arg(prefix, m_target, m_source, type->identifier(), component->name(), assignment);
        // choice
        component->name();
        component->type();
        ++index;
    }

    return result;
}

QString Helper::sequenceAssignmentFromCToPromela(const Asn1Acn::Types::Sequence *type)
{
    QString result;
    QList<QString> optionalFields;
    for (const std::unique_ptr<Asn1Acn::SequenceComponent> &component : type->components()) {
        SequenceComponentVisitor visitor(
                SequenceComponentVisitor::Direction::FROM_C_TO_PROMELA, m_asn1Model, m_target, m_source);
        component->accept(visitor);
        if (visitor.hasComponent()) {
            result.append(visitor.getContent());
            if (visitor.isOptional()) {
                optionalFields.append(visitor.getComponentName());
            }
        }
    }
    for (const QString &field : optionalFields) {
        result.append(QString("%1.exist.%3 = %2.exist.%3;\n").arg(m_target, m_source, field));
    }

    return result;
}

QString Helper::sequenceOfAssignmentFromCToPromela(const Asn1Acn::Types::SequenceOf *type)
{
    SizeConstraintVisitor<IntegerValue> constraintVisitor;
    type->constraints().accept(constraintVisitor);

    Helper helper(m_asn1Model, m_target + ".data[i]", m_source + ".arr[i]");
    QString itemAssignment = addIndent(helper.createAssignmentTemplateFromCToPromela(type->itemsType()));

    QString result = QString("int i = 0;\n"
                             "for(i = 0; i < %1; ++i)\n"
                             "{\n"
                             "%2"
                             "}\n")
                             .arg(constraintVisitor.getMaxSize())
                             .arg(itemAssignment);

    if (constraintVisitor.getMinSize() != constraintVisitor.getMaxSize()) {
        result.append(QString("%1.length = %2.nCount;\n").arg(m_target, m_source));
    }

    return result;
}

QString Helper::choiceAssignmentFromCToPromela(const Asn1Acn::Types::Choice *type)
{
    QString result;
    int32_t index = 1;
    for (const std::unique_ptr<Asn1Acn::Types::ChoiceAlternative> &component : type->components()) {
        Helper helper(m_asn1Model, m_target + ".data." + component->name(), m_source + ".u." + component->name());
        QString assignment = addIndent(helper.createAssignmentTemplateFromCToPromela(component->type()));
        QString prefix = index == 1 ? "" : " else ";
        result += QString("%1if(%3.kind == %4_%5_PRESENT)\n"
                          "{\n"
                          "%6"
                          "    %2.selection = %4_%5_PRESENT;\n"
                          "}\n")
                          .arg(prefix, m_target, m_source, type->identifier(), component->name(), assignment);
        ++index;
    }

    return result;
}

QList<VariableRef> Helper::sequenceListOfFields(const Asn1Acn::Types::Sequence *type)
{
    QList<VariableRef> result;
    QList<QString> optionalFields;
    for (const std::unique_ptr<Asn1Acn::SequenceComponent> &component : type->components()) {
        SequenceComponentVisitor visitor(
                SequenceComponentVisitor::Direction::FROM_C_TO_PROMELA, m_asn1Model, component->name(), "");
        component->accept(visitor);
        if (visitor.hasComponent()) {
            QList<VariableRef> fieldsOfElement = visitor.takeListOfFields();

            while (!fieldsOfElement.empty()) {
                VariableRef field = fieldsOfElement.takeFirst();
                field.prependElement(m_target);
                result.append(std::move(field));
            }

            if (visitor.isOptional()) {
                optionalFields.append(visitor.getComponentName());
            }
        }
    }

    for (const QString &field : optionalFields) {
        VariableRef fieldRef = VariableRef(m_target);
        fieldRef.appendElement("exist");
        fieldRef.appendElement(field);
        result.append(std::move(fieldRef));
    }

    return result;
}

QList<VariableRef> Helper::sequenceOfListOfFields(const Asn1Acn::Types::SequenceOf *type)
{
    SizeConstraintVisitor<IntegerValue> constraintVisitor;
    type->constraints().accept(constraintVisitor);

    Helper helper(m_asn1Model, "data", "");
    QList<VariableRef> fieldsOfItem = helper.generateListOfFields(type->itemsType());

    QList<VariableRef> result;
    size_t itemCount = constraintVisitor.getMaxSize();

    for (size_t index = 0; index < itemCount; ++index) {
        for (const VariableRef &field : fieldsOfItem) {
            VariableRef ref = field;
            ref.addIndexExpression(std::make_unique<Expression>(Constant(index)));
            ref.prependElement(m_target);
            result.append(std::move(ref));
        }
    }

    if (constraintVisitor.getMinSize() != constraintVisitor.getMaxSize()) {
        VariableRef ref(m_target);
        ref.appendElement("length");
        result.append(std::move(ref));
    }

    return result;
}

QList<VariableRef> Helper::choiceListOfFields(const Asn1Acn::Types::Choice *type)
{
    QList<VariableRef> result;
    for (const std::unique_ptr<Asn1Acn::Types::ChoiceAlternative> &component : type->components()) {
        Helper helper(m_asn1Model, component->name(), "");
        QList<VariableRef> fieldsOfElement = helper.generateListOfFields(component->type());

        while (!fieldsOfElement.empty()) {
            VariableRef ref = fieldsOfElement.takeFirst();
            ref.prependElement("data");
            ref.prependElement(m_target);
            result.append(std::move(ref));
        }
    }

    VariableRef selectionRef(m_target);
    selectionRef.appendElement("selection");
    result.append(std::move(selectionRef));

    return result;
}

QString Helper::addIndent(const QString &text)
{
    QStringList lines = text.split('\n');
    QStringList indentedLines;
    std::transform(lines.begin(), lines.end(), std::back_inserter(indentedLines),
            [](const QString &l) { return QString("    ") + l + "\n"; });

    return std::accumulate(indentedLines.begin(), indentedLines.end(), QString(""));
}
}

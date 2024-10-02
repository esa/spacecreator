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

#include "ccodegenerator.h"

#include "asn1typehelper.h"

#include <conversion/common/escaper/escaper.h>
#include <conversion/common/translation/exceptions.h>
#include <promela/PromelaModel/ccode.h>
#include <promela/PromelaModel/proctypeelement.h>

using conversion::Escaper;
using conversion::translator::TranslationException;
using promela::model::CCode;
using promela::model::Expression;
using promela::model::InlineCall;
using promela::model::PrintfStatement;
using promela::model::StringConstant;
using promela::model::VariableRef;

namespace promela::translator {
CCodeGenerator::CCodeGenerator(const Asn1Acn::Asn1Model *asn1Model, const std::set<QString> &messageTypes)
{
    size_t iteratorCounter = 0; // to avoid duplicated names like 'int c_code_it_1 = 0;'
    Asn1TypeHelper helper(asn1Model, "$target$", "$source$", iteratorCounter);
    for (const QString &messageType : messageTypes) {
        m_templatesFromPromelaToC.insert(messageType, helper.createAssignmentTemplateFromPromelaToC(messageType));
        m_templatesFromCToPromela.insert(messageType, helper.createAssignmentTemplateFromCToPromela(messageType));
        m_printfTemplates.insert(messageType, helper.generatePrintfTemplate(messageType));
    }
}

void CCodeGenerator::generateConversionFromParameterToBuffer(promela::model::Sequence &sequence,
        const QString &parameterName, const QString &parameterType, const QString &temporaryVariableName,
        const QString &bufferVariableName)
{
    QList<InlineCall::Argument> assignInlineArguments;
    assignInlineArguments.append(VariableRef(temporaryVariableName));
    assignInlineArguments.append(VariableRef(parameterName));
    sequence.appendElement(InlineCall(QString("%1_assign_value").arg(Escaper::escapePromelaName(parameterType)),
            std::move(assignInlineArguments)));

    QString assignment =
            assignmentFromPromelaToC(parameterType, QString("%1_c_var").arg(Escaper::escapePromelaName(parameterName)),
                    QString("now.%1").arg(Escaper::escapePromelaName(temporaryVariableName)));

    QString code = QString("{\n"
                           "asn1Scc%1 %2_c_var;\n"
                           "BitStream %2_stream;\n"
                           "int %2_rc;\n"
                           "\n"
                           "%4"
                           "\n"
                           "BitStream_Init(&%2_stream,\n"
                           "    now.%3.data,\n"
                           "    asn1Scc%1_REQUIRED_BYTES_FOR_ENCODING);\n"
                           "asn1Scc%1_Encode(&%2_c_var,\n"
                           "    &%2_stream,\n"
                           "    &%2_rc,\n"
                           "    0);\n"
                           "}")
                           .arg(Escaper::escapePromelaName(parameterType), Escaper::escapePromelaName(parameterName),
                                   Escaper::escapePromelaName(bufferVariableName), assignment);

    sequence.appendElement(CCode(std::move(code)));
}

void CCodeGenerator::generateConversionFromBufferToParameter(
        promela::model::Sequence &sequence, const QString &parameterType, const QString &parameterName)
{
    QString assignment =
            assignmentFromCToPromela(parameterType, QString("now.%1").arg(Escaper::escapePromelaName(parameterName)),
                    Escaper::escapePromelaName(parameterName) + "_c_var");

    QString conversionCode = QString("{\n"
                                     "asn1Scc%1 %2_c_var;\n"
                                     "BitStream %2_stream;\n"
                                     "int %2_rc;"
                                     "BitStream_AttachBuffer(&%2_stream,\n"
                                     "    now.%2_message.data,\n"
                                     "    asn1Scc%1_REQUIRED_BYTES_FOR_ENCODING);\n"
                                     "asn1Scc%1_Decode(&%2_c_var,\n"
                                     "    &%2_stream,\n"
                                     "    &%2_rc);\n"
                                     "%3"
                                     "}")
                                     .arg(Escaper::escapePromelaName(parameterType),
                                             Escaper::escapePromelaName(parameterName), std::move(assignment));

    sequence.appendElement(CCode(std::move(conversionCode)));
}

QString CCodeGenerator::assignmentFromPromelaToC(const QString &typeName, const QString &target, const QString &source)
{
    if (!m_templatesFromPromelaToC.contains(typeName)) {
        auto message = QString("No assignment template from promela to c for type %1").arg(typeName);
        throw TranslationException(message);
    }

    QString tmplt = m_templatesFromPromelaToC.value(typeName);

    tmplt.replace("$target$", target);
    tmplt.replace("$source$", source);
    return tmplt;
}

QString CCodeGenerator::assignmentFromCToPromela(const QString &typeName, const QString &target, const QString &source)
{
    if (!m_templatesFromCToPromela.contains(typeName)) {
        auto message = QString("No assignment template from c to promela for type %1").arg(typeName);
        throw TranslationException(message);
    }

    QString tmplt = m_templatesFromCToPromela.value(typeName);
    tmplt.replace("$target$", target);
    tmplt.replace("$source$", source);
    return tmplt;
}

QList<PrintfStatement> CCodeGenerator::printfStatements(
        const QString &typeName, const QString &variable, const QString &additionalMessage)
{
    if (!m_printfTemplates.contains(typeName)) {
        auto message = QString("No assignment template from c to promela for type %1").arg(typeName);
        throw TranslationException(message);
    }

    const QList<Asn1TypeHelper::PrintfTemplate> &tmpltList = m_printfTemplates.value(typeName);

    QList<PrintfStatement> result;

    for (const Asn1TypeHelper::PrintfTemplate &tmplt : tmpltList) {
        QList<Expression> arguments;

        QString fmt = tmplt.m_fmt;
        if (result.empty()) {
            // this is the first printf -> prepend additional message
            fmt.prepend(additionalMessage);
        }
        if (result.size() == tmpltList.size() - 1) {
            // this is the last printf -> append new line
            fmt.append("\\n");
        }

        arguments.append(Expression(StringConstant(std::move(fmt))));

        for (const VariableRef &refTemplate : tmplt.m_args) {
            VariableRef ref = refTemplate;
            ref.changeFirstRef(variable);
            arguments.append(Expression(std::move(ref)));
        }

        result.append(PrintfStatement(std::move(arguments)));
    }

    return result;
}

}

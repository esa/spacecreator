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

#include "helper.h"

#include <conversion/common/translation/exceptions.h>

using conversion::translator::TranslationException;
using promela::model::Expression;
using promela::model::PrintfStatement;
using promela::model::StringConstant;
using promela::model::VariableRef;

namespace promela::translator {
CCodeGenerator::CCodeGenerator(const Asn1Acn::Asn1Model *asn1Model, const std::set<QString> &messageTypes)
{
    Helper helper(asn1Model, "$target$", "$source$");
    for (const QString &messageType : messageTypes) {
        m_templatesFromPromelaToC.insert(messageType, helper.createAssignmentTemplateFromPromelaToC(messageType));
        m_templatesFromCToPromela.insert(messageType, helper.createAssignmentTemplateFromCToPromela(messageType));
        m_printfTemplates.insert(messageType, helper.generatePrintfTemplate(messageType));
    }
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

    const QList<Helper::PrintfTemplate> &tmpltList = m_printfTemplates.value(typeName);

    QList<PrintfStatement> result;

    for (const Helper::PrintfTemplate &tmplt : tmpltList) {
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

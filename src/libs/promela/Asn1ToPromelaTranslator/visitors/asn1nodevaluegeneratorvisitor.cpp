/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
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

#include "asn1nodevaluegeneratorvisitor.h"

#include "asn1typevaluegeneratorvisitor.h"

#include <QDebug>
#include <QString>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/file.h>
#include <conversion/common/translation/exceptions.h>
#include <iostream>
#include <ivcore/ivfunction.h>

using Asn1Acn::Asn1Model;
using Asn1Acn::Definitions;
using Asn1Acn::File;
using Asn1Acn::Project;
using Asn1Acn::Root;
using Asn1Acn::TypeAssignment;
using Asn1Acn::ValueAssignment;
using Asn1Acn::Types::Type;
using conversion::translator::TranslationException;
using ivm::IVInterface;
using ivm::IVModel;
using promela::model::PromelaModel;

namespace promela::translator {

Asn1NodeValueGeneratorVisitor::Asn1NodeValueGeneratorVisitor(PromelaModel &promelaModel, const Asn1Model *asn1Model,
        const IVModel *ivModel, const std::optional<float> &delta)
    : m_promelaModel(promelaModel)
    , m_asn1Model(asn1Model)
    , m_ivModel(ivModel)
    , m_generateSubtypes(true)
    , m_delta(delta)
{
}

Asn1NodeValueGeneratorVisitor::Asn1NodeValueGeneratorVisitor(
        PromelaModel &promelaModel, QStringList typeNames, const std::optional<float> &delta)
    : m_promelaModel(promelaModel)
    , m_asn1Model(nullptr)
    , m_ivModel(nullptr)
    , m_typeNames(std::move(typeNames))
    , m_generateSubtypes(false)
    , m_delta(delta)
{
}

void Asn1NodeValueGeneratorVisitor::visit(const Definitions &defs)
{
    for (const std::unique_ptr<TypeAssignment> &type : defs.types()) {
        type->accept(*this);
    }
}

void Asn1NodeValueGeneratorVisitor::visit(const File &file)
{
    for (const std::unique_ptr<Definitions> &def : file.definitionsList()) {
        def->accept(*this);
    }
}

void Asn1NodeValueGeneratorVisitor::visit(const TypeAssignment &type)
{
    if (m_generateSubtypes) {
        const auto overridenType = findOverridenType(type.name());

        Asn1TypeValueGeneratorVisitor typeVisitor(m_promelaModel, type.name(), overridenType, m_delta);
        type.type()->accept(typeVisitor);
    } else if (m_typeNames.contains(type.name())) {
        Asn1TypeValueGeneratorVisitor typeVisitor(m_promelaModel, type.name(), nullptr, m_delta);
        type.type()->accept(typeVisitor);
    }
}

void Asn1NodeValueGeneratorVisitor::visit(const ValueAssignment &value)
{
    Q_UNUSED(value);
}

void Asn1NodeValueGeneratorVisitor::visit(const Project &project)
{
    Q_UNUSED(project);
}

void Asn1NodeValueGeneratorVisitor::visit(const Root &root)
{
    Q_UNUSED(root);
}

const Type *Asn1NodeValueGeneratorVisitor::findOverridenType(const QString &subtypeName) const
{
    const auto splittedSubtypeName = subtypeName.split('-', Qt::SkipEmptyParts);

    // Subtype name should be fit in a template <function_name>-<interface_name>-<argument_name>
    // So if splitting name by '-' doesn't return 3 strings then the type is not a subtype
    if (splittedSubtypeName.size() != 3) {
        return nullptr;
    }

    const auto &ivFunctionName = splittedSubtypeName.at(0);
    const auto ivFunction = m_ivModel->getFunction(ivFunctionName, Qt::CaseInsensitive);
    if (ivFunction == nullptr) {
        auto errorMessage =
                QString("Unable to find function %1 from subtype name %2").arg(ivFunctionName).arg(subtypeName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &ivInterfaceName = splittedSubtypeName.at(1);
    const auto ivInterface = m_ivModel->getIfaceByName(
            ivInterfaceName, IVInterface::InterfaceType::Required, ivFunction, Qt::CaseInsensitive);
    if (ivInterface == nullptr) {
        auto errorMessage = QString("Unable to find required interface %1 in function %2 from subtype name %3")
                                    .arg(ivInterfaceName)
                                    .arg(ivFunctionName)
                                    .arg(subtypeName);
        throw TranslationException(std::move(errorMessage));
    }
    const auto ivParameters = ivInterface->params();

    const auto &ivParameterName = splittedSubtypeName.at(2);
    const auto foundIvParameter = std::find_if(ivParameters.begin(), ivParameters.end(), [&](const auto &parameter) {
        return QString::compare(parameter.name(), ivParameterName, Qt::CaseInsensitive) == 0;
    });
    if (foundIvParameter == ivParameters.end()) {
        auto errorMessage =
                QString("Unable to find parameter %1 in required interface %2 in function %3 from subtype name %4")
                        .arg(ivParameterName)
                        .arg(ivInterfaceName)
                        .arg(ivFunctionName)
                        .arg(subtypeName);
        throw TranslationException(std::move(errorMessage));
    }

    const auto &ivParameterTypeName = foundIvParameter->paramTypeName();

    for (const auto &asn1File : m_asn1Model->data()) {
        const auto foundType = asn1File->typeFromName(ivParameterTypeName);
        if (foundType != nullptr) {
            qInfo() << "Type" << subtypeName << "is used to subtype type" << foundType->identifier();
            return foundType;
        }
    }

    auto errorMessage = QString(
            "Unable to find ASN.1 type %1 of parameter %2 in required interface %3 in function %4 from subtype name %5")
                                .arg(ivParameterTypeName)
                                .arg(ivParameterName)
                                .arg(ivInterfaceName)
                                .arg(ivFunctionName)
                                .arg(subtypeName);
    throw TranslationException(std::move(errorMessage));

    return nullptr;
}

}

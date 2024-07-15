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
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVModel;
using promela::model::PromelaModel;
using shared::InterfaceParameter;

namespace promela::translator {

Asn1NodeValueGeneratorVisitor Asn1NodeValueGeneratorVisitor::generatorForEnvironmentSubtypes(
        model::PromelaModel &promelaModel, const Asn1Acn::Asn1Model *asn1Model, const ivm::IVModel *ivModel,
        const std::optional<float> &delta)
{
    return Asn1NodeValueGeneratorVisitor(promelaModel, asn1Model, ivModel, delta);
}

Asn1NodeValueGeneratorVisitor Asn1NodeValueGeneratorVisitor::generatorForModelTypes(
        model::PromelaModel &promelaModel, const QStringList &typeNames, const std::optional<float> &delta)
{
    return Asn1NodeValueGeneratorVisitor(promelaModel, std::move(typeNames), delta);
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
    if (m_isEnvironmentSubtype) {
        const auto originalParameterType = findOriginalIvParameterType(type.name());

        Asn1TypeValueGeneratorVisitor typeVisitor(m_promelaModel, type.name(), originalParameterType, m_delta);
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

Asn1NodeValueGeneratorVisitor::Asn1NodeValueGeneratorVisitor(PromelaModel &promelaModel, const Asn1Model *asn1Model,
        const IVModel *ivModel, const std::optional<float> &delta)
    : m_promelaModel(promelaModel)
    , m_asn1Model(asn1Model)
    , m_ivModel(ivModel)
    , m_isEnvironmentSubtype(true)
    , m_delta(delta)
{
    m_environmentSubtypeToIvTypeMapping = prepareEnvironmentSubtypeToIvTypeMap();
}

Asn1NodeValueGeneratorVisitor::Asn1NodeValueGeneratorVisitor(
        PromelaModel &promelaModel, QStringList typeNames, const std::optional<float> &delta)
    : m_promelaModel(promelaModel)
    , m_asn1Model(nullptr)
    , m_ivModel(nullptr)
    , m_typeNames(std::move(typeNames))
    , m_isEnvironmentSubtype(false)
    , m_delta(delta)
{
}

const Type *Asn1NodeValueGeneratorVisitor::findOriginalIvParameterType(const QString &subtypeName) const
{
    for (auto iter = m_environmentSubtypeToIvTypeMapping.constBegin();
            iter != m_environmentSubtypeToIvTypeMapping.constEnd(); ++iter) {
        if (subtypeName.compare(iter.key(), Qt::CaseSensitivity::CaseInsensitive) == 0) {
            const QString &ivParameterTypeName = iter.value();
            for (const auto &asn1File : m_asn1Model->data()) {
                const auto foundType = asn1File->typeFromName(ivParameterTypeName);
                if (foundType != nullptr) {
                    return foundType;
                }
            }
        }
    }

    return nullptr;
}

QMap<QString, QString> Asn1NodeValueGeneratorVisitor::prepareEnvironmentSubtypeToIvTypeMap() const
{
    QMap<QString, QString> result;

    QVector<IVFunction *> functionList = m_ivModel->allObjectsByType<IVFunction>();

    for (const IVFunction *function : functionList) {
        QVector<IVInterface *> interfaceList = function->ris();

        for (const IVInterface *interface : interfaceList) {
            QVector<InterfaceParameter> parameterList = interface->params();

            for (const InterfaceParameter &parameter : parameterList) {
                QString name = QString("%1-%2-%3")
                                       .arg(function->title(), interface->title(), parameter.name())
                                       .replace("_", "-");
                result.insert(name, parameter.paramTypeName());
            }
        }
    }

    return result;
}

}

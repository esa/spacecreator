/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include "promelamodelvisitor.h"

#include "declarationvisitor.h"
#include "proctypeelementvisitor.h"
#include "sequencevisitor.h"
#include "typealiasvisitor.h"
#include "utypevisitor.h"

#include <QVector>
#include <algorithm>

using promela::model::CDecl;
using promela::model::Declaration;
using promela::model::InitProctype;
using promela::model::InlineDef;
using promela::model::NamedMtype;
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Sequence;
using promela::model::TypeAlias;
using promela::model::Utype;
using promela::model::ValueDefinition;

namespace promela::exporter {

PromelaModelVisitor::PromelaModelVisitor(QTextStream &stream, QString baseIndent, QString sequenceIndent)
    : m_stream(stream)
    , m_baseIndent(std::move(baseIndent))
    , m_sequenceIndent(std::move(sequenceIndent))
{
}

void PromelaModelVisitor::visit(const PromelaModel &promelaModel)
{
    generateIncludes(promelaModel.getIncludes());
    if (promelaModel.hasCDecl()) {
        generateCDecl(promelaModel.getCDecl());
    }
    generateMtypes(promelaModel.getMtypeValues());
    generateNamedMtypes(promelaModel.getNamedMtypeValues());
    generateTypeAliases(promelaModel.getTypeAliases());
    generateValueDefinitions(promelaModel.getValueDefinitions());
    generateUtypes(promelaModel.getUtypes());
    generateDeclarations(promelaModel.getDeclarations());
    generateInlineDefs(promelaModel.getInlineDefs());
    generateProctypes(promelaModel.getProctypes());
    if (promelaModel.hasInit()) {
        generateInitProctype(promelaModel.getInit());
    }
    generateIncludes(promelaModel.getEpilogueIncludes());
}

void PromelaModelVisitor::generateCDecl(const CDecl &declaration)
{
    if (declaration.hasContent()) {
        m_stream << "c_decl {\n";
        const auto lines = declaration.getContent().split('\n');
        for (const auto &line : lines) {
            m_stream << m_baseIndent << line << "\n";
        }
        m_stream << "}\n";
    }
}

void PromelaModelVisitor::generateIncludes(const QList<QString> &includes)
{
    for (const QString &file : includes) {
        m_stream << "#include \"" << file << "\"\n";
    }
}

void PromelaModelVisitor::generateMtypes(const QSet<QString> &values)
{
    if (!values.isEmpty()) {
        m_stream << "mtype {\n";
        QVector<QString> mtype_values = QVector<QString>::fromList(values.values());
        std::sort(mtype_values.begin(), mtype_values.end());
        generateMtypeNames(mtype_values);
        m_stream << "}\n";
    }
}

void PromelaModelVisitor::generateNamedMtypes(const std::map<QString, NamedMtype> &values)
{
    for (auto iter = values.begin(); iter != values.end(); ++iter) {
        m_stream << "mtype :" << iter->first << " {\n";
        QVector<QString> mtype_values;
        mtype_values.reserve(iter->second.values().size());
        for (const auto &value : iter->second.values()) {
            mtype_values.append(value);
        }
        std::sort(mtype_values.begin(), mtype_values.end());
        generateMtypeNames(mtype_values);
        m_stream << "}\n";
    }
}

void PromelaModelVisitor::generateMtypeNames(const QVector<QString> &names)
{
    for (const QString &value : names) {
        m_stream << m_baseIndent << value << ",\n";
    }
}

void PromelaModelVisitor::generateTypeAliases(const QList<TypeAlias> &aliases)
{
    TypeAliasVisitor visitor(m_stream);
    std::for_each(aliases.begin(), aliases.end(), visitor);
}

void PromelaModelVisitor::generateValueDefinitions(const QList<ValueDefinition> &values)
{
    for (const ValueDefinition &value : values) {
        m_stream << "#define " << value.getName() << " " << value.getValue() << "\n";
    }
}

void PromelaModelVisitor::generateUtypes(const QList<Utype> &utypes)
{
    for (const Utype &utype : utypes) {
        UtypeVisitor visitor(m_stream, m_baseIndent);
        visitor.visit(utype);
    }
}

void PromelaModelVisitor::generateDeclarations(const QList<Declaration> &values)
{
    for (const Declaration &declaration : values) {
        DeclarationVisitor visitor(m_stream, "");
        visitor.visit(declaration);
    }
}

void PromelaModelVisitor::generateInlineDefs(const std::list<std::unique_ptr<InlineDef>> &inlines)
{
    for (const std::unique_ptr<InlineDef> &inlineDef : inlines) {
        m_stream << "inline " << inlineDef->getName();

        m_stream << "(";
        const QString allArguments = QStringList(inlineDef->getArguments()).join(", ");
        m_stream << allArguments;
        m_stream << ")\n";

        m_stream << "{\n";
        generateSequence(inlineDef->getSequence());
        m_stream << "}\n";
    }
}

void PromelaModelVisitor::generateProctypes(const std::list<std::unique_ptr<Proctype>> &proctypes)
{
    for (const std::unique_ptr<Proctype> &proctype : proctypes) {
        if (proctype->isActive()) {
            m_stream << "active ";
            if (proctype->getInstancesCount() > 1) {
                m_stream << "[" << proctype->getInstancesCount() << "] ";
            }
        }
        m_stream << "proctype " << proctype->getName() << "()";
        if (proctype->hasPriority()) {
            m_stream << " priority " << proctype->getPriority();
        }
        m_stream << "\n";
        m_stream << "{\n";
        generateSequence(proctype->getSequence());
        m_stream << "}\n";
    }
}
void PromelaModelVisitor::generateInitProctype(const InitProctype &init)
{
    m_stream << "init\n";
    m_stream << "{\n";
    generateSequence(init.getSequence());
    m_stream << "}\n";
}

void PromelaModelVisitor::generateSequence(const Sequence &sequence)
{
    if (sequence.getType() != Sequence::Type::NORMAL) {
        SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, m_baseIndent);
        m_stream << m_baseIndent << visitor.getSequencePrefix(sequence) << "{\n";
        visitor.visit(sequence);
        m_stream << m_baseIndent << "}\n";
    } else {
        SequenceVisitor visitor(m_stream, m_baseIndent, m_sequenceIndent, "");
        visitor.visit(sequence);
    }
}
}

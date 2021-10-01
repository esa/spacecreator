/****************************************************************************
**
** Copyright (C) 2018-2019 N7 Space sp. z o. o.
** Contact: http://n7space.com
**
** This file is part of ASN.1/ACN Fuzzer - Tool for generating test cases
** based on ASN.1/ACN models and simulating malformed or malicious data.
**
** Tool was developed under a programme and funded by
** European Space Agency.
**
** This Tool is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Tool is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

namespace Asn1Acn {

class AcnDefinedItemsAddingVisitor : public Types::TypeMutatingVisitor
{
public:
    AcnDefinedItemsAddingVisitor(AcnParameterPtrs acnParameters)
        : m_acnParameters(std::move(acnParameters))
    {}

    AcnDefinedItemsAddingVisitor(AcnArgumentPtrs acnArguments)
        : m_acnArguments(std::move(acnArguments))
    {}

    AcnDefinedItemsAddingVisitor(AcnSequenceComponentPtr acnComponent)
        : m_acnComponent(std::move(acnComponent))
    {}

    ~AcnDefinedItemsAddingVisitor() override {}

    void visit(Types::Boolean &type) override { Q_UNUSED(type); }
    void visit(Types::Null &type) override { Q_UNUSED(type); }
    void visit(Types::BitString &type) override { Q_UNUSED(type); }
    void visit(Types::OctetString &type) override { Q_UNUSED(type); }
    void visit(Types::IA5String &type) override { Q_UNUSED(type); }
    void visit(Types::NumericString &type) override { Q_UNUSED(type); }
    void visit(Types::Enumerated &type) override { Q_UNUSED(type); }
    void visit(Types::Choice &type) override
    {
        for (auto &param : m_acnParameters)
            type.addParameter(std::move(param));
    }

    void visit(Types::Sequence &type) override
    {
        for (auto &param : m_acnParameters)
            type.addParameter(std::move(param));

        if (m_acnComponent)
            type.addComponent(std::move(m_acnComponent));
    }

    void visit(Types::SequenceOf &type) override { Q_UNUSED(type); }
    void visit(Types::Real &type) override { Q_UNUSED(type); }
    void visit(Types::LabelType &type) override { Q_UNUSED(type); }
    void visit(Types::Integer &type) override { Q_UNUSED(type); }
    void visit(Types::UserdefinedType &type) override
    {
        for (auto &arg : m_acnArguments)
            type.addArgument(std::move(arg));
    }

private:
    AcnParameterPtrs m_acnParameters;
    AcnArgumentPtrs m_acnArguments;
    AcnSequenceComponentPtr m_acnComponent;
};

} // namespace Asn1Acn

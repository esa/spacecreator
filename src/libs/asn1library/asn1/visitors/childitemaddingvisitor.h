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

class ChildItemAddingVisitor : public Types::TypeMutatingVisitor
{
public:
    ChildItemAddingVisitor(const QXmlStreamAttributes &attributes,
                           const QString &currentFile,
                           std::unique_ptr<Types::Type> childType = nullptr)
        : m_attributes(attributes)
        , m_currentFile(currentFile)
        , m_childType(std::move(childType))
    {}

    ~ChildItemAddingVisitor() override {}

    void visit(Types::Boolean &type) override { Q_UNUSED(type); }
    void visit(Types::Null &type) override { Q_UNUSED(type); }
    void visit(Types::BitString &type) override { Q_UNUSED(type); }
    void visit(Types::OctetString &type) override { Q_UNUSED(type); }
    void visit(Types::IA5String &type) override { Q_UNUSED(type); }
    void visit(Types::NumericString &type) override { Q_UNUSED(type); }
    void visit(Types::Enumerated &type) override { Q_UNUSED(type); }

    void visit(Types::Choice &type) override
    {
        const auto name = readStringAttribute(QStringLiteral("Name"));
        const auto presentWhenName = readStringAttribute(QStringLiteral("PresentWhenName"));
        const auto adaNameAttribute = readStringAttribute(QStringLiteral("AdaName"));
        const auto cNameAttribute = readStringAttribute(QStringLiteral("CName"));
        const auto presentWhen = readStringAttribute(QStringLiteral("present-when"));

        SourceLocation location(m_currentFile,
                                      readIntegerAttribute(QStringLiteral("Line")),
                                      readIntegerAttribute(QStringLiteral("CharPositionInLine")));

        type.addComponent(std::make_unique<Types::ChoiceAlternative>(name,
                                                                           presentWhenName,
                                                                           adaNameAttribute,
                                                                           cNameAttribute,
                                                                           presentWhen,
                                                                           location,
                                                                           std::move(m_childType)));
    }

    void visit(Types::Sequence &type) override
    {
        const auto name = readStringAttribute(QStringLiteral("Name"));
        const auto cName = readStringAttribute(QStringLiteral("CName"));
        const auto presentWhen = readStringAttribute(QStringLiteral("present-when"));
        const auto optional = readStringAttribute(QStringLiteral("Optional")).toLower();
        SourceLocation location(m_currentFile,
                                      readIntegerAttribute(QStringLiteral("Line")),
                                      readIntegerAttribute(QStringLiteral("CharPositionInLine")));

        type.addComponent(std::make_unique<AsnSequenceComponent>(name,
                                                                       cName,
                                                                       optional == "true",
                                                                       presentWhen,
                                                                       location,
                                                                       std::move(m_childType)));
    }

    void visit(Types::SequenceOf &type) override
    {
        type.setItemsType(std::move(m_childType));
    }

    void visit(Types::Real &type) override { Q_UNUSED(type); }
    void visit(Types::LabelType &type) override { Q_UNUSED(type); }
    void visit(Types::Integer &type) override { Q_UNUSED(type); }
    void visit(Types::UserdefinedType &type) override
    {
        type.setType(std::move(m_childType));
    }

protected:
    int readIntegerAttribute(const QString &key) const { return m_attributes.value(key).toInt(); }
    QString readStringAttribute(const QString &key) const
    {
        return m_attributes.value(key).toString();
    }
    const QString &currentFile() const { return m_currentFile; }

private:
    const QXmlStreamAttributes &m_attributes;
    const QString &m_currentFile;
    std::unique_ptr<Types::Type> m_childType;
};

} // namespace Asn1Acn

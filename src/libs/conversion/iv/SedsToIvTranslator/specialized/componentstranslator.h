#pragma once

#include <QVector>
#include <ivcore/ivinterface.h>

namespace Asn1Acn {
class File;
} // namespace Asn1Acn

namespace ivm {
class IVFunction;
class InterfaceParameter;
} // namespace ivm

namespace seds::model {
class CommandArgument;
class Component;
class GenericTypeMap;
class Interface;
class InterfaceCommand;
class InterfaceDeclaration;
class Package;
enum class CommandArgumentMode;
enum class InterfaceCommandMode;
} // namespace seds::model

namespace conversion::iv::translator {

/**
 * @brief   Translator from SEDS components to InterfaceView functions
 */
class ComponentsTranslator final
{
public:
    /**
     * @brief   Constructor
     *
     * @param   package     Package with components to translate
     * @param   asn1File    ASN.1 file where types of the packed argument will be saved
     */
    ComponentsTranslator(const seds::model::Package &package, Asn1Acn::File &asn1File);
    /**
     * @brief   Deleted copy constructor
     */
    ComponentsTranslator(const ComponentsTranslator &) = delete;
    /**
     * @brief   Deleted move constructor
     */
    ComponentsTranslator(ComponentsTranslator &&) = delete;
    /**
     * @brief   Deleted copy assignment operator
     */
    ComponentsTranslator &operator=(const ComponentsTranslator &) = delete;
    /**
     * @brief   Deleted move assignment operator
     */
    ComponentsTranslator &operator=(ComponentsTranslator &&) = delete;

public:
    auto translateComponents() -> QVector<ivm::IVFunction *>;

private:
    auto translateComponent(const seds::model::Component &component) const -> ivm::IVFunction *;
    auto translateInterface(const seds::model::Interface &interface, const seds::model::Component &component,
            ivm::IVInterface::InterfaceType interfaceType, ivm::IVFunction *ivFunction) const -> void;
    auto translateInterfaceCommand(const seds::model::InterfaceCommand &command,
            const seds::model::Interface &interface, ivm::IVInterface::InterfaceType interfaceType,
            ivm::IVFunction *ivFunction) const -> void;

    auto convertInterfaceCommandMode(seds::model::InterfaceCommandMode commandMode) const
            -> ivm::IVInterface::OperationKind;
    auto convertCommandArgumentMode(seds::model::CommandArgumentMode argumentMode) const
            -> ivm::InterfaceParameter::Direction;

private:
    auto findInterfaceDeclaration(const QString &interfaceTypeName, const seds::model::Component &component) const
            -> const seds::model::InterfaceDeclaration &;

private:
    const seds::model::Package &m_package;
    Asn1Acn::File &m_asn1File;
};

} // namespace conversion::iv::translator

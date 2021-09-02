#include "specialized/componentstranslator.h"

#include <QDebug>
#include <conversion/common/translation/exceptions.h>
#include <ivcore/ivcommonprops.h>
#include <ivcore/ivfunction.h>
#include <seds/SedsModel/interfaces/commandargumentmode.h>
#include <seds/SedsModel/package/package.h>

using conversion::translator::MissingGenericTypeMappingException;
using conversion::translator::TranslationException;
using conversion::translator::UndeclaredInterfaceException;
using conversion::translator::UnhandledValueException;
using conversion::translator::UnsupportedValueException;

namespace conversion::iv::translator {

ComponentsTranslator::ComponentsTranslator(const seds::model::Package &package, Asn1Acn::File &asn1File)
    : m_package(package)
    , m_asn1File(asn1File)
{
}

QVector<ivm::IVFunction *> ComponentsTranslator::translateComponents()
{
    QVector<ivm::IVFunction *> components;

    // Each SEDS component translates to a IV function
    for (const auto &component : m_package.components()) {
        components.append(translateComponent(component));
    }

    return components;
}

ivm::IVFunction *ComponentsTranslator::translateComponent(const seds::model::Component &component) const
{
    auto *ivFunction = new ivm::IVFunction();
    ivFunction->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::name), component.nameStr());

    for (const auto &providedInterface : component.providedInterfaces()) {
        translateInterface(providedInterface, component, ivm::IVInterface::InterfaceType::Provided, ivFunction);
    }

    for (const auto &requiredInterface : component.requiredInterfaces()) {
        translateInterface(requiredInterface, component, ivm::IVInterface::InterfaceType::Required, ivFunction);
    }

    return ivFunction;
}

void ComponentsTranslator::translateInterface(const seds::model::Interface &interface,
        const seds::model::Component &component, ivm::IVInterface::InterfaceType interfaceType,
        ivm::IVFunction *ivFunction) const
{
    const auto &interfaceTypeName = interface.type().nameStr();
    const auto &interfaceDeclaration = findInterfaceDeclaration(interfaceTypeName, component);

    for (const auto &command : interfaceDeclaration.commands()) {
        translateInterfaceCommand(command, interface, interfaceType, ivFunction);
    }
}

void ComponentsTranslator::translateInterfaceCommand(const seds::model::InterfaceCommand &command,
        const seds::model::Interface &interface, ivm::IVInterface::InterfaceType interfaceType,
        ivm::IVFunction *ivFunction) const
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = ivFunction;
    creationInfo.type = interfaceType;
    creationInfo.name = command.nameStr();
    creationInfo.kind = convertInterfaceCommandMode(command.mode());

    auto *ivInterface = ivm::IVInterface::createIface(creationInfo);

    for (const auto &argument : command.arguments()) {
        qDebug() << argument.nameStr();
    }

    ivFunction->addChild(ivInterface);
}

ivm::IVInterface::OperationKind ComponentsTranslator::convertInterfaceCommandMode(
        seds::model::InterfaceCommandMode commandMode) const
{
    switch (commandMode) {
    case seds::model::InterfaceCommandMode::Sync:
        return ivm::IVInterface::OperationKind::Protected;
    case seds::model::InterfaceCommandMode::Async:
        return ivm::IVInterface::OperationKind::Sporadic;
    default:
        throw UnhandledValueException("InterfaceCommandMode");
        break;
    }
}

ivm::InterfaceParameter::Direction ComponentsTranslator::convertCommandArgumentMode(
        seds::model::CommandArgumentMode argumentMode) const
{
    switch (argumentMode) {
    case seds::model::CommandArgumentMode::In:
        return ivm::InterfaceParameter::Direction::IN;
    case seds::model::CommandArgumentMode::Out:
        return ivm::InterfaceParameter::Direction::OUT;
    case seds::model::CommandArgumentMode::InOut:
    case seds::model::CommandArgumentMode::Notify:
        throw UnsupportedValueException("InterfaceCommandMode");
        break;
    default:
        throw UnhandledValueException("InterfaceCommandMode");
        break;
    }
}

const seds::model::InterfaceDeclaration &ComponentsTranslator::findInterfaceDeclaration(
        const QString &interfaceTypeName, const seds::model::Component &component) const
{
    const auto &componentInterfacesDeclarations = component.declaredInterfaces();
    auto interfaceDeclaration =
            std::find_if(componentInterfacesDeclarations.begin(), componentInterfacesDeclarations.end(),
                    [&interfaceTypeName](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                        return interfaceDeclaration.nameStr() == interfaceTypeName;
                    });
    if (interfaceDeclaration != componentInterfacesDeclarations.end()) {
        return *interfaceDeclaration;
    }

    const auto &packageInterfacesDeclarations = m_package.declaredInterfaces();
    interfaceDeclaration = std::find_if(packageInterfacesDeclarations.begin(), packageInterfacesDeclarations.end(),
            [&interfaceTypeName](const seds::model::InterfaceDeclaration &interfaceDeclaration) {
                return interfaceDeclaration.nameStr() == interfaceTypeName;
            });
    if (interfaceDeclaration != packageInterfacesDeclarations.end()) {
        return *interfaceDeclaration;
    }

    throw UndeclaredInterfaceException(interfaceTypeName);
}

} // namespace conversion::iv::translator

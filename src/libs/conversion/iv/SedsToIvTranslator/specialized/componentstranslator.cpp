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
        const seds::model::Component &component, const ivm::IVInterface::InterfaceType interfaceType,
        ivm::IVFunction *ivFunction) const
{
    const auto &interfaceTypeName = interface.type().nameStr();
    const auto &interfaceDeclaration = findInterfaceDeclaration(interfaceTypeName, component);

    for (const auto &command : interfaceDeclaration.commands()) {
        switch (command.mode()) {
        case seds::model::InterfaceCommandMode::Sync:
            throw TranslationException("Sync commands not implemented");
            break;
        case seds::model::InterfaceCommandMode::Async:
            translateAsyncInterfaceCommand(command, interface, interfaceType, ivFunction);
            break;
        default:
            throw UnhandledValueException("InterfaceCommandMode");
            break;
        }
    }
}

void ComponentsTranslator::translateAsyncInterfaceCommand(const seds::model::InterfaceCommand &command,
        const seds::model::Interface &interface, ivm::IVInterface::InterfaceType interfaceType,
        ivm::IVFunction *ivFunction) const
{
    switch (command.argumentsCombination()) {
    case seds::model::ArgumentsCombination::InOnly:
        throw TranslationException("Interface command with in arguments only not implemented");
        break;
    case seds::model::ArgumentsCombination::OutOnly:
        throw TranslationException("Interface command with out arguments only not implemented");
        break;
    case seds::model::ArgumentsCombination::InAndNotify:
        throw TranslationException("Interface command with in and notify arguments not implemented");
        break;
    case seds::model::ArgumentsCombination::NoArgs:
    case seds::model::ArgumentsCombination::NotifyOnly:
    case seds::model::ArgumentsCombination::InAndOut:
    case seds::model::ArgumentsCombination::OutAndNotify:
    case seds::model::ArgumentsCombination::All: {
        const auto message =
                QString("Interface command arguments combination '%1' is not supported for TASTE InterfaceView")
                        .arg(argumentsCombinationToString(command.argumentsCombination()));
        throw TranslationException(message);
    } break;
    default:
        throw UnhandledValueException("ArgumentsCombination");
        break;
    }
}

void ComponentsTranslator::translateInterfaceCommand(const seds::model::InterfaceCommand &command,
        const seds::model::Interface &interface, ivm::IVInterface::InterfaceType interfaceType,
        ivm::IVInterface::OperationKind operationKind, ivm::IVFunction *ivFunction) const
{
    ivm::IVInterface::CreationInfo creationInfo;
    creationInfo.function = ivFunction;
    creationInfo.type = interfaceType;
    creationInfo.name = command.nameStr();
    creationInfo.kind = operationKind;

    auto *ivInterface = ivm::IVInterface::createIface(creationInfo);
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

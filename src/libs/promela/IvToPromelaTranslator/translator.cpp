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

#include "translator.h"

#include <QDebug>
#include <conversion/common/escaper/escaper.h>
#include <ivcore/ivconnection.h>
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <shared/exportableproperty.h>

using conversion::Escaper;
using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::translator::TranslationException;
using ivm::IVConnection;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVInterfaceRequired;
using ivm::IVModel;
using promela::model::Assignment;
using promela::model::BasicType;
using promela::model::ChannelInit;
using promela::model::ChannelRecv;
using promela::model::ChannelSend;
using promela::model::Constant;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::DoLoop;
using promela::model::Expression;
using promela::model::InitProctype;
using promela::model::InlineCall;
using promela::model::InlineDef;
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
using promela::model::Sequence;
using promela::model::Utype;
using promela::model::UtypeRef;
using promela::model::VariableRef;
using shared::ExportableProperty;
using shared::InterfaceParameter;
using shared::VEObject;

namespace promela::translator {
std::vector<std::unique_ptr<Model>> IvToPromelaTranslator::translateModels(
        std::vector<Model *> sourceModels, const Options &options) const
{
    Q_UNUSED(options);

    std::unique_ptr<PromelaModel> promelaModel = std::make_unique<PromelaModel>();

    const auto *ivModel = getModel<IVModel>(sourceModels);

    QVector<IVFunction *> ivFunctionList = ivModel->allObjectsByType<IVFunction>();

    QList<QString> functionNames;

    promelaModel->addInclude("dataview.pml");

    promelaModel->addDeclaration(Declaration(DataType(BasicType::INT), "inited"));

    for (IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();
        functionNames.append(functionName);
        promelaModel->addInclude(QString("%1.pml").arg(functionName.toLower()));

        createPromelaObjectsForFunction(promelaModel.get(), ivModel, ivFunction, functionName);
    }

    Utype systemState("system_state");

    for (const QString &functionName : functionNames) {
        QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(functionName));
        QString fieldName = functionName;
        systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
    }

    promelaModel->addUtype(systemState);

    promelaModel->addDeclaration(Declaration(DataType(UtypeRef("system_state")), "global_state"));

    promelaModel->setInit(generateInitProctype(functionNames));

    std::vector<std::unique_ptr<Model>> result;
    result.push_back(std::move(promelaModel));
    return result;
}

ModelType IvToPromelaTranslator::getSourceModelType() const
{
    return ModelType::InterfaceView;
}

ModelType IvToPromelaTranslator::getTargetModelType() const
{
    return ModelType::Promela;
}

std::set<ModelType> IvToPromelaTranslator::getDependencies() const
{
    return std::set<ModelType> { ModelType::InterfaceView };
}

InitProctype IvToPromelaTranslator::generateInitProctype(const QList<QString> &functionNames) const
{
    Sequence sequence;

    for (const QString &functionName : functionNames) {
        QString initFn = QString("%1_0_init").arg(Escaper::escapePromelaIV(functionName));
        std::unique_ptr<ProctypeElement> initCall = std::make_unique<ProctypeElement>(InlineCall(initFn));
        sequence.appendElement(std::move(initCall));
    }

    std::unique_ptr<ProctypeElement> setInited =
            std::make_unique<ProctypeElement>(Assignment(VariableRef("inited"), Expression(Constant(1))));
    sequence.appendElement(std::move(setInited));
    return InitProctype(std::move(sequence));
}

std::unique_ptr<Proctype> IvToPromelaTranslator::generateProctype(PromelaModel *promelaModel,
        const QString &functionName, const QString &interfaceName, const QString &parameterName,
        const QString &parameterType, size_t queueSize, size_t priority) const
{
    QString channelName = constructChannelName(functionName, interfaceName);
    QList<ChannelInit::Type> channelType;
    channelType.append(
            parameterType.isEmpty() ? ChannelInit::Type(BasicType::INT) : ChannelInit::Type(UtypeRef(parameterType)));
    ChannelInit channelInit(queueSize, std::move(channelType));
    Declaration declaration(DataType(BasicType::CHAN), channelName);
    declaration.setInit(channelInit);
    promelaModel->addDeclaration(declaration);

    Sequence sequence;
    std::unique_ptr<ProctypeElement> waitForInit = std::make_unique<ProctypeElement>(Expression(VariableRef("inited")));
    sequence.appendElement(std::move(waitForInit));

    std::unique_ptr<ProctypeElement> variableDecl = std::make_unique<ProctypeElement>(Declaration(
            parameterType.isEmpty() ? DataType(BasicType::INT) : DataType(UtypeRef(parameterName)), "var0"));
    sequence.appendElement(std::move(variableDecl));

    std::unique_ptr<Sequence> loopSequence = std::make_unique<Sequence>();

    QList<VariableRef> params;
    params.append(VariableRef("var0"));
    std::unique_ptr<ProctypeElement> channelReceive =
            std::make_unique<ProctypeElement>(ChannelRecv(VariableRef(channelName), params));

    loopSequence->appendElement(std::move(channelReceive));

    QString piName = QString("%1_0_PI_0_%2").arg(Escaper::escapePromelaIV(functionName)).arg(interfaceName);

    loopSequence->appendElement(std::make_unique<ProctypeElement>(InlineCall(piName)));

    DoLoop loop;

    loop.appendSequence(std::move(loopSequence));

    std::unique_ptr<ProctypeElement> loopElement = std::make_unique<ProctypeElement>(std::move(loop));

    sequence.appendElement(std::move(loopElement));

    QString proctypeName = QString("%1_%2").arg(functionName).arg(interfaceName);
    std::unique_ptr<Proctype> proctype = std::make_unique<Proctype>(proctypeName, std::move(sequence));

    proctype->setActive(true);
    proctype->setInstancesCount(1);
    proctype->setPriority(priority);

    return proctype;
}

std::unique_ptr<::promela::model::InlineDef> IvToPromelaTranslator::generateSendInline(const QString &functionName,
        const QString &interfaceName, const QString &parameterName, const QString &parameterType,
        const QString &sourceFunctionName, const QString &sourceInterfaceName) const
{
    Q_UNUSED(parameterName);
    Q_UNUSED(parameterType);
    QString inlineName =
            QString("%1_0_RI_0_%2").arg(Escaper::escapePromelaIV(sourceFunctionName)).arg(sourceInterfaceName);
    QString channelName = constructChannelName(functionName, interfaceName);

    Sequence sequence;

    std::unique_ptr<ProctypeElement> dummyVariableDef =
            std::make_unique<ProctypeElement>(Declaration(DataType(BasicType::INT), "dummy"));
    QList<VariableRef> params;
    params.append(VariableRef("dummy"));
    std::unique_ptr<ProctypeElement> channelSend =
            std::make_unique<ProctypeElement>(ChannelSend(VariableRef(channelName), params));

    sequence.appendElement(std::move(dummyVariableDef));
    sequence.appendElement(std::move(channelSend));

    return std::make_unique<InlineDef>(inlineName, std::move(sequence));
}

void IvToPromelaTranslator::createPromelaObjectsForFunction(
        PromelaModel *promelaModel, const IVModel *ivModel, IVFunction *ivFunction, const QString &functionName) const
{
    QVector<IVInterface *> providedInterfaceList = ivFunction->pis();
    for (IVInterface *providedInterface : providedInterfaceList) {

        if (providedInterface->kind() != IVInterface::OperationKind::Cyclic
                && providedInterface->kind() != IVInterface::OperationKind::Protected) {
            auto message =
                    QString("Unallowed interface kind in function %1, only sporatic and cyclic interfaces are allowed")
                            .arg(functionName);
            throw TranslationException(message);
        }

        IVConnection *connection = ivModel->getConnectionForIface(providedInterface->id());

        IVInterface *requiredInterface = connection->sourceInterface();
        const QString sourceInterfaceName = requiredInterface->property("name").toString();
        const QString sourceFunctionName = requiredInterface->function()->property("name").toString();

        const QString interfaceName = providedInterface->property("name").toString();
        size_t queueSize = getQueueSize(providedInterface);
        size_t priority = getPriority(providedInterface);
        QVector<InterfaceParameter> parameterList = providedInterface->params();
        if (parameterList.length() > 1) {
            // at most one parameter is allowed for sporadic interfaces
            throw TranslationException("Sporadic interface with more than one parameters is not allowed");
        }
        QString parameterName;
        QString parameterType;
        if (parameterList.length() == 1) {
            parameterName = parameterList.first().name();
            parameterType = parameterList.first().paramTypeName();
        }

        promelaModel->addInlineDef(generateSendInline(
                functionName, interfaceName, parameterName, parameterType, sourceFunctionName, sourceInterfaceName));

        promelaModel->addProctype(generateProctype(
                promelaModel, functionName, interfaceName, parameterName, parameterType, queueSize, priority));
    }
}

size_t IvToPromelaTranslator::getQueueSize(IVInterface *interface) const
{
    QVariant property = getInterfaceProperty(interface, "queue_size");
    if (property.isValid()) {
        return property.toULongLong();
    }
    return 1;
}

size_t IvToPromelaTranslator::getPriority(IVInterface *interface) const
{
    QVariant property = getInterfaceProperty(interface, "priority");
    if (property.isValid()) {
        return property.toULongLong();
    }
    return 1;
}

QVariant IvToPromelaTranslator::getInterfaceProperty(ivm::IVInterface *interface, const QString &name) const
{
    QVariantList attrs = interface->attributes();
    {
        for (const QVariant attr : attrs) {
            if (attr.canConvert<ExportableProperty>()) {
                ExportableProperty ep = attr.value<ExportableProperty>();
                if (ep.name() == name) {
                    return ep.value();
                }
            }
        }
    }

    return QVariant();
}

QString IvToPromelaTranslator::constructChannelName(const QString &functionName, const QString &interfaceName) const
{
    return QString("%1_%2_channel").arg(functionName).arg(interfaceName);
}
}

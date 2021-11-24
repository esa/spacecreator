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
#include <ivcore/ivfunction.h>
#include <ivcore/ivmodel.h>
#include <shared/exportableproperty.h>

using conversion::Escaper;
using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::translator::TranslationException;
using ivm::IVFunction;
using ivm::IVInterface;
using ivm::IVModel;
using promela::model::BasicType;
using promela::model::ChannelInit;
using promela::model::DataType;
using promela::model::Declaration;
using promela::model::Expression;
using promela::model::InitProctype;
using promela::model::Proctype;
using promela::model::ProctypeElement;
using promela::model::PromelaModel;
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
    Q_UNUSED(sourceModels);
    Q_UNUSED(options);

    std::unique_ptr<PromelaModel> promelaModel = std::make_unique<PromelaModel>();

    const auto *ivModel = getModel<IVModel>(sourceModels);

    QVector<IVFunction *> ivFunctionList = ivModel->allObjectsByType<IVFunction>();

    QList<QString> functionNames;
    for (IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();
        // qDebug() << "Found function " << functionName;
        functionNames.append(functionName);
    }

    promelaModel->addInclude("dataview.pml");

    promelaModel->addDeclaration(Declaration(DataType(BasicType::INT), "inited"));

    for (IVFunction *ivFunction : ivFunctionList) {
        const QString functionName = ivFunction->property("name").toString();
        QVector<IVInterface *> interfaceList = ivFunction->pis();
        for (IVInterface *interface : interfaceList) {
            const QString interfaceName = interface->property("name").toString();
            // qDebug() << "  Found interface " << interfaceName;
            size_t queueSize = getQueueSize(interface);
            size_t priority = getPriority(interface);
            // qDebug() << "    Queue size " << queueSize;
            // qDebug() << "    Priority  " << priority;
            QVector<InterfaceParameter> parameterList = interface->params();
            QString parameterName;
            QString parameterType;
            for (InterfaceParameter &parameter : parameterList) {
                parameterName = parameter.name();
                parameterType = parameter.paramTypeName();
                // qDebug() << "    Found parameter " << parameterName << " of type " << parameterType;
            }

            QString channelName = QString("%1_%2_channel").arg(functionName).arg(interfaceName);
            if (parameterType.isEmpty()) {
                QList<ChannelInit::Type> channelType;
                channelType.append(ChannelInit::Type(BasicType::INT));
                ChannelInit channelInit(queueSize, std::move(channelType));
                Declaration declaration(DataType(BasicType::CHAN), channelName);
                declaration.setInit(channelInit);
                promelaModel->addDeclaration(declaration);
            } else {
                QList<ChannelInit::Type> channelType;
                channelType.append(ChannelInit::Type(UtypeRef(parameterType)));
                ChannelInit channelInit(queueSize, std::move(channelType));
                Declaration declaration(DataType(BasicType::CHAN), channelName);
                declaration.setInit(channelInit);
                promelaModel->addDeclaration(declaration);
            }

            Proctype proctype(QString("%1_%2").arg(functionName).arg(interfaceName));

            proctype.setActive(1);
            proctype.setPriority(priority);

            ProctypeElement waitForInit(Expression(VariableRef("inited")));
            proctype.appendElement(waitForInit);

            promelaModel->addProctype(proctype);
        }
    }

    Utype systemState("system_state");

    for (const QString &functionName : functionNames) {
        QString dataType = QString("%1_Context").arg(Escaper::escapePromelaIV(functionName));
        QString fieldName = functionName;
        systemState.addField(Declaration(DataType(UtypeRef(dataType)), fieldName));
    }

    promelaModel->addUtype(systemState);
    InitProctype init;
    ProctypeElement waitForInit(Expression(VariableRef("inited")));
    init.appendElement(waitForInit);
    promelaModel->setInit(init);

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

size_t IvToPromelaTranslator::getQueueSize(IVInterface *interface) const
{
    QVariantList attrs = interface->attributes();
    {
        for (const QVariant attr : attrs) {
            if (attr.canConvert<ExportableProperty>()) {
                ExportableProperty ep = attr.value<ExportableProperty>();
                if (ep.name() == "queue_size") {
                    return ep.value().toULongLong();
                }
            }
        }
    }

    return 1;
}

size_t IvToPromelaTranslator::getPriority(IVInterface *interface) const
{
    QVariantList attrs = interface->attributes();
    {
        for (const QVariant attr : attrs) {
            if (attr.canConvert<ExportableProperty>()) {
                ExportableProperty ep = attr.value<ExportableProperty>();
                if (ep.name() == "priority") {
                    return ep.value().toULongLong();
                }
            }
        }
    }

    return 1;
}
}

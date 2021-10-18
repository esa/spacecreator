/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "implementationdelegate.h"

#include "commands/cmdentityattributeschange.h"
#include "commandsstackbase.h"
#include "dvfunction.h"
#include "dvpropertytemplateconfig.h"
#include "dvtreeviewmodel.h"

#include <QComboBox>

namespace dve {

ImplementationDelegate::ImplementationDelegate(shared::cmd::CommandsStackBase *commandsStack, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_commandsStack(commandsStack)
{
}

QWidget *ImplementationDelegate::createEditor(
        QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        auto comboBox = new QComboBox(parent);
        QObject::connect(comboBox, &QComboBox::currentTextChanged, this,
                [this, comboBox]() { const_cast<ImplementationDelegate *>(this)->commitData(comboBox); });

        dvm::DVFunction *fn = index.data(dve::DVTreeViewModel::DVObjectRole).value<dvm::DVFunction *>();
        if (fn) {
            comboBox->addItems(fn->availableImplementations());
        }

        comboBox->setFocusPolicy(Qt::StrongFocus);
        comboBox->setEnabled(index.flags().testFlag(Qt::ItemIsEnabled));
        return comboBox;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ImplementationDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.isValid()) {
        if (auto comboBox = qobject_cast<QComboBox *>(editor)) {
            dvm::DVFunction *fn = index.data(dve::DVTreeViewModel::DVObjectRole).value<dvm::DVFunction *>();
            m_updating = true;
            if (fn) {
                comboBox->clear();
                comboBox->addItems(fn->availableImplementations());
                comboBox->setCurrentText(fn->usedImplementation());
            } else {
                comboBox->setCurrentText(index.data(Qt::DisplayRole).toString());
            }
            m_updating = false;
        }
        return;
    }

    return QStyledItemDelegate::setEditorData(editor, index);
}

void ImplementationDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (m_updating) {
        return;
    }

    if (auto comboBox = qobject_cast<QComboBox *>(editor)) {
        QString implementation = comboBox->currentText();
        dvm::DVFunction *fn = index.data(dve::DVTreeViewModel::DVObjectRole).value<dvm::DVFunction *>();
        if (fn && fn->implementation() != implementation) {
            QString impltToken = dvm::meta::Props::token(dvm::meta::Props::Token::selected_implementation);
            if (!fn->hasEntityAttribute(impltToken, implementation)) {
                const QList<EntityAttribute> attributes = { EntityAttribute {
                        impltToken, implementation, EntityAttribute::Type::Attribute } };
                auto attributesCmd = new shared::cmd::CmdEntityAttributesChange(
                        dvm::DVPropertyTemplateConfig::instance(), fn, attributes);
                m_commandsStack->push(attributesCmd);
            }
        }
        model->setData(index, implementation);
        return;
    }

    QStyledItemDelegate::setModelData(editor, model, index);
}

} // namespace shared

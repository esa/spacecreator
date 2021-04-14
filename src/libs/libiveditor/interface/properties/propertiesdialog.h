/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "commandsstack.h"

#include <QDialog>
#include <QPointer>
#include <QSharedPointer>

namespace Ui {
class PropertiesDialog;
}

namespace Asn1Acn {
class File;
}

namespace ivm {
class IVObject;
class PropertyTemplateConfig;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class PropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertiesDialog(ivm::PropertyTemplateConfig *dynPropConfig, ivm::IVObject *obj,
            const QSharedPointer<Asn1Acn::File> &dataTypes, cmd::CommandsStack *commandsStack,
            QWidget *parent = nullptr);
    ~PropertiesDialog() override;

public Q_SLOTS:
    void done(int r) override;

private:
    QString objectTypeName() const;
    void initTabs();
    void initConnectionGroup();
    void initAttributesView();
    void initContextParams();
    void initIfaceParams();
    void initCommentView();

private:
    Ui::PropertiesDialog *ui;
    ivm::IVObject *m_dataObject { nullptr };
    ivm::PropertyTemplateConfig *m_dynPropConfig { nullptr };
    cmd::CommandsStack::Macro *m_cmdMacro { nullptr };
    QSharedPointer<Asn1Acn::File> m_dataTypes;
    QPointer<cmd::CommandsStack> m_commandsStack;
};

}

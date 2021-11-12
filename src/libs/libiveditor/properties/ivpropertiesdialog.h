/*
  Copyright (C) 2019-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivobject.h"
#include "propertiesdialog.h"

#include <QPointer>

namespace Asn1Acn {
class Asn1SystemChecks;
}

namespace ivm {
class AbstractSystemChecks;
class IVPropertyTemplateConfig;
}

namespace ive {
namespace cmd {
class CommandsStack;
}

class IVPropertiesDialog : public shared::PropertiesDialog
{
    Q_OBJECT

public:
    explicit IVPropertiesDialog(ivm::IVPropertyTemplateConfig *dynPropConfig, ivm::IVObject *obj,
            ivm::AbstractSystemChecks *checks, Asn1Acn::Asn1SystemChecks *asn1Checks, cmd::CommandsStack *commandsStack,
            QWidget *parent = nullptr);
    ~IVPropertiesDialog() override;
    void init() override;

protected:
    QString objectTypeName() const override;
    ivm::IVObject *dataObject() const override;

private:
    void initConnectionGroup();
    void initAttributesView();
    void initContextParams();
    void initIfaceParams();
    void initCommentView();
    void initLanguageView();

    QPointer<ivm::AbstractSystemChecks> m_ivChecks;
    QPointer<Asn1Acn::Asn1SystemChecks> m_asn1Checks;
};

} // namespace ive

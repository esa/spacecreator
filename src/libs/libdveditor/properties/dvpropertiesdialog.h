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

#pragma once
#include "dvobject.h"
#include "propertiesdialog.h"

#include <QPointer>

namespace shared {
namespace ui {
class VEInteractiveObject;
class SCRequirementsWidget;
}
namespace cmd {
class CommandsStackBase;
class PropertyTemplateConfig;
} // namespace cmd
} // namespace shared
namespace requirement {
class RequirementsManager;
}

namespace reviews {
class Review;
class ReviewsManager;
class ReviewsModelBase;
}
namespace shared {
namespace ui {

class SCReviewsWidget;
}
class ComponentReviewsProxyModel;

}

namespace shared {
class RequirementsModel;
}
namespace Asn1Acn {
class Asn1SystemChecks;
}
namespace dvm {
class AbstractSystemChecks;
class DVModel;
}

namespace dve {

class DVPropertiesDialog : public shared::PropertiesDialog
{
public:
    DVPropertiesDialog(dvm::DVModel *model, shared::PropertyTemplateConfig *dynPropConfig,
            shared::ui::VEInteractiveObject *uiObj, dvm::AbstractSystemChecks *systemChecker,
            Asn1Acn::Asn1SystemChecks *asn1Checks, shared::cmd::CommandsStackBase *commandsStack,
            QWidget *parent = nullptr);

    void init() override;

public Q_SLOTS:
    void done(int r) override;

protected:
    QString objectTypeName() const override;
    dvm::DVObject *dataObject() const override;

private:
    void initAttributesView();
    void initRequirementsView();
    void initReviewView();

    void addReviewId(const reviews::Review &review);

    dvm::AbstractSystemChecks *m_dvChecker = nullptr;
    Asn1Acn::Asn1SystemChecks *m_asn1Checks = nullptr;
    shared::ui::SCRequirementsWidget *m_reqWidget = nullptr;
    requirement::RequirementsManager *m_reqManager = nullptr;
    shared::RequirementsModel *m_reqModel = nullptr;

    shared::ui::SCReviewsWidget *m_reviewWidget = nullptr;
    reviews::ReviewsManager *m_reviewsManager = nullptr;
    shared::ComponentReviewsProxyModel *m_reviewsModel = nullptr;
};

} // namespace dve

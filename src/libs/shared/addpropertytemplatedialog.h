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

#include <QDialog>
#include <memory>

class QCheckBox;

namespace Ui {
class AddPropertyTemplateDialog;
}

namespace shared {
class PropertyTemplate;
class PropertyTemplateConfig;

class AddPropertyTemplateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddPropertyTemplateDialog(
            PropertyTemplateConfig *config, const QStringList &prohibitedNames, QWidget *parent = nullptr);
    ~AddPropertyTemplateDialog() override;

    PropertyTemplate *attribute() const;

public Q_SLOTS:
    void accept() override;

private:
    bool validateName(bool showWarn);
    bool validateType();
    bool validateValuesList();
    bool validateScope();

    QStringList listValues() const;

private:
    Ui::AddPropertyTemplateDialog *ui;
    PropertyTemplateConfig *m_config;
    QStringList m_prohibitedNames;
    QColor m_nameColorDefault;
    std::unique_ptr<shared::PropertyTemplate> m_attr;
    QList<QPair<QCheckBox *, int>> m_scopeMappings;
};

} // namespace shared

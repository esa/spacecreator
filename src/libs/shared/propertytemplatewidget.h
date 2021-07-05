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

#include <QWidget>

class QPlainTextEdit;
class QPushButton;
class QLabel;
class QComboBox;

namespace shared {
class PropertyTemplateConfig;

class PropertyTemplateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyTemplateWidget(QWidget *parent = nullptr);
    ~PropertyTemplateWidget() override = default;

    void save();
    bool readConfig(const QString &from);
    void setTextColor(const QColor &color);
    bool hasError() const;
    void setPropertyTemplateConfigs(const QList<PropertyTemplateConfig *> &propTemplateConfigs);

Q_SIGNALS:
    void hasErrorChanged(bool hasError);
    void currentTemplateConfigChanged(const QString &title);

private Q_SLOTS:
    void updateErrorInfo();
    void createProperty();

private:
    PropertyTemplateConfig *currentTemplateConfig() const;

private:
    QList<PropertyTemplateConfig *> m_propTemplateConfigs;
    QPlainTextEdit *m_plainTextEdit { nullptr };
    QPushButton *m_btnNewProp { nullptr };
    QComboBox *m_templateConfigTypes { nullptr };
    QLabel *m_errorDisplay { nullptr };
    QStringList m_usedNames;
    bool m_error = false;
};

} // namespace shared

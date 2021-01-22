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

namespace aadl {
class PropertyTemplateConfig;
}
namespace Ui {
class PropertyTemplateWidget;
}

namespace ive {

class PropertyTemplateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyTemplateWidget(QWidget *parent = nullptr);
    ~PropertyTemplateWidget();

    void save();
    bool readConfig(const QString &from);
    void setTextColor(const QColor &color);

    bool hasError() const;

    static QString dynamicPropertiesFilePath();

Q_SIGNALS:
    void hasErrorChanged();

private Q_SLOTS:
    void updateErrorInfo();
    void on_btnNewProp_clicked();

private:
    Ui::PropertyTemplateWidget *ui;
    aadl::PropertyTemplateConfig *m_dynPropConfig { nullptr };
    QStringList m_usedNames;
    bool m_error = false;
};

}

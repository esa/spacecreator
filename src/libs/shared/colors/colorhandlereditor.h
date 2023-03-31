/*
  Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colorhandler.h"

#include <QWidget>

namespace Ui {
class ColorHandlerEditor;
}
class EntityAttribute;
namespace shared {
class PropertyTemplateConfig;
namespace ui {
class VEInteractiveObject;
}

class ColorHandlerEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ColorHandlerEditor(QWidget *parent = nullptr);
    ~ColorHandlerEditor();

    void setInteractiveObject(ui::VEInteractiveObject *iObject, PropertyTemplateConfig *propertiesConfig);
    ui::VEInteractiveObject *interactiveObject() const;

    void setColorHandler(const std::optional<ColorHandler> &h);
    std::optional<ColorHandler> colorHandler() const;

Q_SIGNALS:
    void colorHandlerValueChanged();
    void entityAttributeChanged(const EntityAttribute &entityAttr);

private Q_SLOTS:
    void on_sbWidth_valueChanged(qreal v);
    void on_btnColorStroke_colorChanged(const QColor &c);
    void on_comboBoxPenStyle_currentIndexChanged(int idx);
    void on_cbFillType_currentIndexChanged(int id);
    void on_btnColor_colorChanged(const QColor &c);
    void on_btnColorStop_colorChanged(const QColor &c);
    void on_btnTextColor_colorChanged(const QColor &c);
    void on_comboBoxTextAlignment_currentIndexChanged(int idx);
    void on_fontComboBox_currentFontChanged(const QFont &font);
    void on_doubleSpinBoxFontSize_valueChanged(qreal v);
    void on_comboBoxFontWeight_currentIndexChanged(int idx);
    void resetObjectAttributes();

private:
    void validateControls(PropertyTemplateConfig *propertiesConfig);

private:
    Ui::ColorHandlerEditor *ui;
    std::optional<ColorHandler> m_colorHandler;
    ui::VEInteractiveObject *m_iObj { nullptr };
};

} // namespace shared

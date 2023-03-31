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

#include "colorhandlereditor.h"

#include "colorhandler.h"
#include "common.h"
#include "propertytemplate.h"
#include "propertytemplateconfig.h"
#include "ui/veinteractiveobject.h"
#include "ui_colorhandlereditor.h"
#include "veobject.h"

#include <QMetaEnum>

namespace shared {

template<typename ENUM>
void fillComboBox(QComboBox *combobox)
{
    static const QMetaEnum metaEnum = QMetaEnum::fromType<ENUM>();
    for (int idx = 0; idx < metaEnum.keyCount(); ++idx) {
        combobox->addItem(QString::fromLatin1(metaEnum.key(idx)), metaEnum.value(idx));
    }
}

ColorHandlerEditor::ColorHandlerEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColorHandlerEditor)
{
    ui->setupUi(this);

    ui->cbFillType->addItem(tr("Color"), static_cast<int>(ColorHandler::FillType::Color));
    ui->cbFillType->addItem(tr("Gradient Vertical"), static_cast<int>(ColorHandler::FillType::GradientVertical));
    ui->cbFillType->addItem(tr("Gradient Horizontal"), static_cast<int>(ColorHandler::FillType::GradientHorizontal));

    fillComboBox<Qt::PenStyle>(ui->comboBoxPenStyle);
    fillComboBox<QFont::Weight>(ui->comboBoxFontWeight);
    fillComboBox<ColorHandler::Alignment>(ui->comboBoxTextAlignment);

    setEnabled(false);
}

ColorHandlerEditor::~ColorHandlerEditor()
{
    delete ui;
}

void ColorHandlerEditor::setInteractiveObject(
        ui::VEInteractiveObject *iObject, PropertyTemplateConfig *propertiesConfig)
{
    m_iObj = iObject;
    validateControls(propertiesConfig);
    setColorHandler(m_iObj ? m_iObj->colorHandler() : std::optional<ColorHandler>());
    if (m_colorHandler)
        m_colorHandler->detach(); // To not affect all entities use the same handler
}

ui::VEInteractiveObject *ColorHandlerEditor::interactiveObject() const
{
    return m_iObj;
}

void ColorHandlerEditor::setColorHandler(const std::optional<ColorHandler> &h)
{
    m_colorHandler = h;
    setEnabled(m_colorHandler.has_value());
    if (!m_colorHandler)
        return;

    QSignalBlocker(ui->comboBoxPenStyle);
    int idx = ui->comboBoxPenStyle->findData(static_cast<int>(m_colorHandler->penStyle()));
    ui->comboBoxPenStyle->setCurrentIndex(idx);

    QSignalBlocker(ui->sbWidth);
    ui->sbWidth->setValue(m_colorHandler->penWidth());

    QSignalBlocker(ui->btnColorStroke);
    ui->btnColorStroke->setColor(m_colorHandler->penColor());

    QSignalBlocker(ui->cbFillType);
    idx = ui->cbFillType->findData(static_cast<int>(m_colorHandler->fillType()));
    ui->cbFillType->setCurrentIndex(idx);

    QSignalBlocker(ui->btnColor);
    ui->btnColor->setColor(m_colorHandler->brushColor0());

    QSignalBlocker(ui->btnColorStop);
    ui->btnColorStop->setColor(m_colorHandler->brushColor1());

    QSignalBlocker(ui->fontComboBox);
    ui->fontComboBox->setCurrentFont(m_colorHandler->font());

    QSignalBlocker(ui->doubleSpinBoxFontSize);
    ui->doubleSpinBoxFontSize->setValue(m_colorHandler->font().pointSizeF());

    QSignalBlocker(ui->comboBoxFontWeight);
    idx = ui->comboBoxFontWeight->findData(static_cast<int>(m_colorHandler->font().weight()));
    ui->comboBoxFontWeight->setCurrentIndex(idx);

    QSignalBlocker(ui->btnTextColor);
    ui->btnTextColor->setColor(m_colorHandler->textColor());

    QSignalBlocker(ui->comboBoxTextAlignment);
    idx = ui->comboBoxTextAlignment->findData(static_cast<int>(m_colorHandler->textAlignment()));
    ui->comboBoxTextAlignment->setCurrentIndex(idx);
}

std::optional<ColorHandler> ColorHandlerEditor::colorHandler() const
{
    return m_colorHandler;
}

void ColorHandlerEditor::on_sbWidth_valueChanged(qreal v)
{
    if (m_colorHandler) {
        m_colorHandler->setPenWidth(v);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged(
            { EntityAttribute { StyleAttribute::kPenWidth, v, EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_btnColorStroke_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setPenColor(c);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged({ EntityAttribute {
            StyleAttribute::kPenColor, c.name(QColor::HexArgb), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_comboBoxPenStyle_currentIndexChanged(int idx)
{
    const Qt::PenStyle penStyle = ui->comboBoxPenStyle->itemData(idx).value<Qt::PenStyle>();
    if (m_colorHandler) {
        m_colorHandler->setPenStyle(penStyle);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged(
            { EntityAttribute { StyleAttribute::kPenStyle, typeName(penStyle), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_cbFillType_currentIndexChanged(int id)
{
    const ColorHandler::FillType ft = ColorHandler::FillType(ui->cbFillType->itemData(id).toInt());
    const bool stopColorShown = ft != ColorHandler::FillType::Color;

    ui->labelBrushColor1->setVisible(stopColorShown);
    ui->btnColorStop->setVisible(stopColorShown);

    if (m_colorHandler) {
        m_colorHandler->setFillType(ft);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged(
            { EntityAttribute { StyleAttribute::kBrushFillType, typeName(ft), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_btnColor_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setBrushColor0(c);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged({ EntityAttribute {
            StyleAttribute::kBrushColor0, c.name(QColor::HexArgb), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_btnColorStop_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setBrushColor1(c);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged({ EntityAttribute {
            StyleAttribute::kBrushColor1, c.name(QColor::HexArgb), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_btnTextColor_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setTextColor(c);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged({ EntityAttribute {
            StyleAttribute::kTextColor, c.name(QColor::HexArgb), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_comboBoxTextAlignment_currentIndexChanged(int idx)
{
    ColorHandler::Alignment alignment = ui->comboBoxTextAlignment->itemData(idx).value<ColorHandler::Alignment>();
    if (m_colorHandler) {
        m_colorHandler->setTextAlignment(alignment);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged({ EntityAttribute {
            StyleAttribute::kTextAlignment, typeName(alignment), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_fontComboBox_currentFontChanged(const QFont &font)
{
    if (m_colorHandler) {
        QFont f = m_colorHandler->font();
        f.setFamily(font.family());
        m_colorHandler->setFont(f);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged(
            { EntityAttribute { StyleAttribute::kFontFamily, font.family(), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_doubleSpinBoxFontSize_valueChanged(qreal v)
{
    if (m_colorHandler) {
        QFont f = m_colorHandler->font();
        f.setPointSizeF(v);
        m_colorHandler->setFont(f);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged(
            { EntityAttribute { StyleAttribute::kFontPointSize, v, EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::on_comboBoxFontWeight_currentIndexChanged(int idx)
{
    const QFont::Weight weight = ui->comboBoxFontWeight->itemData(idx).value<QFont::Weight>();
    if (m_colorHandler) {
        QFont f = m_colorHandler->font();
        f.setWeight(weight);
        m_colorHandler->setFont(f);
        Q_EMIT colorHandlerValueChanged();
    }
    Q_EMIT entityAttributeChanged(
            { EntityAttribute { StyleAttribute::kFontWeight, typeName(weight), EntityAttribute::Type::Property } });
}

void ColorHandlerEditor::resetObjectAttributes()
{
    for (const auto &attr : m_iObj->entity()->entityAttributes()) {
        if (attr.name().startsWith(QLatin1String("Style")))
            m_iObj->entity()->removeEntityAttribute(attr.name());
    }
    m_iObj->applyColorScheme();
}

void ColorHandlerEditor::validateControls(PropertyTemplateConfig *propertiesConfig)
{
    const bool isPenStyleAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kPenStyle);
    ui->comboBoxPenStyle->setVisible(isPenStyleAvailable);
    ui->labelPenStyle->setVisible(isPenStyleAvailable);
    if (auto propTemplate = propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kPenStyle)) {
        ui->comboBoxPenStyle->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isPenColorAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kPenColor);
    ui->btnColorStroke->setVisible(isPenColorAvailable);
    ui->labelPenColor->setVisible(isPenColorAvailable);
    if (auto propTemplate = propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kPenColor)) {
        ui->btnColorStroke->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isPenWidthAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kPenWidth);
    ui->sbWidth->setVisible(isPenWidthAvailable);
    ui->labelPenWidth->setVisible(isPenWidthAvailable);
    if (auto propTemplate = propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kPenWidth)) {
        ui->sbWidth->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isBrushFillTypeAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kBrushFillType);
    ui->cbFillType->setVisible(isBrushFillTypeAvailable);
    ui->labelFillType->setVisible(isBrushFillTypeAvailable);
    if (auto propTemplate =
                    propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kBrushFillType)) {
        ui->cbFillType->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isBrushColor0Available =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kBrushColor0);
    ui->btnColor->setVisible(isBrushColor0Available);
    ui->labelBrushColor0->setVisible(isBrushColor0Available);
    if (auto propTemplate =
                    propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kBrushColor0)) {
        ui->btnColor->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isBrushColor1Available =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kBrushColor1);
    ui->btnColorStop->setVisible(isBrushColor1Available);
    ui->labelBrushColor1->setVisible(isBrushColor1Available);
    ui->btnColorStop->setVisible(isBrushColor1Available);
    if (auto propTemplate =
                    propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kBrushColor1)) {
        ui->btnColorStop->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isFontFamilyAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kFontFamily);
    ui->fontComboBox->setVisible(isFontFamilyAvailable);
    ui->labelFontFamily->setVisible(isFontFamilyAvailable);
    if (auto propTemplate =
                    propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kFontFamily)) {
        ui->fontComboBox->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isFontPointSizeAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kFontPointSize);
    ui->doubleSpinBoxFontSize->setVisible(isFontPointSizeAvailable);
    ui->labelFontPointSize->setVisible(isFontPointSizeAvailable);
    if (auto propTemplate =
                    propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kFontPointSize)) {
        ui->doubleSpinBoxFontSize->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isFontWeightAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kFontWeight);
    ui->comboBoxFontWeight->setVisible(isFontWeightAvailable);
    ui->labelFontWeight->setVisible(isFontWeightAvailable);
    if (auto propTemplate =
                    propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kFontWeight)) {
        ui->comboBoxFontWeight->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isTextColorAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kTextColor);
    ui->btnTextColor->setVisible(isTextColorAvailable);
    ui->labelTextColor->setVisible(isTextColorAvailable);
    if (auto propTemplate = propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kTextColor)) {
        ui->btnTextColor->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    const bool isTextAlignmentAvailable =
            propertiesConfig->hasPropertyTemplateForObject(m_iObj->entity(), StyleAttribute::kTextAlignment);
    ui->comboBoxTextAlignment->setVisible(isTextAlignmentAvailable);
    ui->labelTextAlignment->setVisible(isTextAlignmentAvailable);
    if (auto propTemplate =
                    propertiesConfig->propertyTemplateForObject(m_iObj->entity(), StyleAttribute::kTextAlignment)) {
        ui->comboBoxTextAlignment->setEnabled(propTemplate->validate(m_iObj->entity()));
    }

    for (auto groupBox : findChildren<QGroupBox *>()) {
        const QWidgetList children = groupBox->findChildren<QWidget *>();
        groupBox->setVisible(std::any_of(children.cbegin(), children.cend(),
                [groupBox](const QWidget *widget) { return widget->isVisibleTo(groupBox); }));
    }
}

} // namespace shared

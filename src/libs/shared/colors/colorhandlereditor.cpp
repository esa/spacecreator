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

#include "colormanager.h"
#include "ui_colorhandlereditor.h"

namespace shared {

ColorHandlerEditor::ColorHandlerEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColorHandlerEditor)
{
    ui->setupUi(this);

    ui->cbFillType->addItem(tr("Color"), ColorHandler::FillType::Color);
    ui->cbFillType->addItem(tr("Gradient Vertical"), ColorHandler::FillType::GradientVertical);
    ui->cbFillType->addItem(tr("Gradient Horizontal"), ColorHandler::FillType::GradientHorizontal);

    setEnabled(false);
}

ColorHandlerEditor::~ColorHandlerEditor()
{
    delete ui;
}

void ColorHandlerEditor::setColorHandler(ColorHandler *h)
{
    m_colorHandler = h;
    setEnabled(m_colorHandler);
    if (!m_colorHandler)
        return;

    QSignalBlocker block(this);
    ui->sbWidth->setValue(m_colorHandler->penWidth());
    ui->btnColorStroke->setColor(m_colorHandler->penColor());
    for (int i = 0; i < ui->cbFillType->count(); ++i) {
        if (ui->cbFillType->itemData(i).toInt() == m_colorHandler->fillType()) {
            ui->cbFillType->setCurrentIndex(i);
            break;
        }
    }
    ui->btnColor->setColor(m_colorHandler->brushColor0());
    ui->btnColorStop->setColor(m_colorHandler->brushColor1());
}

ColorHandler *ColorHandlerEditor::colorHandler() const
{
    return m_colorHandler;
}

void ColorHandlerEditor::on_sbWidth_valueChanged(qreal v)
{
    if (m_colorHandler) {
        m_colorHandler->setPenWidth(v);
        Q_EMIT colorHandlerValueChanged();
    }
}

void ColorHandlerEditor::on_btnColorStroke_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setPenColor(c);
        Q_EMIT colorHandlerValueChanged();
    }
}

void ColorHandlerEditor::on_cbFillType_currentIndexChanged(int id)
{
    ColorHandler::FillType ft = ColorHandler::FillType(ui->cbFillType->itemData(id).toInt());
    const bool stopColorShown = ft != ColorHandler::FillType::Color;

    ui->labelColorStop->setVisible(stopColorShown);
    ui->btnColorStop->setVisible(stopColorShown);

    if (m_colorHandler) {
        m_colorHandler->setFillType(ft);
        Q_EMIT colorHandlerValueChanged();
    }
}

void ColorHandlerEditor::on_btnColor_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setBrushColor0(c);
        Q_EMIT colorHandlerValueChanged();
    }
}

void ColorHandlerEditor::on_btnColorStop_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setBrushColor1(c);
        Q_EMIT colorHandlerValueChanged();
    }
}

}

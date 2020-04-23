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

namespace taste3 {
namespace aadl {

ColorHandlerEditor::ColorHandlerEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ColorHandlerEditor)
{
    ui->setupUi(this);

    ui->cbFillType->addItem(tr("Color"), ColorHandler::FillType::Color);
    ui->cbFillType->addItem(tr("Gradient"), ColorHandler::FillType::Gradient);

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
    ui->cbFillType->setCurrentIndex(m_colorHandler->fillType() == ColorHandler::FillType::Color ? 0 : 1);
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
        emit colorHandlerValueChanged();
    }
}

void ColorHandlerEditor::on_btnColorStroke_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setPenColor(c);
        emit colorHandlerValueChanged();
    }
}

void ColorHandlerEditor::on_cbFillType_currentIndexChanged(int id)
{
    ColorHandler::FillType ft = ColorHandler::FillType(ui->cbFillType->itemData(id).toInt());
    const bool stopColorShown = ft == ColorHandler::FillType::Gradient;

    ui->labelColorStop->setVisible(stopColorShown);
    ui->btnColorStop->setVisible(stopColorShown);

    if (m_colorHandler) {
        m_colorHandler->setFillType(ft);
        emit colorHandlerValueChanged();
    }
}

void ColorHandlerEditor::on_btnColor_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setBrushColor0(c);
        emit colorHandlerValueChanged();
    }
}

void ColorHandlerEditor::on_btnColorStop_colorChanged(const QColor &c)
{
    if (m_colorHandler) {
        m_colorHandler->setBrushColor1(c);
        emit colorHandlerValueChanged();
    }
}

} // ns aadl
} // ns taste3

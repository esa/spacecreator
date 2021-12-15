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

#include "zoomcontroller.h"

#include "ui/graphicsviewbase.h"

#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>

namespace ive {

/*!
 * \class ive::ZoomValidator
 * \brief validator used in ZoomController's combo box.
 * \sa ZoomController
 */
class ZoomValidator : public QValidator
{
public:
    explicit ZoomValidator(QObject *parent = nullptr)
        : QValidator(parent)
        , m_validator(new QIntValidator(this))
    {
    }

    virtual State validate(QString &s, int &i) const override
    {
        clearSuffix(s);
        QValidator::State validated = m_validator->validate(s, i);
        addSuffix(s);

        return validated;
    }

    virtual void fixup(QString &s) const override
    {
        clearSuffix(s);
        m_validator->fixup(s);
        addSuffix(s);
    }

    void setBottom(int min) { m_validator->setBottom(min); }

    void setRange(int bottom, int top) { m_validator->setRange(bottom, top); }

    void setTop(int top) { m_validator->setTop(top); }

    void addSuffix(QString &text) const
    {
        if (!text.endsWith('%'))
            text.append('%');
    }

    QString clearSuffix(QString &value) const
    {
        value.remove('%');
        return value;
    }

private:
    QIntValidator *m_validator;
};

/*!
 * \class ive::ZoomController
 * \brief Control to change, reset and display the zoom level of the graphics scene. Uses editable combo box.
 * \sa ZoomValidator
 */
ZoomController::ZoomController(QWidget *parent)
    : QWidget(parent)
    , m_combo(new QComboBox(this))
    , m_validator(new ZoomValidator(this))
{
    QHBoxLayout *hBox = new QHBoxLayout(this);
    hBox->addWidget(new QLabel(tr("Zoom:")));
    hBox->addWidget(m_combo);
    hBox->setContentsMargins({ 0, 0, 0, 0 });
}

void ZoomController::refill()
{
    QSignalBlocker sb(this);
    m_combo->clear();

    m_combo->setEditable(true);
    m_combo->setInsertPolicy(QComboBox::InsertAtBottom);
    m_combo->setValidator(m_validator);

    for (int z : { 5, 10, 25, 50, 75, 100, 150, 200, 400 }) {
        QString s(QString::number(z));
        m_validator->fixup(s);
        m_combo->addItem(s);
    }
}

void ZoomController::setView(shared::ui::GraphicsViewBase *view)
{
    if (m_view == view)
        return;

    if (m_view) {
        disconnect(m_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
                &ZoomController::onCurrentIndexChanged);
        disconnect(m_view, &shared::ui::GraphicsViewBase::zoomChanged, this, &ZoomController::displayZoomLevel);
    }

    m_view = view;
    setEnabled(nullptr != m_view);

    if (!m_view)
        return;

    m_validator->setBottom(qRound(m_view->minZoomPercent()));
    m_validator->setTop(qRound(m_view->maxZoomPercent()));

    refill();

    connect(m_view, &shared::ui::GraphicsViewBase::zoomChanged, this, &ZoomController::displayZoomLevel);
    connect(m_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ZoomController::onCurrentIndexChanged);

    displayZoomLevel(m_view->zoom());
}

void ZoomController::onCurrentIndexChanged(int idx)
{
    QString strPercent = m_combo->itemText(idx);
    bool ok(false);
    const qreal percent = m_validator->clearSuffix(strPercent).toDouble(&ok);
    if (ok)
        setZoomLevel(percent);
}

void ZoomController::setZoomLevel(qreal percent)
{
    if (m_view)
        m_view->setZoom(percent);
}

void ZoomController::displayZoomLevel(qreal percent)
{
    m_combo->setCurrentText(QString::number(percent));
}

}

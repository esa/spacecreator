/*
   Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "textview.h"

#include "mscfile.h"
#include "mscmodel.h"
#include "mscwriter.h"

/*!
  \class TextView
  \inmodule MscWidgets

  Is a text view of the current model. The text is the same as if it would be saved to a file.
*/

TextView::TextView(QWidget *parent)
    : QTextBrowser(parent)
{
    m_updateTimer.setInterval(10);
    m_updateTimer.setSingleShot(true);
    connect(&m_updateTimer, &QTimer::timeout, this, &TextView::refillView);
}

void TextView::setModel(msc::MscModel *model)
{
    if (model == m_model)
        return;

    if (m_model)
        disconnect(m_model, nullptr, this, nullptr);

    m_model = model;
    connect(m_model, &msc::MscModel::dataChanged, this, &TextView::updateView);

    updateView();
}

void TextView::updateView()
{
    if (m_dirty || !isVisible() || !m_model)
        return;

    m_dirty = true;
    m_updateTimer.start();
}

void TextView::refillView()
{
    if (!m_model)
        return;

    msc::MscWriter mscWriter;
    const QString &mscText = mscWriter.modelText(m_model);
    setTextColor(Qt::black);
    try {
        msc::MscFile file;
        QScopedPointer<msc::MscModel> model(file.parseText(mscText));
    } catch (...) {
        // our own msc file is corrupt - write output in red
        setTextColor(Qt::red);
    }
    setText(mscText);
    m_dirty = false;
}

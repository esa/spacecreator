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

#include <QGuiApplication>

/*!
  \class TextView
  \inmodule MscWidgets

  Is a text view of the current model. The text is the same as if it would be saved to a file.
*/

TextView::TextView(QWidget *parent)
    : QTextBrowser(parent)
{
}

void TextView::setModel(msc::MscModel *model)
{
    m_model = model;
    updateView();
}

void TextView::updateView()
{
    if (m_dirty) {
        return;
    }

    if (QGuiApplication::mouseButtons() != Qt::NoButton) {
        // Prevent the waterflow of "cifChanged" notifications on manual item moves
        return;
    }

    m_dirty = true;
    QMetaObject::invokeMethod(this, "refillView", Qt::QueuedConnection);
}

void TextView::refillView()
{
    Q_ASSERT(m_model);
    m_dirty = false;

    if (!isVisible()) {
        return;
    }

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
}

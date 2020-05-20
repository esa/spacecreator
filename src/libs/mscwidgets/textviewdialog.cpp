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

#include "textviewdialog.h"

#include "delayedsignal.h"
#include "mscfile.h"
#include "mscmodel.h"
#include "mscwriter.h"

#include <QAction>
#include <QPointer>
#include <QTextBrowser>
#include <QVBoxLayout>

namespace msc {

struct TextViewDialog::TextViewDialogPrivate {
    QTextBrowser *textView = nullptr;
    QPointer<msc::MscModel> model;
    shared::DelayedSignal *updateSignal = nullptr;
    QAction *toggleViewAction = nullptr;
};

TextViewDialog::TextViewDialog(QWidget *parent)
    : QDialog(parent)
    , d(new TextViewDialogPrivate)
{
    setWindowTitle(tr("MSC Text Contents"));

    d->textView = new QTextBrowser;
    auto layout = new QVBoxLayout(this);
    layout->addWidget(d->textView);
    layout->setContentsMargins(0, 0, 0, 0);
    d->textView->setFrameStyle(QFrame::NoFrame);

    // Listen to the delayed signals for updating the text
    d->updateSignal = new shared::DelayedSignal(this);
    d->updateSignal->setInterval(10);
    connect(d->updateSignal, &shared::DelayedSignal::triggered, this, &TextViewDialog::updateText);

    setModal(false);
}

TextViewDialog::~TextViewDialog()
{
    delete d;
    d = nullptr;
}

void TextViewDialog::setModel(msc::MscModel *model)
{
    if (model == d->model) {
        return;
    }

    if (d->model != nullptr) {
        d->model->disconnect(this);
    }

    d->model = model;
    if (d->model != nullptr) {
        // Update the text and do this on all changes
        connect(d->model, &msc::MscModel::dataChanged, d->updateSignal, &shared::DelayedSignal::triggerSignal);
        d->updateSignal->triggerSignal();
    }
}

QAction *TextViewDialog::toggleViewAction()
{
    if (d->toggleViewAction == nullptr) {
        d->toggleViewAction = new QAction(tr("Show MSC text dialog"));
        d->toggleViewAction->setCheckable(true);
        d->toggleViewAction->setChecked(isVisible());
        connect(d->toggleViewAction, &QAction::triggered, this, &TextViewDialog::setVisible);
    }
    return d->toggleViewAction;
}

void TextViewDialog::closeEvent(QCloseEvent *)
{
    d->toggleViewAction->setChecked(false);
}

void TextViewDialog::showEvent(QShowEvent *)
{
    d->toggleViewAction->setChecked(true);
    d->updateSignal->triggerSignal();
}

void TextViewDialog::updateText()
{
    if (d->model == nullptr || !isVisible()) {
        return;
    }

    msc::MscWriter mscWriter;
    const QString &mscText = mscWriter.modelText(d->model);

    // Set the text color. Black for the normal color, red if there is a problem
    d->textView->setTextColor(Qt::black);
    try {
        // Test if we can parse the file
        msc::MscFile file;
        QScopedPointer<msc::MscModel> model(file.parseText(mscText));
    } catch (...) {
        // our own msc file is corrupt - write output in red
        d->textView->setTextColor(Qt::red);
    }

    d->textView->setText(mscText);
}

}

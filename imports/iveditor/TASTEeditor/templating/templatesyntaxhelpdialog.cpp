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
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "templatesyntaxhelpdialog.h"

#include <QBoxLayout>
#include <QFile>
#include <QTextBrowser>
#include <QDialogButtonBox>
#include <QDebug>

namespace templating {

/**
 * @brief TemplateSyntaxHelpDialog::TemplateSyntaxHelpDialog ctor
 * @param parent
 */
TemplateSyntaxHelpDialog::TemplateSyntaxHelpDialog(QWidget *parent)
    : QDialog(parent, Qt::Tool)
{
    setWindowTitle(tr("String Template Reference"));

    QTextBrowser *browser = new QTextBrowser();
    browser->setOpenExternalLinks(true);

    QFile helpFile(QStringLiteral("://help/template_help.html"));
    if (helpFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&helpFile);
        browser->setText(stream.readAll());
    }
    else {
        qWarning() << "Unable to open" << helpFile.fileName();
    }

    QDialogButtonBox *closeButtonBox = new QDialogButtonBox(this);
    closeButtonBox->setStandardButtons(QDialogButtonBox::Close);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(browser);
    vLayout->addWidget(closeButtonBox);
    setLayout(vLayout);

    connect(closeButtonBox, &QDialogButtonBox::rejected, this, &TemplateSyntaxHelpDialog::reject);
}

}

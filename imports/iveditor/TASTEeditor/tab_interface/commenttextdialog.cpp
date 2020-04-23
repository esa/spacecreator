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

#include "commenttextdialog.h"

#include "commands/commandids.h"
#include "commands/commandsfactory.h"
#include "ui_commenttextdialog.h"

#include <app/commandsstack.h>
#include <tab_aadl/aadlobjectcomment.h>

namespace taste3 {
namespace aadl {

CommentTextDialog::CommentTextDialog(AADLObjectComment *comment, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CommentTextDialog)
    , m_entity(comment)
{
    ui->setupUi(this);
    connect(this, &QDialog::accepted, this, [this]() {
        const QString text = ui->plainTextEdit->toPlainText();
        if (m_entity->title() == text)
            return;

        const QVariantMap textArg { { meta::Props::token(meta::Props::Token::name), text } };
        const QVariantList commentTextParams { QVariant::fromValue(m_entity), QVariant::fromValue(textArg) };
        auto commentTextCmd = cmd::CommandsFactory::create(cmd::ChangeEntityAttributes, commentTextParams);
        if (commentTextCmd) {
            commentTextCmd->setText(QObject::tr("Edit Comment"));
            taste3::cmd::CommandsStack::current()->push(commentTextCmd);
        }
    });
    ui->plainTextEdit->setPlainText(comment->title());
}

CommentTextDialog::~CommentTextDialog()
{
    delete ui;
}

} // namespace aadl
} // namespace taste3

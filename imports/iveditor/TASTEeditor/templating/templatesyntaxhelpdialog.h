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

#ifndef TEMPLATESYNTAXHELPDIALOG_H
#define TEMPLATESYNTAXHELPDIALOG_H

#include <QDialog>

namespace taste3 {
namespace templating {

/**
 * @brief The TemplateSyntaxHelpDialog class shows all actual built-in tags and filters supported by Grantlee
 */
class TemplateSyntaxHelpDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TemplateSyntaxHelpDialog(QWidget *parent = nullptr);
};

} // ns templating
} // ns taste3

#endif // TEMPLATESYNTAXHELPDIALOG_H

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

#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>

class QSpinBox;
class QTextEdit;

namespace taste3 {
namespace templating {

class StringTemplate;

/**
 * @brief The PreviewDialog class generates XML document from string template file and shows it.
 */
class PreviewDialog : public QDialog
{
    Q_OBJECT
public:
    PreviewDialog(QWidget *parent = nullptr);

    void parse(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName);

    QString text() const;

private slots:
    void onErrorOccurred(const QString &error);
    void onIndentChanged(int value);

private:
    templating::StringTemplate *m_stringTemplate;
    QTextEdit *m_textEdit;
};

} // ns processing
} // ns taste3

#endif // PREVIEWDIALOG_H

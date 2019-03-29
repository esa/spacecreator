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

#pragma once

#include <QDialog>
#include <QVariantMap>

namespace Ui {
class Asn1Editor;
}

namespace asn1 {

class Asn1TreeView;

class Asn1Editor : public QDialog
{
    Q_OBJECT

public:
    Asn1Editor(QWidget *parent = nullptr);
    ~Asn1Editor();

    void setAsn1Types(const QVariantList &asn1Types);
    const QVariantList &asn1Types() const;

    void setFileName(const QString &fileName);
    const QString &fileName() const;

public Q_SLOTS:
    void accept() override;

private Q_SLOTS:
    void openFile();
    void showParseError(const QString &error);
    void showAsn1Type(const QString &text);
    void setAsn1Value();
    void getAsn1Value();

private:
    void loadFile(const QString &file);
    void addAsn1TypeItems();

private:
    Ui::Asn1Editor *ui;
    Asn1TreeView *m_asn1TreeView;
    QVariantList m_asn1Types;
    QString m_fileName;
};

} // namespace asn1

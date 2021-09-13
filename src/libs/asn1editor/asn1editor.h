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
#include <QPointer>
#include <memory>

namespace Ui {
class Asn1Editor;
}

namespace Asn1Acn {
class Asn1SystemChecks;
class File;
}

namespace asn1 {

class Asn1TreeView;

class Asn1Editor : public QDialog
{
    Q_OBJECT

public:
    Asn1Editor(Asn1Acn::Asn1SystemChecks *asn1Checks, QWidget *parent = nullptr);
    Asn1Editor(Asn1Acn::Asn1SystemChecks *asn1Checks, std::unique_ptr<Asn1Acn::File> attrAsn1File,
            QWidget *parent = nullptr);
    ~Asn1Editor();

    void setValue(const QString &value);
    QString value() const;

    void setValueEditOnlyMode();

public Q_SLOTS:
    void accept() override;

    void showAsn1Type(const QString &text);

private Q_SLOTS:
    void showParseError(const QString &error);
    void setAsn1Value();
    void getAsn1Value();

private:
    void addAsn1TypeItems();

private:
    Ui::Asn1Editor *ui;
    Asn1TreeView *m_asn1TreeView;
    QPointer<Asn1Acn::Asn1SystemChecks> m_asn1Checks;
    std::unique_ptr<Asn1Acn::File> m_attrAsn1File;
    QString m_fileName;
};

} // namespace asn1

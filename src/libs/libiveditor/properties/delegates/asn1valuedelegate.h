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

#pragma once

#include <QPointer>
#include <QStyledItemDelegate>

namespace Asn1Acn {
class Asn1SystemChecks;
}
namespace asn1 {
class Asn1Editor;
}

namespace ive {

class Asn1ValueDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    Asn1ValueDelegate(Asn1Acn::Asn1SystemChecks *asn1Checks, QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

protected Q_SLOTS:
    void onDialogAccepted();

private:
    QPointer<Asn1Acn::Asn1SystemChecks> m_asn1Checks;
};

}

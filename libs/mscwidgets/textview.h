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

#pragma once

#include <QPointer>
#include <QTextBrowser>

class MainModel;

namespace msc {
class MscModel;
}

class TextView : public QTextBrowser
{
    Q_OBJECT
public:
    explicit TextView(QWidget *parent = nullptr);
    void setModel(msc::MscModel *model);

public Q_SLOTS:
    void updateView();

private Q_SLOTS:
    void refillView();

private:
    bool m_dirty = false;
    QPointer<msc::MscModel> m_model;
};

/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "document/abstracttabdocument.h"

#include <QGraphicsView>
#include <QPointer>

class QGraphicsView;

namespace document {

class DummyGraphicsScene;
class DummyTabDocument : public AbstractTabDocument
{
    Q_OBJECT
public:
    DummyTabDocument(QObject *parent = nullptr);
    ~DummyTabDocument() override;

    QString title() const override;

protected:
    virtual bool loadImpl(const QString &path) override;
    virtual bool saveImpl(const QString &path) override;
    virtual void closeImpl() override;
    virtual QVector<QAction *> initActions() override;

    QWidget *createView() override;
    QGraphicsScene *createScene() override;

protected Q_SLOTS:
    void onActionDummy();

private:
    QGraphicsScene *m_customScene { nullptr };
    QPointer<QGraphicsView> m_view { nullptr };
    QAction *m_actDummy { nullptr };
};

}

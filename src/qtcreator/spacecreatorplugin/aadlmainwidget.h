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

#include "endtoend/endtoendview.h"
#include "iveditorcore.h"

#include <QPointer>
#include <QSharedPointer>
#include <QVector>
#include <QWidget>

class QAction;
class QGraphicsView;
class QUndoStack;

namespace aadlinterface {
class IVEditorCore;
}

namespace shared {
class ActionsBar;
}

namespace spctr {

class AadlModelStorage;
class MscModelStorage;

class AadlMainWidget : public QWidget
{
    Q_OBJECT
public:
    AadlMainWidget(AadlModelStorage *aadlStorage, MscModelStorage *mscStorage, QWidget *parent = nullptr);
    ~AadlMainWidget();

    bool load(const QString &filename);
    bool save();

    bool isDirty() const;
    QUndoStack *undoStack();

    QString textContents() const;

    void showAsn1Dialog();
    void setMinimapVisible(bool visible);
    void showE2EDataflow(const QStringList &mscFiles);
    void onAttributesManagerRequested();
    void onColorSchemeMenuInvoked();
    void onDynContextEditorMenuInvoked();

    QSharedPointer<aadlinterface::IVEditorCore> ivPlugin() const;

Q_SIGNALS:
    void dirtyChanged(bool dirty);
    void aadlDataLoaded(const QString &fileName, QSharedPointer<aadlinterface::IVEditorCore> data);

private Q_SLOTS:
    void showAsn1Errors(const QStringList &faultyInterfaces);

private:
    void init();

    QSharedPointer<aadlinterface::IVEditorCore> m_plugin;
    QVector<QAction *> m_actions;
    QPointer<AadlModelStorage> m_aadlStorage;
    QPointer<MscModelStorage> m_mscStorage;
    QPointer<aadlinterface::EndToEndView> m_endToEndView;
    shared::ActionsBar *m_aadlToolBar = nullptr;
};

}

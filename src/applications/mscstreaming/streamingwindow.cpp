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

#include "streamingwindow.h"

#include "chartitem.h"
#include "chartlayoutmanager.h"
#include "geometry.h"
#include "mainmodel.h"
#include "remotecontrolhandler.h"
#include "remotecontrolwebserver.h"
#include "ui_streamingwindow.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>

namespace msc {

struct StreamingWindow::StreamingWindowPrivate {
    explicit StreamingWindowPrivate()
        : ui(new Ui::StreamingWindow)
    {
    }

    ~StreamingWindowPrivate() { delete ui; }

    Ui::StreamingWindow *ui = nullptr;

    msc::MainModel m_mainModel;

    msc::RemoteControlWebServer *m_remoteControlWebServer = nullptr;
    msc::RemoteControlHandler *m_remoteControlHandler = nullptr;

    Q_DISABLE_COPY(StreamingWindowPrivate);
};

/*!
  \class StreamingWindow

  This is the main window class for the MSC stream remote control UI.
*/

/*!
 * \brief StreamingWindow::StreamingWindow Create an empty view.
 * \param plugin hosts most code of the code for the MSC UI of type \ref msc::MSCEditorCore
 * \param parent
 */
StreamingWindow::StreamingWindow(QWidget *parent)
    : QMainWindow(parent)
    , d(new StreamingWindowPrivate)
{
    d->ui->setupUi(this);
    d->ui->graphicsView->setScene(d->m_mainModel.graphicsScene());

    static constexpr qreal padding = 120.;
    const QSizeF defaultSize(this->size() - QSizeF(padding, padding));
    d->m_mainModel.chartViewModel().setPreferredChartBoxSize(defaultSize);
    d->m_mainModel.initialModel();

    connect(d->m_mainModel.graphicsScene(), &QGraphicsScene::sceneRectChanged, this,
            &StreamingWindow::adaptWindowSizeToChart);
}

StreamingWindow::~StreamingWindow() { }

/*!
 * \brief StreamingWindow::startRemoteControl Start the remote app controller
 * \param port Listen on this port
 * \return True if success
 */
bool StreamingWindow::startRemoteControl(quint16 port)
{
    if (!d->m_remoteControlWebServer) {
        d->m_remoteControlWebServer = new msc::RemoteControlWebServer(this);
        d->m_remoteControlHandler = new msc::RemoteControlHandler(this);
        d->m_remoteControlHandler->setMscModel(d->m_mainModel.mscModel());
        d->m_remoteControlHandler->setUndoStack(d->m_mainModel.undoStack());
        d->m_remoteControlHandler->setLayoutManager(&(d->m_mainModel.chartViewModel()));

        connect(d->m_remoteControlWebServer, &msc::RemoteControlWebServer::executeCommand, d->m_remoteControlHandler,
                &msc::RemoteControlHandler::handleRemoteCommand);
        connect(d->m_remoteControlHandler, &msc::RemoteControlHandler::commandDone, d->m_remoteControlWebServer,
                &msc::RemoteControlWebServer::commandDone);
    }
    if (d->m_remoteControlWebServer->start(port)) {
        return true;
    }

    qCritical() << "Unable to start server on port " << port;

    d->m_remoteControlWebServer->deleteLater();
    d->m_remoteControlWebServer = nullptr;
    d->m_remoteControlHandler->deleteLater();
    d->m_remoteControlHandler = nullptr;
    return false;
}

void StreamingWindow::adaptWindowSizeToChart(const QRectF &rect)
{
    QRect windowRect = geometry();
    QRect widgetRect = d->ui->graphicsView->geometry();
    const QSize extraMargin(5, 5);
    const QSize offsets(windowRect.width() - widgetRect.width(), windowRect.height() - widgetRect.height());
    widgetRect = rect.marginsAdded(msc::ChartItem::chartMargins()).toRect();
    windowRect.setSize(rect.size().toSize() + offsets + extraMargin);

    const QRect availableRect = screen()->availableGeometry();
    windowRect = shared::rectInRect(windowRect, availableRect);

    setGeometry(windowRect);
}

/**
   @todo With Qt >= 5.14 use QWidget::screen()
 */
QScreen *StreamingWindow::screen() const
{
    if (auto screenByPos = QGuiApplication::screenAt(geometry().center())) {
        return screenByPos;
    }

    return QGuiApplication::primaryScreen();
}

void StreamingWindow::closeEvent(QCloseEvent *e)
{
    QMainWindow::closeEvent(e);
    QApplication::quit();
}

}

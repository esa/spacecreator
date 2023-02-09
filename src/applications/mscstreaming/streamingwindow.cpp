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

#include "baseitems/instanceheaditem.h"
#include "chartitem.h"
#include "geometry.h"
#include "instanceitem.h"
#include "mscchartviewconstants.h"
#include "msccommandsstack.h"
#include "mscmodel.h"
#include "remotecontrolhandler.h"
#include "remotecontrolwebserver.h"
#include "streaminglayoutmanager.h"
#include "ui_streamingwindow.h"

#include <QApplication>
#include <QDebug>
#include <QRect>
#include <QScreen>
#include <QScrollBar>
#include <QStyle>
#include <QUndoStack>
#include <memory>

namespace msc {

struct StreamingWindow::StreamingWindowPrivate {
    explicit StreamingWindowPrivate()
        : ui(new Ui::StreamingWindow)
        , m_dataModel(std::move(MscModel::defaultModel()))
        , m_undoStack(new MscCommandsStack)
        , m_layoutManager(new StreamingLayoutManager(m_undoStack.get()))
    {
    }

    ~StreamingWindowPrivate() { delete ui; }

    Ui::StreamingWindow *ui = nullptr;

    std::unique_ptr<MscModel> m_dataModel;
    std::unique_ptr<MscCommandsStack> m_undoStack;
    std::unique_ptr<StreamingLayoutManager> m_layoutManager;
    std::unique_ptr<QGraphicsScene> m_overlayScene;

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
    d->ui->graphicsView->setScene(d->m_layoutManager->graphicsScene());
    d->m_layoutManager->setCurrentChart(d->m_dataModel->firstChart());

    static constexpr qreal padding = 120.;
    const QSizeF defaultSize(this->size() - QSizeF(padding, padding));

    connect(d->m_layoutManager->graphicsScene(), &QGraphicsScene::sceneRectChanged, this,
            &StreamingWindow::adaptWindowSizeToChart);

    d->m_overlayScene = std::make_unique<QGraphicsScene>();
    d->ui->instanceGraphicsView->setScene(d->m_overlayScene.get());
    syncInstances();
    checkInstancesVisibility();
    d->ui->graphicsView->installEventFilter(this);
    connect(d->m_layoutManager.get(), &StreamingLayoutManager::instanceItemsChanged, this,
            &StreamingWindow::syncInstances, Qt::QueuedConnection);

    connect(d->ui->graphicsView->verticalScrollBar(), &QScrollBar::valueChanged, this,
            &StreamingWindow::checkInstancesVisibility, Qt::QueuedConnection);
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
        d->m_remoteControlHandler->setMscModel(d->m_dataModel.get());
        d->m_remoteControlHandler->setUndoStack(d->m_undoStack->undoStack());
        d->m_remoteControlHandler->setLayoutManager(d->m_layoutManager.get());
        d->m_remoteControlHandler->setChart(d->m_dataModel->firstChart());

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
    const QRect widgetRect = d->ui->graphicsView->geometry();
    const int extraMargin = 5;

    QRect newWidgetRect = rect.marginsAdded(msc::ChartItem::chartMargins()).toRect();
    bool scroll = false;
    if (newWidgetRect.height() > widgetRect.height()) {
        scroll = true;
        // add space for the scrollbar
        newWidgetRect.setWidth(newWidgetRect.width() + qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent));
    }
    const int newWidth = newWidgetRect.width() + extraMargin;
    if (newWidth > windowRect.width()) {
        windowRect.setWidth(newWidth);
    }

    const QRect availableRect = screen()->availableGeometry();
    windowRect = shared::rectInRect(windowRect, availableRect);

    setGeometry(windowRect);

    if (d->m_overlayScene->width() != d->m_layoutManager->graphicsScene()->width()) {
        QRectF rect = d->m_overlayScene->sceneRect();
        rect.setWidth(d->m_layoutManager->graphicsScene()->width());
        d->m_overlayScene->setSceneRect(rect);
        syncInstances();
    }

    if (scroll) {
        scrollToBottom();
    }
}

void StreamingWindow::syncInstances()
{
    d->m_overlayScene->clear();

    qreal height = 0;

    for (InstanceItem *item : d->m_layoutManager->instanceItems()) {
        auto header = new InstanceHeadItem(d->m_layoutManager->currentChart());
        header->setName(item->name());
        header->setX(item->x() + CHART_BOX_MARGIN + 12); /// @todo - why?!?
        header->setY(-30); /// @todo - why?!?
        d->m_overlayScene->addItem(header);
        height = std::max(height, header->boundingRect().height() + 10);
    }

    d->ui->instanceGraphicsView->setMaximumHeight(height);
    d->ui->instanceGraphicsView->setMinimumHeight(height);
}

void StreamingWindow::checkInstancesVisibility()
{
    QScrollBar *bar = d->ui->graphicsView->verticalScrollBar();
    if (bar && bar->isVisible() && bar->value() > 0) {
        d->ui->instanceGraphicsView->setVisible(true);
        return;
    }
    d->ui->instanceGraphicsView->setVisible(false);
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

bool StreamingWindow::eventFilter(QObject *obj, QEvent *event)
{
    bool result = QMainWindow::eventFilter(obj, event);

    if (event->type() == QEvent::Resize && obj == d->ui->graphicsView) {
        const qreal sceneWidth = d->m_layoutManager->graphicsScene()->sceneRect().width();
        QRectF rect = d->ui->instanceGraphicsView->sceneRect();

        rect.setWidth(sceneWidth);
        if (d->m_overlayScene->sceneRect().width() != rect.width()) {
            d->m_overlayScene->setSceneRect(rect);
            syncInstances();
        }
    }

    return result;
}

void StreamingWindow::scrollToBottom()
{
    QRectF sceneRect = d->m_layoutManager->graphicsScene()->sceneRect();
    d->ui->graphicsView->ensureVisible(sceneRect.center().x(), sceneRect.bottom() + 1, 1, 1);
}

}

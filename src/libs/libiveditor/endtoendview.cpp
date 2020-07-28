#include "endtoendview.h"

#include "ui/graphicsviewbase.h"

#include <QBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QPushButton>

namespace aadlinterface {

struct EndToEndView::EndToEndViewPrivate {
    shared::ui::GraphicsViewBase *view { nullptr };

    QString mscFilePath;
    QString dir;
};

EndToEndView::EndToEndView(QWidget *parent)
    : QDialog(parent)
    , d(new EndToEndViewPrivate)
{
    setWindowTitle(tr("End To End Dataflow"));
    setAttribute(Qt::WA_DeleteOnClose, false);
    setAttribute(Qt::WA_QuitOnClose, false);

    auto pathLabel = new QLabel(tr("MSC file: -"));
    auto pathButton = new QPushButton(tr("&Choose MSC file"));
    auto refreshButton = new QPushButton(tr("&Refresh view"));

    d->view = new shared::ui::GraphicsViewBase;

    auto barLayout = new QHBoxLayout;
    barLayout->addWidget(pathLabel);
    barLayout->addWidget(pathButton);
    barLayout->addWidget(refreshButton);
    barLayout->addStretch(1);
    auto layout = new QVBoxLayout(this);
    layout->addLayout(barLayout);
    layout->addWidget(d->view);

    connect(pathButton, &QPushButton::clicked, this, [=]() {
        const QString path = QFileDialog::getOpenFileName(
                this, tr("Choose MSC file"), d->dir, tr("MSC files (*.msc);;All files (*)"));
        if (!path.isEmpty()) {
            d->mscFilePath = path;

            QFileInfo info(path);
            pathLabel->setText(tr("MSC file: %1").arg(info.fileName()));
            d->dir = info.path();

            refreshButton->click();
        }
    });

    connect(refreshButton, &QPushButton::clicked, this, &EndToEndView::refreshView);
}

EndToEndView::~EndToEndView()
{
    delete d;
    d = nullptr;
}

void EndToEndView::setVisible(bool visible)
{
    const bool wasVisible = isVisible();
    QWidget::setVisible(visible);
    if (isVisible() != wasVisible) {
        Q_EMIT visibleChanged(visible);
    }
}

void EndToEndView::refreshView() { }

}

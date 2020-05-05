#pragma once

#include <QGraphicsView>

namespace shared {
namespace ui {

class GraphicsViewBase : public QGraphicsView
{
    Q_OBJECT

    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(qreal minZoomPercent READ minZoomPercent WRITE setMinZoomPercent NOTIFY minZoomPercentChanged)
    Q_PROPERTY(qreal maxZoomPercent READ maxZoomPercent WRITE setMaxZoomPercent NOTIFY maxZoomPercentChanged)
    Q_PROPERTY(qreal zoomStepPercent READ zoomStepPercent WRITE setZoomStepPercent NOTIFY zoomStepPercentChanged)

public:
    explicit GraphicsViewBase(QGraphicsScene* scene, QWidget* parent = nullptr);
    explicit GraphicsViewBase(QWidget* parent = nullptr);
    ~GraphicsViewBase() override;

    double zoom() const;
    qreal minZoomPercent() const;
    qreal maxZoomPercent() const;
    qreal zoomStepPercent() const;

Q_SIGNALS:
    void mouseMoved(const QString &coordsInfo) const;
    void zoomChanged(qreal percent);
    void minZoomPercentChanged(qreal percent);
    void maxZoomPercentChanged(qreal percent);
    void zoomStepPercentChanged(qreal percent);

public Q_SLOTS:
    void setZoom(double percent);
    void setMinZoomPercent(qreal percent);
    void setMaxZoomPercent(qreal percent);
    void setZoomStepPercent(qreal percent);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    struct GraphicsViewBasePrivate;
    GraphicsViewBasePrivate* d;
};

}
}


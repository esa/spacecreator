/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

   This application is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   This application is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
*/

#pragma once

#include <QObject>
#include <QWidget>
#include <memory>

class QGraphicsView;

namespace shared {
namespace ui {

struct MiniMapPrivate;
class MiniMap : public QWidget
{
    Q_OBJECT
public:
    explicit MiniMap(QWidget *parent = nullptr);
    ~MiniMap(); // the definition (even an empty) is necessary for the dptr (std::unique_ptr)

    void setupSourceView(QGraphicsView *view);

Q_SIGNALS:
    void visibilityChanged(bool visible);

protected:
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

    void updateSceneContent();
    void updateViewportFrame();

protected Q_SLOTS:
    void onSceneUpdated();
    void onViewUpdated();

private:
    const std::unique_ptr<MiniMapPrivate> d;

    void composeMap();
};

} // namespace ui
} // namespace shared

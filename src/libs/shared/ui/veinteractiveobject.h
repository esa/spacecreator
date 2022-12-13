/*
   Copyright (C) 2018-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "colors/colormanager.h"
#include "ui/interactiveobjectbase.h"

#include <QFont>
#include <QPen>
#include <QPointer>

namespace shared {
namespace cmd {
class CommandsStackBase;
}
class VEObject;
namespace ui {
class TextItem;

class VEInteractiveObject : public ui::InteractiveObjectBase
{
    Q_OBJECT
public:
    explicit VEInteractiveObject(VEObject *entity, QGraphicsItem *parent = nullptr);

    QPen pen() const;
    void setPen(const QPen &pen);

    QBrush brush() const;
    void setBrush(const QBrush &brush);

    QFont font() const;
    void setFont(const QFont &font);

    void setCommandsStack(cmd::CommandsStackBase *commandsStack);

    QString toString() const;

public:
    /**
     * @brief updateText from model object and updates the text position
     * by calling 'updateTextPosition'
     */
    void updateText();

public:
    // Methods for children to implement

    virtual int itemLevel(bool isSelected) const = 0;
    virtual VEObject *entity() const;

    /**
     * @brief init initializes InteractiveObject's TextItem and applys Color Scheme
     */
    virtual void init();


    /**
     * @brief updateEntity pushes command on command stack
     */
    virtual void updateEntity();

    /**
     * @brief updateFromEntity
     */
    virtual void updateFromEntity() = 0;
    virtual bool doLayout() = 0;
    virtual void rebuildLayout() override;
    virtual QList<QPair<VEObject *, QVector<QPointF>>> prepareChangeCoordinatesCommandParams() const;

    virtual QString prepareTooltip() const;
    virtual void enableEditMode() {};
    virtual void childBoundingBoxChanged();



Q_SIGNALS:
    void clicked(const QPointF &scenePos);
    void doubleClicked();

public Q_SLOTS:
    virtual void applyColorScheme() = 0;

protected:
    // Methods to be implemented by child classes

    /**
     * @brief initTextItem sets up a basic (non-interactive) text item.
     * Children classes can overload this method to set up the text item specific for their role.
     * Called by init()
     * @return a TextItem
     */
    virtual TextItem *initTextItem();

    /**
     * @brief updateTextPosition
     * Called when boundingBoxChanged is emitted and when the text item is updated from the model.
     * Children classes can overload this method to set up the text item specific for their role. This could be name label for a
     * function name, or an interface name next to the interface.
     */
    virtual void updateTextPosition() = 0;

    /**
     * @brief handledColorType
     * Each child of of this class must be able to return its type as denoted by the enum ColorManager::HandledColors
     * @return ColorManager::HandledColors
     */
    virtual ColorManager::HandledColors handledColorType() const = 0;

    /**
     * @brief colorHandler
     * The default implementation of colorHandler uses 'handledColorType' to instantiate a ColorHandler
     * @return a ColorHandler
     */
    virtual ColorHandler colorHandler() const;

protected:
    // Callbacks for receiving events
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void onSelectionChanged(bool isSelected) override;

    void mergeGeometry();


protected:
    const QPointer<VEObject> m_dataObject;
    QPointer<cmd::CommandsStackBase> m_commandsStack;
    TextItem *m_textItem;

    QBrush m_brush;
    QPen m_pen;
    QFont m_font;
};
QDebug operator<<(QDebug debug, const shared::ui::VEInteractiveObject &veobj);


} // namespace ui
} // namespace shared

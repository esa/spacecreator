/*
   Copyright (C) 2018 European Space Agency - <maxime.perrotin@esa.int>

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

#include "basecreatortool.h"
#include <mscmessage.h>
#include <messageitem.h>

namespace msc {

class MessageCreatorTool : public BaseCreatorTool
{
    Q_OBJECT
public:
    MessageCreatorTool(ChartViewModel *model, QGraphicsView *view, QObject *parent = nullptr);
    virtual ToolType toolType() const override;

protected:
    QPointer<MscMessage> m_message = nullptr;
    QPointer<MessageItem> m_messageItem = nullptr;

    void createPreviewItem() override;
    void commitPreviewItem() override;
    void removePreviewItem() override;
};

} // ns msc

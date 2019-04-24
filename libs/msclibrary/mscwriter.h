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

#pragma once

#include <QObject>

namespace msc {

class MscAction;
class MscChart;
class MscCondition;
class MscCoregion;
class MscCreate;
class MscDocument;
class MscEntity;
class MscInstance;
class MscInstanceEvent;
class MscMessage;
class MscMessageDeclaration;
class MscMessageDeclarationList;
class MscModel;
class MscTimer;

class MscWriter : public QObject
{
    Q_OBJECT

public:
    MscWriter(QObject *parent = nullptr);

    bool saveModel(MscModel *model, const QString &fileName);
    bool saveChart(const MscChart *chart, const QString &fileName);
    QString modelText(MscModel *model);

    QString serialize(const MscInstance *instance, const QVector<msc::MscInstanceEvent *> &instanceEvents,
                      int tabsSize = 0);
    QString serialize(const MscMessage *message, const MscInstance *instance, int tabsSize = 0);
    QString serialize(const MscCondition *condition, int tabsSize = 0);
    QString serialize(const MscCreate *create, const MscInstance *instance, int tabsSize = 0);
    QString serialize(const MscTimer *timer, const MscInstance *instance, int tabsSize = 0);
    QString serialize(const MscAction *action, const MscInstance *instance, int tabsSize = 0);
    QString serialize(const MscCoregion *region, const MscInstance *instance, int tabsSize = 0);
    QString serialize(const MscMessageDeclarationList *declarationList, int tabsSize = 0);
    QString serialize(const MscMessageDeclaration *declaration, int tabsSize = 0);
    QString serialize(const MscChart *chart, int tabsSize = 0);
    QString serialize(const MscDocument *document, int tabsSize = 0);

    void setModel(MscModel *model);

private:
    QString tabs(int tabsSize) const;
    QString dataDefinition() const;
    QString serializeComment(const msc::MscEntity *entity) const;
    QString serializeParameters(const MscMessage *message) const;

    QString serializeCif(const msc::MscEntity *entity, const QString &entitySerialized, int tabsSize) const;

    MscModel *m_model = nullptr;
};

} // namespace msc

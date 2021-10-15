/*
   Copyright (C) 2018 - 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "msceditorcore.h"

#include <QPointer>
#include <QSharedPointer>
#include <coreplugin/idocument.h>

namespace spctr {
class SpaceCreatorProjectManager;

class MscEditorDocument : public Core::IDocument
{
    Q_OBJECT

public:
    explicit MscEditorDocument(SpaceCreatorProjectManager *projectManager, QObject *parent = nullptr);

    // IDocument
    OpenResult open(QString *errorString, const QString &fileName, const QString &realFileName) override;
    bool save(QString *errorString, const QString &fileName, bool autoSave) override;
#if QTC_VERSION == 415
    void setFilePath(const Utils::FilePath &) override;
#elif QTC_VERSION == 48
    void setFilePath(const Utils::FileName &) override;
#else
#warning(Unsupported QtC version)
#endif
    bool shouldAutoSave() const override;
    bool isSaveAsAllowed() const override;
    bool isModified() const override;
    bool reload(QString *errorString, ReloadFlag flag, ChangeType type) override;

    // Internal
    MSCEditorCorePtr mscEditorCore() const;

public Q_SLOTS:
    void checkForErrors();

Q_SIGNALS:
    void reloadRequested(QString *errorString, const QString &);
    void mscDataLoaded(const QString &fileName, MSCEditorCorePtr data);

private:
    QPointer<SpaceCreatorProjectManager> m_projectManager;
    MSCEditorCorePtr m_plugin;
};

}

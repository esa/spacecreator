/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021-2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#pragma once

#include "spinrunworker.h"

#include <QDialog>
#include <QPushButton>
#include <QStringList>
#include <QTextBrowser>
#include <QVBoxLayout>

namespace ive {
class SpinRunDialog : public QDialog
{
    Q_OBJECT

public:
    SpinRunDialog(QWidget *parent);

    void setOutputDirectory(const QString &outputDirectory);
    void setPropertiesSelected(const QStringList &properties);
    void setSubtypesSelected(const QStringList &subtypes);
    void setFunctionsSelected(const QStringList &functions);
    void setSpinConfig(const SpinConfigData &config);
    void setProjectRoot(const QString &projectRoot);
    void setPropertiesPath(const QString &propertiesPath);
    void setSubtypesPath(const QString &subtypesPath);

    int exec() override;

protected:
    void reject() override;

private Q_SLOTS:
    void workerTextAvailable(QString text);
    void workerFinished();
    void jobFinished(bool success);
    void closeButtonClicked();

private:
    QVBoxLayout *m_layout;
    QTextBrowser *m_browser;
    QHBoxLayout *m_secondLayout;
    QPushButton *m_closeButton;

    SpinRunWorker *m_worker;

    QString m_outputDirectory;
    QStringList m_propertiesSelected;
    QStringList m_subtypesSelected;
    QStringList m_functionsSelected;
    SpinConfigData m_spinConfig;
    QString m_projectRoot;
    QString m_propertiesPath;
    QString m_subtypesPath;

    bool m_closePending;
    bool m_finished;
};
}

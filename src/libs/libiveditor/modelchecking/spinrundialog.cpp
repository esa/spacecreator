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

#include "spinrundialog.h"

namespace ive {
SpinRunDialog::SpinRunDialog(QWidget *parent)
    : QDialog(parent)
    , m_layout(new QVBoxLayout(this))
    , m_browser(new QTextBrowser(this))
    , m_secondLayout(new QHBoxLayout())
{
    m_closeButton = new QPushButton(tr("Close"), this);
    setWindowTitle("Spin model-checker");
    setMinimumSize(600, 400);
    setLayout(m_layout);
    m_layout->addWidget(m_browser);
    m_layout->addLayout(m_secondLayout);

    m_secondLayout->setDirection(QHBoxLayout::Direction::RightToLeft);
    m_secondLayout->QLayout::addWidget(m_closeButton);

    QFont font = m_browser->font();
    font.setFamily("monospace");
    font.setStyleHint(QFont::StyleHint::Monospace);
    font.setFixedPitch(true);
    m_browser->setFont(font);

    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()));
}

void SpinRunDialog::setPropertiesSelected(const QStringList &properties)
{
    m_propertiesSelected = properties;
}

void SpinRunDialog::setSubtypesSelected(const QStringList &subtypes)
{
    m_subtypesSelected = subtypes;
}

void SpinRunDialog::setFunctionsSelected(const QStringList &functions)
{
    m_functionsSelected = functions;
}

void SpinRunDialog::setSpinConfig(const SpinConfigData &config)
{
    m_spinConfig = config;
}

void SpinRunDialog::setProjectRoot(const QString &projectRoot)
{
    m_projectRoot = projectRoot;
}

void SpinRunDialog::setPropertiesPath(const QString &propertiesPath)
{
    m_propertiesPath = propertiesPath;
}

void SpinRunDialog::setSubtypesPath(const QString &subtypesPath)
{
    m_subtypesPath = subtypesPath;
}

int SpinRunDialog::exec()
{
    m_worker = new SpinRunWorker(this, m_propertiesSelected, m_subtypesSelected, m_functionsSelected, m_spinConfig,
            m_projectRoot, "tmc-out");
    m_worker->setPropertiesPath(m_propertiesPath);
    m_worker->setSubtypesPath(m_subtypesPath);
    connect(m_worker, SIGNAL(textAvailable(QString)), this, SLOT(workerTextAvailable(QString)), Qt::QueuedConnection);
    connect(m_worker, SIGNAL(jobFinished(bool)), this, SLOT(workerFinished(bool)), Qt::QueuedConnection);
    m_worker->start();
    return QDialog::exec();
}

void SpinRunDialog::workerTextAvailable(QString text)
{
    m_browser->moveCursor(QTextCursor::MoveOperation::End);
    m_browser->insertPlainText(text);
    m_browser->moveCursor(QTextCursor::MoveOperation::End);
}

void SpinRunDialog::workerFinished(bool success)
{
    if (success) {
        m_browser->append("FINISHED.");
        m_browser->moveCursor(QTextCursor::MoveOperation::End);
    } else {
        m_browser->append("FAILURE.");
        m_browser->moveCursor(QTextCursor::MoveOperation::End);
    }
}

void SpinRunDialog::closeButtonClicked()
{
    m_worker->stop();
}
}

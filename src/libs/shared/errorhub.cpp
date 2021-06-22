/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "errorhub.h"

#include <algorithm>

namespace shared {

ErrorHub *ErrorHub::m_instance = nullptr;

ErrorItem::ErrorItem() { }

ErrorItem::ErrorItem(TaskType type, const QString &description, const QString &fileName, int line)
    : m_type(type)
    , m_description((description))
    , m_fileName(fileName)
    , m_line(line)
{
}

ErrorHub::ErrorHub(QObject *parent)

    : QObject(parent)
{
}

ErrorHub *ErrorHub::instance()
{
    if (m_instance == nullptr) {
        m_instance = new ErrorHub;
    }
    return m_instance;
}

void ErrorHub::addError(ErrorItem::TaskType type, const QString &description, const QString &fileName, int line)
{
    instance()->m_errors.append(ErrorItem(type, description.isEmpty() ? tr("Unknown error") : description,
            fileName.isEmpty() ? m_instance->m_currentFile : fileName, line));
    Q_EMIT m_instance->errorAdded(m_instance->m_errors.last());
}

void ErrorHub::clearErrors()
{
    instance()->m_errors.clear();
    Q_EMIT m_instance->cleared();
}

void ErrorHub::clearFileErrors(const QString &fileName)
{
    auto it = std::remove_if(instance()->m_errors.begin(), instance()->m_errors.end(),
            [&fileName](const ErrorItem &item) { return item.m_fileName == fileName; });
    instance()->m_errors.erase(it, instance()->m_errors.end());

    Q_EMIT instance()->clearedFile(fileName);
}

const QVector<ErrorItem> &ErrorHub::errors()
{
    return instance()->m_errors;
}

QStringList ErrorHub::errorDescriptions()
{
    QStringList messages;
    for (const ErrorItem &item : instance()->m_errors) {
        messages.append(item.m_description);
    }
    return messages;
}

bool ErrorHub::hasErrors()
{
    return !instance()->m_errors.isEmpty();
}

void ErrorHub::setCurrentFile(const QString &fileName)
{
    instance()->m_currentFile = fileName;
}

void ErrorHub::clearCurrentFile()
{
    instance()->m_currentFile.clear();
}

} // namespace shared

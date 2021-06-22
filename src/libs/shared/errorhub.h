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

#pragma once

#include <QObject>
#include <QVector>

namespace shared {

class ErrorItem
{
public:
    enum TaskType
    {
        Unknown,
        Error,
        Warning
    };

    ErrorItem();
    ErrorItem(TaskType type, const QString &description, const QString &fileName, int line = -1);

    TaskType m_type = Error;
    QString m_description;
    QString m_fileName;
    int m_line = -1;
};

class ErrorHub : public QObject
{
    Q_OBJECT
public:
    explicit ErrorHub(QObject *parent = nullptr);

    static ErrorHub *instance();

    static void addError(ErrorItem::TaskType type, const QString &description, const QString &fileName, int line = -1);
    static void clearErrors();
    static void clearFileErrors(const QString &fileName);
    static const QVector<ErrorItem> &errors();
    static QStringList errorDescriptions();

    static bool hasErrors();

    static void setCurrentFile(const QString &fileName);
    static void clearCurrentFile();

Q_SIGNALS:
    void errorAdded(const ErrorItem &error);
    void cleared();
    void clearedFile(const QString &fileName);

private:
    static ErrorHub *m_instance;

    QVector<ErrorItem> m_errors;
    QString m_currentFile;
};

} // namespace shared

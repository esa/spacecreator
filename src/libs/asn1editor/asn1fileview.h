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

#include <QWidget>

class QFileInfo;

namespace Ui {
class ASN1FileView;
}

namespace asn1 {

class ASN1FileView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool fileContentVisible READ fileContentVisible WRITE setFileContentVisible NOTIFY
                    fileContentVisibleChanged)

public:
    explicit ASN1FileView(QWidget *parent = nullptr);
    ~ASN1FileView();

    QSize sizeHint() const override;

    QString fileName() const;

    bool fileContentVisible() const;

public Q_SLOTS:
    void setDirectory(const QString &directory);
    void setFileName(const QString &fileName);
    void setFile(const QFileInfo &file);
    void fillPreview();

    void setFileContentVisible(bool fileContentVisible);

Q_SIGNALS:
    void asn1Selected(const QString &fileName);
    void fileContentVisibleChanged();

private Q_SLOTS:
    void selectFile();

private:
    Ui::ASN1FileView *ui;
    QString m_directory;
};

}

/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include <QDialog>
#include <functional>

namespace msc {
class MscModel;
}

namespace ive {

class InterfaceDocument;

/*!
 * \brief The EndToEndView class shows the MSC message sequence through the interface view
 */
class EndToEndView : public QDialog
{
    Q_OBJECT

public:
    explicit EndToEndView(InterfaceDocument *document, QWidget *parent);
    explicit EndToEndView(InterfaceDocument *document);
    ~EndToEndView() override;

    void setVisible(bool visible) override;
    void setMscFiles(const QStringList &files);
    void setMscDataFetcher(std::function<msc::MscModel *(QString)> fetcher);
    bool exportScene(const QString &outputPath);

    bool refreshView();
    bool setMscFile(const QString &fileName, QString *errorString = nullptr);
    bool setMscChart(const QString &chartName);

Q_SIGNALS:
    void visibleChanged(bool visible);

private Q_SLOTS:
    bool saveSceneToRaster(const QString &outputPath);
    bool saveSceneToVector(const QString &outputPath);
    void callExport();

private:
    struct EndToEndViewPrivate;
    EndToEndViewPrivate *d;
};

}

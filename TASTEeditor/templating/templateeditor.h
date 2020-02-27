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

#ifndef TEMPLATEEDITOR_H
#define TEMPLATEEDITOR_H

#include <QDialog>
#include <QVariant>

class QSpinBox;
class QTabWidget;
class QPlainTextEdit;

namespace taste3 {
namespace templating {

class StringTemplate;
class TemplateSyntaxHelpDialog;

/**
 * @brief The PreviewDialog class generates XML document or another text from string template file and shows it.
 * Also it possible to edit, open and save template(s)
 */
class TemplateEditor : public QDialog
{
    Q_OBJECT
public:
    TemplateEditor(QWidget *parent = nullptr);

    bool parseTemplate(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName);
    bool saveResultToFile(const QString &fileName);

    QString resultText() const;

private slots:
    void onApplyTemplate();
    void onSaveTemplateAs();
    void onOpenTemplate();
    void onHelpRequested();
    void onSaveResult();
    void onErrorOccurred(const QString &errorString);
    void onValidateXMLToggled(bool validate);
    void onIndentChanged(int value);

private:
    QPlainTextEdit *addTemplateEditor(const QString &tabLabel = QString());
    bool parseTemplate();
    void openIncludedTemplates(const QString &templateText);

private:
    templating::StringTemplate *m_stringTemplate;
    QTabWidget *m_templatesTabWidget;
    QPlainTextEdit *m_resultTextEdit;
    QHash<QString, QVariantList> m_grouppedObjects;
    QString m_templateFileName;
    QHash<QString, QString> m_openedTemplates; // key: file name, value: full path
    TemplateSyntaxHelpDialog *m_helpDialog;
};

} // ns templating
} // ns taste3

#endif // TEMPLATEEDITOR_H

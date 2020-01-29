#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QDialog>

class QSpinBox;
class QTextEdit;

namespace taste3 {
namespace templating {

class StringTemplate;

class PreviewDialog : public QDialog
{
    Q_OBJECT
public:
    PreviewDialog(QWidget *parent = nullptr);

    QString parse(const QHash<QString, QVariantList> &grouppedObjects, const QString &templateFileName);

private slots:
    void onErrorOccurred(const QString &error);
    void onIndentChanged(int value);

private:
    templating::StringTemplate *m_stringTemplate;
    QTextEdit *m_textEdit;
};

} // ns processing
} // ns taste3

#endif // PREVIEWDIALOG_H

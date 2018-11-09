#ifndef ASN1EDITWIDGET_H
#define ASN1EDITWIDGET_H

#include <QDialog>

namespace Ui {
    class Asn1Editor;
}

class Asn1Editor : public QDialog
{
    Q_OBJECT

public:
    Asn1Editor(QWidget *parent = nullptr);
    ~Asn1Editor();

public Q_SLOTS:
    void openFile();
    void showParseError(const QString &error);

private:
    void loadFile(const QString &file);

private:
    Ui::Asn1Editor *ui;
};

#endif // ASN1EDITWIDGET_H

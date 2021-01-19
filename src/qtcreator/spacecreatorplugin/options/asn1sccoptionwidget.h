#pragma once

#include <QWidget>

namespace spctr {

namespace Ui {
class Asn1SccOptionWidget;
}

class Asn1SccOptionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit Asn1SccOptionWidget(QWidget *parent = nullptr);
    ~Asn1SccOptionWidget();

    void setCompiler(const QString &compiler);
    QString compiler() const;

    void setParameter(const QString &parameter);
    QString parameter() const;

private:
    Ui::Asn1SccOptionWidget *ui;
};

}

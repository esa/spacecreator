#ifndef MSCINSTANCE_H
#define MSCINSTANCE_H

#include <QString>
#include <QStringList>
#include <QObject>

namespace msc {

class MscInstance : public QObject
{
    Q_OBJECT
public:
    explicit MscInstance(QObject *parent = nullptr);
    MscInstance(const QString &name, QObject *parent = nullptr);

    const QString &name() const;
    void setName(const QString &name);

    const QString &kind() const;
    void setKind(const QString &kind);

    const QStringList &decomposition() const;
    void setDecomposition(const QStringList &decomposition);

Q_SIGNALS:
    void nameChanged(const QString &name);
    void kindChanged(const QString &kind);
    void decompositionChanged(const QStringList &decomposition);

private:
    QString m_name;
    QString m_kind;
    QStringList m_decomposition;
};

} // namespace msc

#endif // MSCINSTANCE_H

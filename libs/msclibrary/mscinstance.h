#ifndef MSCINSTANCE_H
#define MSCINSTANCE_H

#include <QString>
#include <QStringList>

namespace msc {

class MscInstance
{
public:
    MscInstance(const QString &name = "");

    const QString &name() const;
    void setName(const QString &name);

    const QString &kind() const;
    void setKind(const QString &kind);

    const QStringList &decomposition() const;
    void setDecomposition(const QStringList &decomposition);

private:
    QString m_name;
    QString m_kind;
    QStringList m_decomposition;
};

} // namespace msc

#endif // MSCINSTANCE_H

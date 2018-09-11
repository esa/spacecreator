#ifndef MSCINSTANCE_H
#define MSCINSTANCE_H

#include <QString>

namespace msc {

class MscInstance
{
public:
    MscInstance(const QString &name = "");

    const QString &name() const;
    void setName(const QString &name);

private:
    QString m_name;
};

} // namespace msc

#endif // MSCINSTANCE_H

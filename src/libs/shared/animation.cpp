#include "animation.h"

#include <QPropertyAnimation>

namespace shared {
namespace Animation {

QPropertyAnimation *createLinearAnimation(
        QObject *target, const QString &propName, const QVariant &from, const QVariant &to, const int durationMs)
{
    QPropertyAnimation *anim = new QPropertyAnimation(target, propName.toUtf8());
    anim->setDuration(durationMs);
    anim->setEasingCurve(QEasingCurve::Linear);

    anim->setStartValue(from);
    anim->setEndValue(to);

    return anim;
}

}
}

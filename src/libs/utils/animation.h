#pragma once

class QObject;
class QString;
class QVariant;
class QPropertyAnimation;

namespace utils {
namespace Animation {

QPropertyAnimation* createLinearAnimation(QObject* target, const QString& propName, const QVariant& from, const QVariant& to, const int durationMs);

}
}

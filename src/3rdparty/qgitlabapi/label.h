#ifndef LABEL_H
#define LABEL_H

#include "QGitlabAPI_global.h"

#include <QColor>
#include <QJsonObject>
#include <QString>

class QGITLABAPI_EXPORT Label
{
public:
    Label();
    Label(const QJsonObject &label);

    int mLabelId;
    QString mName;
    QString mDescription;
    QColor mColor;
};

#endif // LABEL_H

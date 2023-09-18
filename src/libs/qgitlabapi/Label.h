#ifndef LABEL_H
#define LABEL_H

#include <QJsonObject>
#include <QString>

#include "QGitlabAPI_global.h"

class QGITLABAPI_EXPORT Label
{
public:
    Label();
    Label(const QJsonObject &label);

    QString mName;
    QString mDescription;


};

#endif // LABEL_H

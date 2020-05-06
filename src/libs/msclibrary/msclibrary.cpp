#include "msclibrary.h"

#include "mscchart.h"
#include "msccomment.h"
#include "mscdocument.h"
#include "mscentity.h"
#include "mscinstance.h"
#include "mscinstanceevent.h"
#include "mscmessagedeclaration.h"
#include "mscmodel.h"

static void init_msc_library()
{
    Q_INIT_RESOURCE(mscresources);
}

namespace msc {

/**
   Initializes the library resources and Qt meta types.
 */
void initMscLibrary()
{
    init_msc_library();

    qRegisterMetaType<QList<msc::MscMessageDeclaration *>>("QList<msc::MscMessageDeclaration*>");
    qRegisterMetaType<QVector<msc::MscChart *>>("QVector<msc::MscChart*>");
    qRegisterMetaType<QVector<msc::MscComment *>>("QVector<msc::MscComment*>");
    qRegisterMetaType<QVector<msc::MscDocument *>>("QVector<msc::MscDocument*>");
    qRegisterMetaType<QVector<msc::MscEntity *>>("QVector<msc::MscEntity*>");
    qRegisterMetaType<QVector<msc::MscInstance *>>("QVector<msc::MscInstance*>");
    qRegisterMetaType<msc::MscDocument::HierarchyType>("HierarchyType");
    qRegisterMetaType<msc::MscInstanceEvent *>("msc::MscInstanceEvent*");
    qRegisterMetaType<msc::MscMessageDeclaration *>("msc::MscMessageDeclaration*");
}

}

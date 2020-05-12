#include "sharedlibrary.h"

#include <qglobal.h>

static void init_shared_library()
{
    Q_INIT_RESOURCE(sharedresources);
}

namespace shared {

/**
   Initializes the library resources and Qt meta types.
 */
void initSharedLibrary()
{
    init_shared_library();
}

}

TEMPLATE = lib
TARGET = mscwidgets

include(../../esa.pri)
include(../../depend_asn1library.pri)
include(../../depend_msclibrary.pri)

CONFIG += static

QT += core gui widgets

HEADERS += \
    baseitems/common/abstractinteractiveobject.h \
    baseitems/common/arrowsign.h \
    baseitems/common/drawrectinfo.h \
    baseitems/common/highlightrectitem.h \
    baseitems/common/objectanchor.h \
    baseitems/common/objectslink.h \
    baseitems/common/utils.h \
    baseitems/arrowitem.h \
    baseitems/commentitem.h \
    baseitems/grippoint.h \
    baseitems/grippointshandler.h \
    baseitems/instanceenditem.h \
    baseitems/instanceheaditem.h \
    baseitems/interactiveobject.h \
    baseitems/labeledarrowitem.h \
    baseitems/msgidentificationitem.h \
    baseitems/objectslinkitem.h \
    baseitems/textitem.h \
    commands/cmdchangeinstanceorder.h \
    commands/cmdchangeinstanceposition.h \
    commands/cmdchartitemchangegeometry.h \
    commands/cmdcommentitemchangegeometry.h \
    commands/cmdcoregionitemcreate.h \
    commands/cmdsetasn1file.h \
    commands/cmdsetmessagedeclarations.h \
    commands/common/commandids.h \
    commands/common/commandsfactory.h \
    commands/common/commandsstack.h \
    commands/basecommand.h \
    commands/cmdactioninformaltext.h \
    commands/cmdactionitemcreate.h \
    commands/cmdactionitemmove.h \
    commands/cmdconditionitemcreate.h \
    commands/cmdconditionitemmove.h \
    commands/cmddeleteentity.h \
    commands/cmdentitycommentchange.h \
    commands/cmdentitynamechange.h \
    commands/cmddocumentcreate.h \
    commands/cmddocumentmove.h \
    commands/cmdhierarchytypechange.h \
    commands/cmdinstancestopchange.h \
    commands/cmdinstanceitemcreate.h \
    commands/cmdinstancekindchange.h \
    commands/cmdmessageitemcreate.h \
    commands/cmdmessageitemresize.h \
    commands/cmdpastechart.h \
    commands/cmdsetparameterlist.h \
    commands/cmdtimeritemcreate.h \
    commands/cmdtimeritemmove.h \
    coregionitem.h \
    tools/actioncreatortool.h \
    tools/basecreatortool.h \
    tools/basetool.h \
    tools/commentcreatortool.h \
    tools/conditioncreatortool.h \
    tools/coregioncreatortool.h \
    tools/entitydeletetool.h \
    tools/hierarchycreatortool.h \
    tools/instancecreatortool.h \
    tools/instancestoptool.h \
    tools/messagecreatortool.h \
    tools/pointertool.h \
    tools/timercreatortool.h \
    actionitem.h \
    chartitem.h \
    chartviewmodel.h \
    conditionitem.h \
    documentitem.h \
    graphicsview.h \
    hierarchyviewmodel.h \
    instanceitem.h \
    messagedeclarationsdialog.h \
    messagedialog.h \
    messageitem.h \
    textview.h \
    timeritem.h \
    baseitems/common/coordinatesconverter.h

SOURCES += \
    baseitems/common/arrowsign.cpp \
    baseitems/common/drawrectinfo.cpp \
    baseitems/common/highlightrectitem.cpp \
    baseitems/common/objectanchor.cpp \
    baseitems/common/objectslink.cpp \
    baseitems/common/utils.cpp \
    baseitems/arrowitem.cpp \
    baseitems/commentitem.cpp \
    baseitems/grippoint.cpp \
    baseitems/grippointshandler.cpp \
    baseitems/instanceenditem.cpp \
    baseitems/instanceheaditem.cpp \
    baseitems/interactiveobject.cpp \
    baseitems/labeledarrowitem.cpp \
    baseitems/msgidentificationitem.cpp \
    baseitems/objectslinkitem.cpp \
    baseitems/textitem.cpp \
    commands/cmdchangeinstanceorder.cpp \
    commands/cmdchangeinstanceposition.cpp \
    commands/cmdchartitemchangegeometry.cpp \
    commands/cmdcommentitemchangegeometry.cpp \
    commands/cmdcoregionitemcreate.cpp \
    commands/cmdsetasn1file.cpp \
    commands/cmdsetmessagedeclarations.cpp \
    commands/common/commandsfactory.cpp \
    commands/common/commandsstack.cpp \
    commands/basecommand.cpp \
    commands/cmdactionitemcreate.cpp \
    commands/cmdactioninformaltext.cpp \
    commands/cmdactionitemmove.cpp \
    commands/cmdconditionitemcreate.cpp \
    commands/cmdconditionitemmove.cpp \
    commands/cmddeleteentity.cpp \
    commands/cmdentitycommentchange.cpp \
    commands/cmdentitynamechange.cpp \
    commands/cmddocumentcreate.cpp \
    commands/cmddocumentmove.cpp \
    commands/cmdhierarchytypechange.cpp \
    commands/cmdinstancestopchange.cpp \
    commands/cmdinstanceitemcreate.cpp \
    commands/cmdinstancekindchange.cpp \
    commands/cmdmessageitemcreate.cpp \
    commands/cmdmessageitemresize.cpp \
    commands/cmdpastechart.cpp \
    commands/cmdsetparameterlist.cpp \
    commands/cmdtimeritemcreate.cpp \
    commands/cmdtimeritemmove.cpp \
    coregionitem.cpp \
    tools/actioncreatortool.cpp \
    tools/basecreatortool.cpp \
    tools/basetool.cpp \
    tools/commentcreatortool.cpp \
    tools/conditioncreatortool.cpp \
    tools/coregioncreatortool.cpp \
    tools/entitydeletetool.cpp \
    tools/instancecreatortool.cpp \
    tools/instancestoptool.cpp \
    tools/hierarchycreatortool.cpp \
    tools/messagecreatortool.cpp \
    tools/timercreatortool.cpp \
    tools/pointertool.cpp \
    chartitem.cpp \
    chartviewmodel.cpp \
    actionitem.cpp \
    conditionitem.cpp \
    documentitem.cpp \
    graphicsview.cpp \
    hierarchyviewmodel.cpp \
    instanceitem.cpp \
    messagedeclarationsdialog.cpp \
    messagedialog.cpp \
    messageitem.cpp \
    textview.cpp \
    timeritem.cpp \
    baseitems/common/coordinatesconverter.cpp

FORMS += \
    messagedeclarationsdialog.ui \
    messagedialog.ui

RESOURCES += \
    resources.qrc


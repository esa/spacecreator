TEMPLATE = subdirs

SUBDIRS =

exists($$(QTC_SOURCE)) {
    SUBDIRS += mscplugin
} else {
    message("Your need to define $QTC_SOURCE and $QTC_BUILD variables to build the QtCreator plugin");
}

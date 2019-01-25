include(suppress_antlr_wrn.pri)
include(depend_conf_utils.pri)

ANTLR_CPPRT_PATH = $$PWD/3rdparty/antlr/cpp_runtime
win32: LIBS += -L$$clean_path("$$ANTLR_CPPRT_PATH/runtime/bin/vs-2017/x64/$$BuildType Static") -lantlr4-runtime
#The case of the $$BuildType above differs from the actual one
#(debug|release is used, Debug|Release is on the FS), but it doesn't matter here
else:unix {
    LIBS += -L$$clean_path($$ANTLR_CPPRT_PATH/dist)
    exists($$clean_path($$ANTLR_CPPRT_PATH/dist/libantlr4-runtime.a)) {
        LIBS += -lantlr4-runtime
    } else {
        ANTLR_RT_LIB_OS_TAIL = linux
        osx: ANTLR_RT_LIB_OS_TAIL = macos
        LIBS += -lantlr4-runtime_$${ANTLR_RT_LIB_OS_TAIL}_x64
    }
}

BuildType = Release
CONFIG(debug, debug|release) {
    BuildType = Debug
} # Profiling, etc (if any) are defaulted to Release

ANTLR_CPPRT_PATH = $$PWD/3rdparty/antlr/cpp_runtime
win32: LIBS += -L$$clean_path("$$ANTLR_CPPRT_PATH/runtime/bin/vs-2017/x64/$$BuildType Static") -lantlr4-runtime
else:unix {
    ANTLR_RT_LIB_OS_TAIL = linux
    osx: ANTLR_RT_LIB_OS_TAIL = macos
    LIBS += -L$$clean_path($$ANTLR_CPPRT_PATH/dist) -lantlr4-runtime_$$(ANTLR_RT_LIB_OS_TAIL)_x64
}

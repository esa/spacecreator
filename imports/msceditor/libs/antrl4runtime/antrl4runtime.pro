TEMPLATE = aux

ANTLR_RUNTIME_PATH=$$shell_path("$$_PRO_FILE_PWD_\..\..\3rdparty\antlr\cpp_runtime\runtime")

win32 {
    CONFIG(debug, debug|release) {
        antlrBin.targetBuild = Debug
    } else {
        antlrBin.targetBuild = Release
    }

    antlrBin.target = antlr4-rtl
    antlrBin.vcproj = $$shell_path($$ANTLR_RUNTIME_PATH/antlr4cpp-vs2017.vcxproj)
    #TODO: make it configurable:
    antlrBin.buildTool = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/Bin/msbuild.exe"
    antlrBin.commands = \
    $$shell_quote($$antlrBin.buildTool) \
    $$antlrBin.vcproj /p:PlatformToolset=v141 /p:configuration=$$shell_quote($$antlrBin.targetBuild Static) /p:platform=x64

    QMAKE_EXTRA_TARGETS += antlrBin
    PRE_TARGETDEPS += $$antlrBin.target
}

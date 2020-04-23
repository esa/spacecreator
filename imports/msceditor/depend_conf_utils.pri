BuildType = release
CONFIG(debug, debug|release) {
    BuildType = debug
} # Profiling, etc (if any) are defaulted to release

LibSuffix = a
LibPrefix = lib

win32:!win32-g++ {
    LibSuffix = lib
    LibPrefix = ""
}

# Variables for the build system
$VC_PATH = "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC"
$VC_BIN_PATH = "$VC_PATH/Tools/MSVC/14.16.27023/bin/HostX64/x64"
$QT_DIR = "C:/Qt/5.14.2/msvc2017_64"

# Set the variables for visual studio builds
pushd $VC_PATH'/Auxiliary/Build'
cmd /c "vcvars64.bat&set" |
foreach {
  if ($_ -match "=") {
    $i = $_.indexof("=")
    $k = $_.substring(0, $i)
    $v = $_.substring($i + 1)
    set-item -force -path "ENV:\$k"  -value "$v"
  }
}
popd

# Create the build directory
mkdir build_win
cd build_win

# Run CMake
cmake -GNinja -DCMAKE_CXX_COMPILER="$VC_BIN_PATH/cl.exe" -DCMAKE_C_COMPILER="$VC_BIN_PATH/cl.exe" -DCMAKE_PREFIX_PATH="$QT_DIR" -DQT_QMAKE_EXECUTABLE="$QT_DIR/bin/qmake.exe" ..
# Build it
if( $LASTEXITCODE -eq 0 ) {
    ninja
}

exit $LASTEXITCODE

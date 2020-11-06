# Visual Studio installation path
$VC_PATH = "C:/BuildTools/VC"

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

# Variables for the build system
$VC_BIN_PATH = "$VC_PATH/Tools/MSVC/$Env:VCToolsVersion/bin/Hostx64/x64"

# Create the build directory
mkdir build_win
cd build_win

# Run CMake
cmake -GNinja -DCMAKE_PREFIX_PATH="$Env:QTDIR" -DQT_QMAKE_EXECUTABLE="$Env:QTDIR/bin/qmake.exe" ..
# Build it
if( $LASTEXITCODE -eq 0 ) {
    ninja
}

exit $LASTEXITCODE

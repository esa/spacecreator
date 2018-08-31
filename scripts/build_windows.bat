@echo off

REM build MSC editor using VisualStudio 2017 and Qt 5.11.1

set PROGFILES=%ProgramFiles%
if not "%ProgramFiles(x86)%" == "" set PROGFILES=%ProgramFiles(x86)%

REM Check if Visual Studio 2017 is installed
set MSVCDIR="%PROGFILES%\Microsoft Visual Studio\2017"
set VCVARSALLPATH="%PROGFILES%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
if exist %MSVCDIR% (
  if exist %VCVARSALLPATH% (
   	set COMPILER_VER="2017"
    echo Using Visual Studio 2017
  ) else (
    echo Error: Did not find Visual Studio 2017
    exit /b 2
  )
)

REM Check if Visual Studio 2017 is installed
set QMAKE="C:\Qt\5.11.1\msvc2017_64\bin\qmake.exe"
if exist %QMAKE% (
  echo "Using qmake %QMAKE%"
) else (
  echo "Error: Did not find %QMAKE%"
  exit /b 3
)

REM Build!
echo "%VCVARSALLPATH%"
call %VCVARSALLPATH%

mkdir build_win
cd build_win

call %qmake% ..
call nmake

cd ..

exit /b

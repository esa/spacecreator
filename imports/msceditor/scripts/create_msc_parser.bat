@echo off

REM Creating MSC parser using ANTLR4

SET ANTLR_JAR=%~dp0..\..\..\src\3rdparty\antlr\antlr-4.8-complete.jar
REM Check if ANTLR4 is found
if not exist %ANTLR_JAR% (
    echo Error: Did not find ANTLR4 jar file
    exit /b 2
)
SET CLASSPATH=.;%ANTLR_JAR%;%CLASSPATH%

SET JAVA="C:\Program Files (x86)\Java\jre1.8.0_181\bin\java.exe"
if not exist %JAVA% (
	SET JAVA="C:\Program Files (x86)\Common Files\Oracle\Java\javapath\java.exe"
	if not exist %JAVA% (
		SET JAVA=java.exe
	)
)

echo %cd%
cd %~dp0
cd ..
cd libs\msclibrary\parser
echo %cd%

@echo on

if not exist MscParser.cpp (
    echo "Generating parser from grammar"
    %JAVA% org.antlr.v4.Tool -Dlanguage=Cpp -no-listener -visitor Msc.g4
    exit /b
)

For /F "Delims=" %%I In ('dir /b /OD Msc.g4 MscParser.cpp ^| more +1') Do Set _Newer=%%I
echo %_Newer%
if %_Newer%==Msc.g4 (
    echo "Updateing parser from grammar"
    %JAVA% org.antlr.v4.Tool -Dlanguage=Cpp -no-listener -visitor Msc.g4
    exit /b
)

echo "No need to generate the parser. It's up to date"

exit /b

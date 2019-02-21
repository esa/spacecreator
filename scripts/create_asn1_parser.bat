@echo off

REM Creating ASN parser using ANTLR4

SET ANTLR_JAR=%~dp0..\3rdparty\antlr\antlr-4.7.1-complete.jar
REM Check if ANTLR4 is found
if not exist %ANTLR_JAR% (
    echo Error: Did not find ANTLR4 jar file
    exit /b 2
)
SET CLASSPATH=.;%ANTLR_JAR%;%CLASSPATH%

SET JAVA="C:\Program Files\Java\jre1.8.0_191\bin\java.exe"
REM Check if Java is found
if not exist %JAVA% (
    SET JAVA="C:\Program Files (x86)\Common Files\Oracle\Java\javapath\java.exe"
    if not exist %JAVA% (
        echo Error: Did not find Java - using path
        SET JAVA=java.exe
    )
)

echo %cd%
cd %~dp0
cd ..
cd libs\asn1editor\parser
echo %cd%

@echo on

if not exist ASNParser.cpp (
    echo "Generating parser from grammar"
    %JAVA% org.antlr.v4.Tool -Dlanguage=Cpp -no-listener -visitor ASN.g4
    exit /b
)

For /F "Delims=" %%I In ('dir /b /OD ASN.g4 ASNParser.cpp ^| more +1') Do Set _Newer=%%I
echo %_Newer%
if %_Newer%==ASN.g4 (
    echo "Updateing parser from grammar"
    %JAVA% org.antlr.v4.Tool -Dlanguage=Cpp -no-listener -visitor ASN.g4
    exit /b
)

echo "No need to generate the parser. It's up to date"

exit /b

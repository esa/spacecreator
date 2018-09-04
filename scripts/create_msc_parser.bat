@echo off

REM Creating MSC parser using ANTLR4

SET ANTLR_JAR=%~dp0..\3rdparty\antlr\antlr-4.7.1-complete.jar
REM Check if ANTLR4 is found
if not exist %ANTLR_JAR% (
    echo Error: Did not find ANTLR4 jar file
    exit /b 2
)
SET CLASSPATH=.;%ANTLR_JAR%;%CLASSPATH%

SET JAVA="C:\Program Files (x86)\Java\jre1.8.0_181\bin\java.exe"
REM Check if Java is found
if not exist %JAVA% (
    SET JAVA="C:\Program Files (x86)\Common Files\Oracle\Java\javapath"
    if not exist %JAVA% (
        echo Error: Did not find Java - using path
        SET JAVA=java.exe
    )
)

echo %cd%
cd %~dp0
cd ..
cd libs\msclibrary\parser
echo %cd%
%JAVA% org.antlr.v4.Tool -Dlanguage=Cpp Msc.g4

exit /b

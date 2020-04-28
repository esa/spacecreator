#!/bin/sh

ANTLR_JAR=`pwd`/3rdparty/antlr/antlr-4.8-complete.jar

export CLASSPATH=".:$ANTLR_JAR:$CLASSPATH"
alias antlr4='java -jar $ANTLR_JAR'

cd libs/msclibrary/parser

# Was the file generated already?
if [ ! -f MscParser.h ]; then
    echo "Need to build the parser files for the first time"
    antlr4 -Dlanguage=Cpp -no-listener -visitor Msc.g4
fi

# If the grammar is newer than the generated file
if [ Msc.g4 -nt MscParser.h ]; then
    echo "Need to update build the parser files"
    antlr4 -Dlanguage=Cpp -no-listener -visitor Msc.g4
fi

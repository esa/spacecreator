#!/bin/sh

ANTLR_JAR=`pwd`/3rdparty/antlr/antlr-4.7.1-complete.jar

export CLASSPATH=".:$ANTLR_JAR:$CLASSPATH"
alias antlr4='java -jar $ANTLR_JAR'

cd libs/msclibrary/parser

antlr4 -Dlanguage=Cpp -no-listener -visitor Msc.g4

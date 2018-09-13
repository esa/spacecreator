// Define a grammar called Msc
grammar Msc;

file : document | msc;

document : DOCUMENTKEY IDENTIFIER ';' (document|msc)* DOCUMENTENDKEY ';';
DOCUMENTKEY : [mM][sS][cC][dD][oO][cC][uU][mM][eE][nN][tT]; // ignore case sensitivity
DOCUMENTENDKEY : [eE][nN][dD][mM][sS][cC][dD][oO][cC][uU][mM][eE][nN][tT]; // ignore case sensitivity

msc : MSCKEY IDENTIFIER ';' inst? instance* MSCENDKEY ';';
MSCKEY : [mM][sS][cC]; // ignore case sensitivity
MSCENDKEY : [eE][nN][dD][mM][sS][cC]; // ignore case sensitivity

inst : 'inst' instElements ';';
instElements : instName (',' instElements)?;
instName : IDENTIFIER;

instance : INSTANCEKEY IDENTIFIER instanceHeader? ';' message* INSTANCEENDKEY ';';
INSTANCEKEY : [iI][nN][sS][tT][aA][nN][cC][eE];
INSTANCEENDKEY : [eE][nN][dD][iI][nN][sS][tT][aA][nN][cC][eE];
instanceHeader : ':' instancekind decomposition*;
instancekind : IDENTIFIER;
decomposition : IDENTIFIER;

message :  (inMessage|outMessage) ';';
inMessage : INMESSAGEKEY IDENTIFIER FROMMESSAGEKEY messageTarget;
outMessage : OUTMESSAGEKEY IDENTIFIER TOMESSAGEKEY messageTarget;
INMESSAGEKEY : [iI][nN];
OUTMESSAGEKEY : [oO][uU][tT];
FROMMESSAGEKEY : [fF][rR][oO][mM];
TOMESSAGEKEY : [tT][oO];
messageTarget : IDENTIFIER;

IDENTIFIER : Word;
fragment Word : Nondigit (Nondigit|Digit)*;

fragment Nondigit : [a-zA-Z_.];

fragment Digit : [0-9];

WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines

BLOCKCOMMENT : '/*' .*? '*/' -> skip;

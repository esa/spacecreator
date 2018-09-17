// Define a grammar called Msc
grammar Msc;

file : document | msc;

document : documentHead (document|msc)* DOCUMENTENDKEY ';';
documentHead : DOCUMENTKEY IDENTIFIER ';';
DOCUMENTKEY : [mM][sS][cC][dD][oO][cC][uU][mM][eE][nN][tT]; // ignore case sensitivity
DOCUMENTENDKEY : [eE][nN][dD][mM][sS][cC][dD][oO][cC][uU][mM][eE][nN][tT]; // ignore case sensitivity

msc : mscHead inst? mscBody MSCENDKEY ';';
mscHead: MSCKEY IDENTIFIER ';';
MSCKEY : [mM][sS][cC]; // ignore case sensitivity
MSCENDKEY : [eE][nN][dD][mM][sS][cC]; // ignore case sensitivity

mscBody : instance*;

inst : 'inst' instElements ';';
instElements : instName (',' instElements)?;
instName : IDENTIFIER;

instance : INSTANCEKEY IDENTIFIER instanceHeader? ';' messageEvent* INSTANCEENDKEY ';';
INSTANCEKEY : [iI][nN][sS][tT][aA][nN][cC][eE];
INSTANCEENDKEY : [eE][nN][dD][iI][nN][sS][tT][aA][nN][cC][eE];
instanceHeader : ':' instancekind decomposition*;
instancekind : IDENTIFIER;
decomposition : IDENTIFIER;

messageEvent :  (messageOutput|messageInput) ';';
messageInput : INMESSAGEKEY IDENTIFIER FROMMESSAGEKEY inputAddress;
messageOutput : OUTMESSAGEKEY IDENTIFIER TOMESSAGEKEY outputAddress;
INMESSAGEKEY : [iI][nN];
OUTMESSAGEKEY : [oO][uU][tT];
FROMMESSAGEKEY : [fF][rR][oO][mM];
TOMESSAGEKEY : [tT][oO];
inputAddress : IDENTIFIER;
outputAddress : IDENTIFIER;

IDENTIFIER : Word;
fragment Word : Nondigit (Nondigit|Digit)*;

fragment Nondigit : [a-zA-Z_.];

fragment Digit : [0-9];

WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines
LB : '/\r' ->skip; // linebreak

BLOCKCOMMENT : '/*' .*? '*/' -> skip;

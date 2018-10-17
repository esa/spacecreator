// Define a grammar called Msc
grammar Msc;

tokens {
    NAMEORENV,
    MSCFILE
}

file : mscDocument | mscDefinition;

mscDocument
    : MSCDOCUMENT NAME SEMI (mscDocument | mscDefinition)* ENDMSCDOCUMENT SEMI definingMscReference*
    ;

definingMscReference
    : REFERENCE virtuality? NAME
    ;

virtuality
    : VIRTUAL | REDEFINED | FINALIZED
    ;

mscDefinition
    : MSC NAME SEMI instance* ENDMSC SEMI
        |       LANGUAGE NAME SEMI
        |       DATA FILENAME SEMI
        |       INST NAME SEMI
        |       MSG NAME COLON VARIABLEVALUE SEMI
    ;

instance
    : INSTANCE NAME SEMI mscevent* ENDINSTANCE SEMI //-> (INSTANCE NAME mscevent*)
        |       NAME COLON INSTANCE SEMI mscevent* ENDINSTANCE SEMI //-> (INSTANCE NAME mscevent*)
        |       GATE (IN|OUT) NAME VARIABLEVALUE? (TO|FROM) NAME SEMI
    ;

mscevent
    : CONDITION NAME (SHARED ALL)? SEMI
    | IN NAME VARIABLEVALUE? FROM a=nameOrEnv SEMI                 //-> (IN NAME VARIABLEVALUE? (NAMEORENV $a))
    | OUT NAME VARIABLEVALUE? TO c=nameOrEnv ( VIA d=NAME )? SEMI  //-> (OUT NAME VARIABLEVALUE? (NAMEORENV $c) (VIA $d)? )
    ;

nameOrEnv
    : NAME
    | ENV
    ;


/*Keywords*/

ACTION:'action'|'ACTION';
AFTER:'after'|'AFTER';
ALL:'all'|'ALL';
ALT:'alt'|'ALT';
AS:'as'|'AS';
BEFORE:'before'|'BEFORE';
BEGIN:'begin'|'BEGIN';
BOTTOM:'bottom'|'BOTTOM';
CALL:'call'|'CALL';
COMMENT:'comment'|'COMMENT';
CONCURRENT:'concurrent'|'CONCURRENT';
CONDITION:'condition'|'CONDITION';
CONNECT:'connect'|'CONNECT';
CREATE:'create'|'CREATE';
DATA:'data'|'DATA';
DECOMPOSED:'decomposed'|'DECOMPOSED';
DEF:'def'|'DEF';
EMPTY:'empty'|'EMPTY';
END:'end'|'END';
ENDCONCURRENT:'endconcurrent'|'ENDCONCURRENT';
ENDEXPR:'endexpr'|'ENDEXPR';
ENDINSTANCE:'endinstance'|'ENDINSTANCE';
ENDMETHOD:'endmethod'|'ENDMETHOD';
ENDMSC:'endmsc'|'ENDMSC';
ENDMSCDOCUMENT:'endmscdocument'|'ENDMSCDOCUMENT';
ENDSUSPENSION:'endsuspension'|'ENDSUSPENSION';
ENV:'env'|'ENV';
EQUALPAR:'equalpar'|'EQUALPAR';
ESCAPE:'escape'|'ESCAPE';
EXC:'exc'|'EXC';
EXTERNAL:'external'|'EXTERNAL';
FINAL:'final'|'FINAL';
FINALIZED:'finalized'|'FINALIZED';
FOUND:'found'|'FOUND';
FROM:'from'|'FROM';
GATE:'gate'|'GATE';
IN:'in'|'IN';
INF:'inf'|'INF';
INHERITS:'inherits'|'INHERITS';
INITIAL:'initial'|'INITIAL';
INLINE:'inline'|'INLINE';
INST:'inst'|'INST';
INSTANCE:'instance'|'INSTANCE';
INT_BOUNDARY:'int_boundary'|'INT_BOUNDARY';
LABEL:'label'|'LABEL';
LANGUAGE:'language'|'LANGUAGE';
LOOP:'loop'|'LOOP';
LOST:'lost'|'LOST';
METHOD:'method'|'METHOD';
MSC:'msc'|'MSC';
MSCDOCUMENT:'mscdocument'|'MSCDOCUMENT';
MSG:'msg'|'MSG';
NESTABLE:'nestable'|'NESTABLE';
NONNESTABLE:'nonnestable'|'NONNESTABLE';
OFFSET:'offset'|'OFFSET';
OPT:'opt'|'OPT';
ORIGIN:'origin'|'ORIGIN';
OTHERWISE:'otherwise'|'OTHERWISE';
OUT:'out'|'OUT';
PAR:'par'|'PAR';
PARENTHESIS:'parenthesis'|'PARENTHESIS';
RECEIVE:'receive'|'RECEIVE';
REDEFINED:'redefined'|'REDEFINED';
REFERENCE:'reference'|'REFERENCE';
RELATED:'related'|'RELATED';
REPLYIN:'replyin'|'REPLYIN';
REPLYOUT:'replyout'|'REPLYOUT';
SEQ:'seq'|'SEQ';
SHARED:'shared'|'SHARED';
STARTTIMER:'starttimer'|'STARTTIMER';
STOP:'stop'|'STOP';
STOPTIMER:'stoptimer'|'STOPTIMER';
SUSPENSION:'suspension'|'SUSPENSION';
//TEXT:'text'|'TEXT';
TIME:'time'|'TIME';
TIMEOUT:'timeout'|'TIMEOUT';
TIMER:'timer'|'TIMER';
TO:'to'|'TO';
TOP:'top'|'TOP';
UNDEF:'undef'|'UNDEF';
USING:'using'|'USING';
UTILITIES:'utilities'|'UTILITIES';
VARIABLES:'variables'|'VARIABLES';
VIA:'via'|'VIA';
VIRTUAL:'virtual'|'VIRTUAL';
WHEN:'when'|'WHEN';
WILDCARDS:'wildcards'|'WILDCARDS';
SEMI : ';';
COLON : ':';
COMMA : ',';
MINUS : '-';


fragment
DECIMALDIGIT : [0-9];

fragment
LETTER : [a-zA-Z];

fragment
NATIONAL : LEFTCURLYBRACKET | VERTICALLINE | RIGHTCURLYBRACKET | OVERLINE | UPWARDARROWHEAD ;

fragment
ALPHANUMERIC : LETTER | DECIMALDIGIT | NATIONAL;

fragment
LEFTSQUAREBRACKET : '[' ;

fragment
RIGHTSQUAREBRACKET : ']' ;

fragment
LEFTCURLYBRACKET : '{' ;

fragment
RIGHTCURLYBRACKET : '}' ;

fragment
VERTICALLINE : '|';

fragment
LEFTOPEN : '(';

fragment
LEFTCLOSED : LEFTSQUAREBRACKET;

fragment
RIGHTOPEN : ')';

fragment
RIGHTCLOSED : RIGHTSQUAREBRACKET;

fragment
ABSTIMEMARK : '@';

fragment
RELTIMEMARK : '&';

fragment
OVERLINE : '~';

fragment
UPWARDARROWHEAD : '^';

fragment
FULLSTOP : '.';

fragment
UNDERLINE : '_';

fragment
QUALIFIERLEFT : '<<';

fragment
QUALIFIERRIGHT : '>>';

fragment
LEFTANGULARBRACKET : '<';

fragment
RIGHTANGULARBRACKET : '>';

fragment
APOSTROPHE : '\'';


OTHERCHARACTER : '?' | '%' | '+' | '-' | '!' | '/' | '*' | '"' | '=';

SPECIAL : ABSTIMEMARK | RELTIMEMARK | LEFTOPEN | RIGHTOPEN | LEFTCLOSED | RIGHTCLOSED | LEFTANGULARBRACKET | RIGHTANGULARBRACKET | '#' | COMMA; //  ';' and ':' were here (ttsiod)

COMPOSITESPECIAL : QUALIFIERLEFT | QUALIFIERRIGHT;

QUALIFIER : QUALIFIERLEFT /* TEXT */ QUALIFIERRIGHT ;

//NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | COMMA )+
//{ if (-1 != $text.IndexOf(',')) { $text = $text.Substring(0, $text.IndexOf(','));}} ;
NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | COMMA )+ ;

FILENAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS )+  ;

//VARIABLEVALUE
//    : ( '(' ( options {greedy=false;} : . )* ')' )?
//    ;
VARIABLEVALUE : '(' NAME ')';

CHARACTERSTRING :
    APOSTROPHE ( ALPHANUMERIC | OTHERCHARACTER | SPECIAL | FULLSTOP | UNDERLINE | ' ' | (APOSTROPHE APOSTROPHE) )* APOSTROPHE ;


COMMENTLOST : '/*' .*? '*/' -> skip;
WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines
LB : '/\r' ->skip; // linebreak

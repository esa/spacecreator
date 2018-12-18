// Define a grammar called Msc
grammar Msc;

tokens {
    NAMEORENV,
    MSCFILE
}

file : mscDocument | mscDefinition;

textDefinition
    : ('text' | 'TEXT') STRING SEMI
    ;

// 3 Message Sequence Chart document

mscDocument
    : MSCDOCUMENT NAME SEMI (mscDocument | mscDefinition | instance)* ENDMSCDOCUMENT SEMI definingMscReference*
    ;

definingMscReference
    : REFERENCE virtuality? NAME
    ;

virtuality
    : VIRTUAL | REDEFINED | FINALIZED
    ;

mscDefinition
    : messageSequenceChart
        |       LANGUAGE NAME SEMI
        |       DATA NAME SEMI
        |       MSG NAME (COMMA NAME)* (COLON LEFTOPEN parameterList RIGHTOPEN)? SEMI
    ;

// 4.1 Message sequence chart

messageSequenceChart
    : MSC NAME SEMI (gateDeclaration)? mscBody ENDMSC SEMI // TODO add head, virtuality and hmsc
    ;

mscBody
    : mscStatement* | instanceDeclStatement*
    ;

mscStatement
    : textDefinition | eventDefinition
    ;

eventDefinition
    : (NAME COLON instanceEventList) // TODO add "| (instanceNameList COLON multiInstanceEventList)"
    ;

instanceEventList
    : METHOD instanceEvent* ENDMETHOD
    | instanceHeadStatement instanceEvent* instanceEndStatement
    ; // TODO add suspension, ...

instanceDeclStatement
    : instance
    ;

//

instance
    : INSTANCE NAME (COLON instanceKind)? (LEFTOPEN parameterList RIGHTOPEN)? SEMI instanceEvent* ENDINSTANCE SEMI
        |       NAME COLON INSTANCE instanceKind? SEMI instanceEvent* ENDINSTANCE SEMI
        |       gateDeclaration
        |       INST NAME (COLON instanceKind)? SEMI
    ;

gateDeclaration
    : GATE (IN|OUT) NAME (COMMA NAME)? (LEFTOPEN parameterList RIGHTOPEN)? (TO|FROM) NAME SEMI
    ;

instanceHeadStatement
    : INSTANCE instanceKind decomposition? SEMI
    ;

instanceEndStatement
    : ENDINSTANCE SEMI
    ;

instanceKind
    : NAME (NAME)*
    ;

instanceEvent
    : (mscEvent | timerStatement | action) SEMI
    ;

// 4.3 Message

mscEvent
    : CONDITION NAME (SHARED ALL)?
    | IN messageIdentification (FROM outputAddress)?
    | OUT messageIdentification (TO inputAddress)?
    ;

messageIdentification
    : NAME (COMMA NAME)? (LEFTOPEN parameterList RIGHTOPEN)?
    ;

outputAddress
    : (instanceName=NAME | ENV) (VIA gateName=NAME)?
    ;

inputAddress
    : (instanceName=NAME | ENV) (VIA gateName=NAME)?
    ;

// 4.8 TIMER

timerStatement
    : startTimer | stopTimer | timeout
    ;

startTimer
    : (STARTTIMER|SET) NAME (COMMA NAME)? duration? (LEFTOPEN parameterList RIGHTOPEN)?
    ;

duration
    : LEFTSQUAREBRACKET durationLimit (COMMA durationLimit)? RIGHTSQUAREBRACKET
    ;

durationLimit
    : NAME /* TODO should be a <expression string> */
    | INF
    ;

stopTimer
    : (STOPTIMER|RESET) NAME (COMMA NAME)?
    ;

timeout
    : TIMEOUT NAME (COMMA NAME)? (LEFTOPEN parameterList RIGHTOPEN)?
    ;

// 4.9 ACTION

action
    : ACTION actionStatement
    ;

actionStatement
    : informalAction | dataStatementList
    ;

informalAction
    : CHARACTERSTRING
    ;

//

parameterList
    : paramaterDefn (',' parameterList)?
    ;

paramaterDefn
    : binding | expression | pattern
    ;

// 5.7 Bindings

binding
    : leftBinding | rightBinding
    ;

leftBinding
    : pattern LEFTBINDSYMBOL expression
    ;

rightBinding
    : expression RIGHTBINDSYMBOL pattern
    ;

expression
    : expressionString
    ;

pattern
    : variableString | wildcard
    ;

wildcard
    : NAME // TODO not correct ?
    ;

//

variableString
    : STRING // TODO not correct ?
    ;

expressionString
    : NAME COLON NAME // TODO not correct ?
    ;

variableValue
    : LEFTOPEN NAME RIGHTOPEN
    ;

decomposition
    : DECOMPOSED substructureReference
    ;

substructureReference
    : AS NAME
    ;

// 5.10 DATA IN ACTION boxes

dataStatementList
    : dataStatement (COMMA dataStatementList)?
    ;
dataStatement
    : defineStatement | undefineStatement | binding
    ;
defineStatement
    : DEF variableString
    ;
undefineStatement
    : UNDEF variableString
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
RESET : 'reset'|'RESET'; // From the old standard - now it's stoptimer
SEQ:'seq'|'SEQ';
SET : 'set'|'SET'; // From the old standard - now it's startptimer
SHARED:'shared'|'SHARED';
STARTTIMER:'starttimer'|'STARTTIMER';
STOP:'stop'|'STOP';
STOPTIMER:'stoptimer'|'STOPTIMER';
SUSPENSION:'suspension'|'SUSPENSION';
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
LEFTOPEN : '(';
RIGHTOPEN : ')';
LEFTSQUAREBRACKET : '[' ;
RIGHTSQUAREBRACKET : ']' ;


fragment
DECIMALDIGIT : [0-9];

fragment
LETTER : [a-zA-Z];

fragment
NATIONAL : LEFTCURLYBRACKET | VERTICALLINE | RIGHTCURLYBRACKET | OVERLINE | UPWARDARROWHEAD ;

fragment
ALPHANUMERIC : LETTER | DECIMALDIGIT | NATIONAL;

fragment
LEFTCURLYBRACKET : '{' ;

fragment
RIGHTCURLYBRACKET : '}' ;

fragment
VERTICALLINE : '|';

fragment
LEFTCLOSED : LEFTSQUAREBRACKET;

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

LEFTBINDSYMBOL : ':' '='?; // ':=' TODO should be ':=' only

RIGHTBINDSYMBOL : '=:';

CHARACTERSTRING :
    APOSTROPHE ( ALPHANUMERIC | OTHERCHARACTER | SPECIAL | FULLSTOP | UNDERLINE | ' ' | (APOSTROPHE APOSTROPHE) )* APOSTROPHE ;

fragment
TEXT : ( ALPHANUMERIC | OTHERCHARACTER | SPECIAL | FULLSTOP | UNDERLINE | ' ' | APOSTROPHE )*;

MISC : OTHERCHARACTER | APOSTROPHE;

OTHERCHARACTER : '?' | '%' | '+' | '-' | '!' | '*' | '"' | '='; // exclude '/' as it's used for linebreaks

SPECIAL : ABSTIMEMARK | RELTIMEMARK | LEFTOPEN | RIGHTOPEN | LEFTCLOSED | RIGHTCLOSED | LEFTANGULARBRACKET | RIGHTANGULARBRACKET | '#' | COMMA; //  ';' and ':' were here (ttsiod)

COMPOSITESPECIAL : QUALIFIERLEFT | QUALIFIERRIGHT;

fragment
NOTE : '/*' TEXT '*/';

QUALIFIER : QUALIFIERLEFT /* TEXT */ QUALIFIERRIGHT ;

//NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | COMMA )+
//{ if (-1 != $text.IndexOf(',')) { $text = $text.Substring(0, $text.IndexOf(','));}} ;
NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS )+ ;

FILENAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS )+  ;

STRING : '"' (ALPHANUMERIC | SPECIAL | FULLSTOP | UNDERLINE)* '"';

COMMENTSKIPED: ('comment \''|'COMMENT \'') TEXT '\'' -> skip;
COMMENTLOST : '/*' .*? '*/' -> channel(2);
WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines
LB : ('/\r'|'/\n') ->skip; // linebreak

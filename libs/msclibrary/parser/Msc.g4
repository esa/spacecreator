// Define a grammar called Msc
grammar Msc;

// TODO: It would be good
// a) to activate the named actions below only on Windows;
// b) to incorparate a single action for all names to reduce copy-n-paste here.

// Parser

@parser::preinclude{
#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif // SUPPRESS_ANTLR_WRNS_WINDOWS
}
@parser::postinclude{
#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(pop)
#endif // SUPPRESS_ANTLR_WRNS_WINDOWS
}

// BaseVisitor

@parser::basevisitorpreinclude{
#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif // SUPPRESS_ANTLR_WRNS_WINDOWS
}
@parser::basevisitorpostinclude{
#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(pop)
#endif // SUPPRESS_ANTLR_WRNS_WINDOWS
}

// Visitor

@parser::visitorpreinclude{
#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif // SUPPRESS_ANTLR_WRNS_WINDOWS
}
@parser::visitorpostinclude{
#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(pop)
#endif // SUPPRESS_ANTLR_WRNS_WINDOWS
}

// Lexer

@lexer::preinclude{
#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif // SUPPRESS_ANTLR_WRNS_WINDOWS
}
@lexer::postinclude{
#ifdef SUPPRESS_ANTLR_WRNS_WINDOWS
    #pragma warning(pop)
#endif // SUPPRESS_ANTLR_WRNS_WINDOWS
}

tokens {
    NAMEORENV,
    MSCFILE
}

file : mscDocument | mscDefinition;

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
    : MSC NAME SEMI instance* ENDMSC SEMI
        |       LANGUAGE NAME SEMI
        |       DATA NAME SEMI
        |       MSG NAME COLON variableValue SEMI
    ;

instance
    : INSTANCE NAME (COLON instanceKind)? (LEFTOPEN parameterList RIGHTOPEN)? SEMI instanceEvent* ENDINSTANCE SEMI
        |       NAME COLON INSTANCE instanceKind? SEMI instanceEvent* ENDINSTANCE SEMI
        |       GATE (IN|OUT) NAME (COMMA NAME)? (LEFTOPEN parameterList RIGHTOPEN)? (TO|FROM) NAME SEMI
        |       INST NAME (COLON instanceKind)? SEMI
    ;

instanceKind
    : NAME+
    ;

instanceEvent
    : (mscEvent | timerStatement) SEMI
    ;

mscEvent
    : CONDITION NAME (SHARED ALL)?
    | IN NAME variableValue? (FROM a=nameOrEnv)?
    | OUT NAME variableValue? (TO c=nameOrEnv)? ( VIA d=NAME )?
    ;

nameOrEnv
    : NAME | ENV
    ;

timerStatement
    : startTimer | stopTimer | timeout
    ;

startTimer
    : STARTTIMER NAME (COMMA NAME)? duration? (LEFTOPEN parameterList RIGHTOPEN)?
    ;

duration
    : LEFTSQUAREBRACKET durationLimit (COMMA durationLimit)? RIGHTSQUAREBRACKET
    ;

durationLimit
    : NAME /* TODO should be a <expression string> */
    | INF
    ;

stopTimer
    : STOPTIMER NAME (COMMA NAME)?
    ;

timeout
    : TIMEOUT NAME (COMMA NAME)? (LEFTOPEN parameterList RIGHTOPEN)?
    ;

parameterList
    : NAME (',' parameterList)?
    ;

variableValue
    : LEFTOPEN NAME RIGHTOPEN
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

CHARACTERSTRING :
    APOSTROPHE ( ALPHANUMERIC | OTHERCHARACTER | SPECIAL | FULLSTOP | UNDERLINE | ' ' | (APOSTROPHE APOSTROPHE) )* APOSTROPHE ;

fragment
TEXT : ( ALPHANUMERIC | OTHERCHARACTER | SPECIAL | FULLSTOP | UNDERLINE | ' ' | APOSTROPHE )*;

MISC : OTHERCHARACTER | APOSTROPHE;

OTHERCHARACTER : '?' | '%' | '+' | '-' | '!' | '/' | '*' | '"' | '=';

SPECIAL : ABSTIMEMARK | RELTIMEMARK | LEFTOPEN | RIGHTOPEN | LEFTCLOSED | RIGHTCLOSED | LEFTANGULARBRACKET | RIGHTANGULARBRACKET | '#' | COMMA; //  ';' and ':' were here (ttsiod)

COMPOSITESPECIAL : QUALIFIERLEFT | QUALIFIERRIGHT;

fragment
NOTE : '/*' TEXT '*/';

QUALIFIER : QUALIFIERLEFT /* TEXT */ QUALIFIERRIGHT ;

//NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | COMMA )+
//{ if (-1 != $text.IndexOf(',')) { $text = $text.Substring(0, $text.IndexOf(','));}} ;
NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS )+ ;

FILENAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS )+  ;

COMMENTSKIPED: ('comment \''|'COMMENT \'') TEXT '\'' -> skip;
COMMENTLOST : NOTE -> skip;
WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines
LB : '/\r' ->skip; // linebreak

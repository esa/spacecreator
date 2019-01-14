// Define a grammar called Msc
// Specs from
// https://www.itu.int/rec/dologin_pub.asp?lang=e&id=T-REC-Z.120-201102-I!!PDF-E&type=items

grammar Msc;

tokens {
    NAMEORENV,
    MSCFILE
}

file : mscDocument | mscDefinition;

// 2.1
name
    // : NAME
    : NAME+ // name in the 1993 spec is more relaxed
    ;

// 2.3 Comment

end
    : (comment)? SEMI
    ;

comment
    : COMMENT CHARACTERSTRING
    ;

textDefinition
    : ('text' | 'TEXT') STRING end
    ;

// 3 Message Sequence Chart document

mscDocument
    : documentHead (containingClause | mscDocument | mscDefinition)* ENDMSCDOCUMENT end definingMscReference*
    | MSCDOCUMENT NAME REFERENCED end // not in the spec
    ;

documentHead
    : MSCDOCUMENT NAME end dataDefinition
    ;

definingMscReference
    : REFERENCE virtuality? NAME
    ;

virtuality
    : VIRTUAL | REDEFINED | FINALIZED
    ;

containingClause
    : (INST instanceItem)+
    ;
instanceItem
    : instanceName=NAME (COLON instanceKind)? (inheritance)? (decomposition)? (dynamicDeclList | end)
    ;
inheritance
    : INHERITS instanceKind
    ;
messageDeclClause
    : (MSG messageDecl end)*
    ;

mscDefinition
    : messageSequenceChart
        | MSG NAME (COMMA NAME)* (COLON LEFTOPEN parameterList RIGHTOPEN)? end
    ;

identifier
    : (QUALIFIER)? NAME
    ;

// 4.1 Message sequence chart

messageSequenceChart
    : virtuality? MSC mscHead mscBody ENDMSC end // TODO add hmsc
    ;

mscHead
    : NAME (mscParameterDecl)? end (mscInstInterface)? mscGateInterface
    ;

mscParameterDecl
    : LEFTOPEN mscParmDeclList RIGHTOPEN
    ;
mscParmDeclList
    : mscParmDeclBlock (end mscParameterDecl)?
    ;
mscParmDeclBlock
    : dataParameterDecl
    | instanceParameterDecl
    | messageParameterDecl
    | timerParameterDecl
    ;

instanceParameterDecl
    : INST instanceParmDeclList
    ;
instanceParmDeclList
    : NAME (COLON instanceKind)? (COMMA instanceParmDeclList)?
    ;

messageParameterDecl
    : MSG messageParmDeclList
    ;
messageParmDeclList
    : messageDeclList
    ;

timerParameterDecl
    : TIMER timerParmDeclList
    ;
timerParmDeclList
    : timerDeclList
    ;

mscInstInterface
    : containingClause
    ;
instanceKind
    : NAME (NAME)*
    ;

mscGateInterface
    : mscGateDef*
    ;
mscGateDef
    : GATE msgGate end // TODO (| methodCallGate | replyGate| createGate | orderGate) end
    ;
msgGate
    : defInGate | defOutGate
    ;

mscBody
    : mscStatement* | instanceDeclStatement*
    ;
mscStatement
    : textDefinition | eventDefinition
    ;

eventDefinition
    : (instanceName=NAME COLON instanceEventList) | (instanceNameList COLON multiInstanceEventList)
    ;

instanceEventList
    : (startMethod instanceEvent* endMethod)
    | (startSuspension instanceEvent* endSuspension)
    | (startCoregion instanceEvent* endCoregion)
    | (instanceHeadStatement instanceEvent* (instanceEndStatement | stop | (stop instanceEndStatement)))
    ;
instanceDeclStatement
    : instanceHeadStatement (orderableEvent | nonOrderableEvent)* (instanceEndStatement | stop | (stop instanceEndStatement))
    ;
instanceEvent
    : orderableEvent
    | nonOrderableEvent
    ;

orderableEvent
    : (LABEL eventName=NAME end)? (messageEvent
//    | incompleteMessageEvent
    | create | timerStatement | action
    | coregion) // Not like in the standard and this does not prevet recursive coregion statements
//    | methodCallEvent | incompleteMethodCallEvent | create | timerStatement | action)
//        (BEFORE orderDestList)? (AFTER orderDestList)?  end (TIME timeDestList end)?
    end
    ;

nonOrderableEvent
    : sharedCondition
//    | sharedMscReference
//    | sharedInlineExpr
    ;

instanceNameList
    : (NAME (COMMA NAME)*) | ALL
    ;

multiInstanceEventList
    : multiInstanceEvent +
    ;
multiInstanceEvent
    : condition // TODO add | mscReference | inlineExpr
    ;

// 4.2 Instance

instanceHeadStatement
    // older standard
    : INSTANCE instanceName=NAME ((COLON)? instanceKind)? (decomposition)? (LEFTOPEN parameterList RIGHTOPEN)? end
    | INSTANCE (instanceKind)? (decomposition)? end
    ;
instanceEndStatement
    : ENDINSTANCE end
    ;

// 4.3 Message

messageEvent
    : messageOutput | messageInput
    ;
messageOutput
    : OUT msgIdentification (TO)? (inputAddress)? // should not be optional according to spec
    ;
messageInput
    : IN msgIdentification (FROM)? (outputAddress)? // should not be optional according to spec
    ;
incompleteMessageEvent
    : incompleteMessageOutput | incompleteMessageInput
    ;
incompleteMessageOutput
    : OUT msgIdentification TO LOST (inputAddress)?
    ;
incompleteMessageInput
    : IN msgIdentification FROM FOUND (outputAddress)?
    ;
msgIdentification
    : messageName=NAME (COMMA messageInstanceName=NAME)? (LEFTOPEN parameterList RIGHTOPEN)?
    (VIA gateName=NAME)? // the via is not accordint ot the spec
    ;
outputAddress
    : (instanceName=NAME | ENV) (VIA gateName=NAME)?
    | VIA gateName=NAME // the via is not accordint ot the spec
    ;
inputAddress
    : (instanceName=NAME | ENV) (VIA gateName=NAME)?
    | VIA gateName=NAME // the via is not accordint ot the spec
    ;

// 4.4 Control Flow

methodCallEvent
    : callOut | callIn |replyOut | replyIn
    ;
callOut
    : CALL msgIdentification TO inputAddress
    ;
callIn
    : RECEIVE msgIdentification FROM outputAddress
    ;
replyOut
    : REPLYOUT msgIdentification TO inputAddress
    ;
replyIn
    : REPLYIN msgIdentification FROM outputAddress
    ;
incompleteMethodCallEvent
    : incompleteCallOut | incompleteCallIn | incompleteReplyOut | incompleteReplyIn
    ;
incompleteCallOut
    : CALL msgIdentification TO LOST (inputAddress)?
    ;
incompleteCallIn
    : RECEIVE msgIdentification FROM FOUND (outputAddress)?
    ;
incompleteReplyOut
    : REPLYOUT msgIdentification TO LOST (inputAddress)?
    ;
incompleteReplyIn
    : REPLYIN msgIdentification FROM FOUND (outputAddress)?
    ;
startMethod
    : METHOD end
    ;
endMethod
    : ENDMETHOD end
    ;
startSuspension
    : SUSPENSION end
    ;
endSuspension
    : ENDSUSPENSION end
    ;

// 4.5 Environment and gates

inputDest
    : (LOST (inputAddress)?) | inputAddress
    ;
outputDest
    : (FOUND (outputAddress)?) | outputAddress
    ;
defInGate
    : (gateName=NAME)?  OUT msgIdentification TO inputDest
    ;
defOutGate
    : (gateName=NAME)?  IN msgIdentification FROM outputDest
    ;

// 4.7 Condition

sharedCondition
    : (shared)? conditionIdentification (shared)? end // TODO second shared should be mandatory by spec?
    ;
conditionIdentification
    : CONDITION conditionText
    ;
conditionText
    : conditionNameList
    | (WHEN (conditionNameList | LEFTOPEN expression RIGHTOPEN))
    | OTHERWISE
    ;
conditionNameList
//    : NAME (COMMA NAME)*
    : name (COMMA conditionNameList)* // to support old spec names as well
    ;
shared
    : SHARED (sharedInstanceList | ALL)
    ;
sharedInstanceList
    : NAME (COMMA sharedInstanceList)?
    ;
condition
    : (shared)? conditionIdentification end
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
    | name
    ;

// 4.10 Instance creation

create
    : CREATE NAME (LEFTOPEN parameterList RIGHTOPEN)?
    ;

// 4.11 Instance stop

stop
    : STOP end
    ;

// 5.2 Syntax interface to external data languages

variableString
    : STRING // TODO not correct ?
    ;
typeRefString
    : STRING
    ;
dataDefinitionString
    : NAME
    ;

// 5.4 Declaring DATA

messageDeclList
    : messageDecl (end messageDeclList)?
    ;
messageDecl
    : messageNameList (COLON LEFTOPEN typeRefList RIGHTOPEN)?
    ;
messageNameList
    : NAME (COMMA messageNameList)?
    ;
timerDeclList
    : timerDecl (end timerDeclList)?
    ;
timerDecl
    : timerNameList (duration)? (COLON LEFTOPEN typeRefList RIGHTOPEN)?
    ;
timerNameList
    : NAME (COMMA timerNameList)?
    ;
typeRefList
    : STRING (COMMA typeRefList)?
    ;
dynamicDeclList
    : VARIABLES variableDeclList end
    ;
variableDeclList
    : variableDeclItem (end variableDeclList)?
    ;
variableDeclItem
    : variableList COLON STRING
    ;
variableList
    : variableString (COMMA variableList)?
    ;
dataDefinition
    : (LANGUAGE dataLanguageName=NAME end)? (wildcardDecl)? (DATA dataDefString=NAME end)?
    ;
wildcardDecl
    : WILDCARDS variableDeclList end
    ;

// 5.5 Static DATA

dataParameterDecl
    : (VARIABLES)?  variableDeclList
    ;
actualDataParameters
    : (VARIABLES)?  actualDataParameterList
    ;
actualDataParameterList
    : expressionString (COMMA actualDataParameterList)?
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
    : . | CHARACTERSTRING // TODO not correct ?
    ;

// 5.8 Data in message and timer parameters

parameterList
    : paramaterDefn (COMMA parameterList)?
    ;

paramaterDefn
    : SEQUENCEOF | binding | expression | pattern
    ;

//

expressionString
    : NAME COLON NAME // TODO not correct ?
    ;

variableValue
    : LEFTOPEN NAME RIGHTOPEN
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

// 7.1 Coregion

startCoregion
    : CONCURRENT end
    ;
endCoregion
    : ENDCONCURRENT end
    ;
coregion // this is not as expected in the standard
    : CONCURRENT | ENDCONCURRENT
    ;

// 7.4 Instance decomposition

decomposition
    : DECOMPOSED substructureReference
    ;

substructureReference
    : AS NAME
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
ENDMSC:'endmsc'|'ENDMSC'|'endsubmsc'|'ENDSUBMSC';
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
MSC:'msc'|'MSC'|'submsc'|'SUBMSC';
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
LEFTCURLYBRACKET : '{';
RIGHTCURLYBRACKET : '}';

// Extra keywords from the old spec
REFERENCED:'referenced'|'REFERENCED';


fragment
DECIMALDIGIT : [0-9];

fragment
LETTER : [a-zA-Z];

fragment
NATIONAL : LEFTCURLYBRACKET | VERTICALLINE | RIGHTCURLYBRACKET | OVERLINE | UPWARDARROWHEAD ;

fragment
ALPHANUMERIC : LETTER | DECIMALDIGIT | NATIONAL;

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

LEFTBINDSYMBOL : ':=';
RIGHTBINDSYMBOL : '=:';

CHARACTERSTRING :
    APOSTROPHE ( ALPHANUMERIC | OTHERCHARACTER | SPECIAL | FULLSTOP | UNDERLINE | ' ' | [\r] | [\n] | (APOSTROPHE APOSTROPHE) )* APOSTROPHE ;

fragment
TEXT : ( ALPHANUMERIC | OTHERCHARACTER | SPECIAL | FULLSTOP | UNDERLINE | ' ' | APOSTROPHE )*;

MISC : OTHERCHARACTER | APOSTROPHE;

OTHERCHARACTER : '?' | '%' | '+' | '-' | '!' | '*' | '"' | '=' | '/';

SPECIAL : ABSTIMEMARK | RELTIMEMARK | LEFTOPEN | RIGHTOPEN | LEFTCLOSED | RIGHTCLOSED | LEFTANGULARBRACKET | RIGHTANGULARBRACKET | '#' | COMMA | SEMI | COLON;

COMPOSITESPECIAL : QUALIFIERLEFT | QUALIFIERRIGHT;

fragment
NOTE : '/*' TEXT '*/';

QUALIFIER : QUALIFIERLEFT /* TEXT */ QUALIFIERRIGHT ;

//NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | COMMA )+
//{ if (-1 != $text.IndexOf(',')) { $text = $text.Substring(0, $text.IndexOf(','));}} ;
//NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS )+ ;

// '`', '/' are not as from the spec
NAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS | '`' | '/' )+ ;

FILENAME : ( LETTER | DECIMALDIGIT | UNDERLINE | FULLSTOP | MINUS )+  ;

STRING : '"' (ALPHANUMERIC | SPECIAL | FULLSTOP | UNDERLINE)* '"';


SEQUENCEOF // custom
//    : LEFTCURLYBRACKET ALPHANUMERIC+ (COMMA ALPHANUMERIC)* RIGHTCURLYBRACKET
    : LEFTCURLYBRACKET ( NAME | ' ' | COMMA)+ RIGHTCURLYBRACKET
    ;


COMMENTLOST : '/*' .*? '*/' -> channel(2);
WS : [ \t\r\n]+ -> skip ; // skip spaces, tabs, newlines
LB : '/' ('\r' | '\n')+ ->skip; // linebreak

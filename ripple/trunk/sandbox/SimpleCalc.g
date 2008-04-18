grammar SimpleCalc;

/*tokens {
	PLUS 	= '+' ;
	MINUS	= '-' ;
	MULT	= '*' ;
	DIV	= '/' ;
}*/

@parser::header {
import java.io.InputStream;
import java.io.FileInputStream;
import org.antlr.runtime.ModifiedANTLRInputStream;
}

@parser::members {
    public static void main(String[] args) throws Exception {
        InputStream is = new FileInputStream(args[0]);

        CharStream cs = new ModifiedANTLRInputStream(is);
        //CharStream cs = new ANTLRFileStream(args[0]);

        SimpleCalcLexer lex = new SimpleCalcLexer(cs);
       	CommonTokenStream tokens = new CommonTokenStream(lex);

        SimpleCalcParser parser = new SimpleCalcParser(tokens);

        try {
            parser.expr();
        } catch (RecognitionException e)  {
            e.printStackTrace();
        }
    }
}

/*------------------------------------------------------------------
 * PARSER RULES
 *------------------------------------------------------------------*/

expr	: term ( (WS)? ( OP_PLUS | OP_OPTIONAL ) (WS)? term )* ;

term	: factor ( (WS)? ( OP_STAR | OP_APPLY_PRE ) (WS)? factor )* ;

factor	: NUMBER ;


/*------------------------------------------------------------------
 * LEXER RULES
 *------------------------------------------------------------------*/

/*options
{
	k = 3;

	// Use custom error recovery.
	defaultErrorHandler = false;
}*/

fragment
WS_CHAR
	: ' ' | '\t' | '\r'
	| '\n'  { /*newline();*/ /*RESTOREME matchEndOfLine();*/ }
	;

WS
	: (WS_CHAR)+
	;

/*
ESC
	: //(('\0'..'\8') | ('\11'..'\12') | ('\14'..'\27'))
	  (('\u0000'..'\u0008') | ('\u000b'..'\u000c') | ('\u000e'..'\u001b'))
		{ matchEscapeCharacter(); $setType(Token.SKIP); }
	;
*/

fragment
HEX
	: ('0'..'9')
	| ('A'..'F')
	;

fragment
SCHARACTER
	: ' ' | '!' | ('#'..'[')  // excludes: '\"', '\\'
	| (']'..'\uFFFE')  // Note: '\u10FFFF' in Turtle
	| '\\u' HEX HEX HEX HEX
	| '\\U' HEX HEX HEX HEX HEX HEX HEX HEX
	| '\\' ('\\' | '\"' | 't' | 'n' | 'r' )
	;

fragment
UCHARACTER
	: (' '..'=') | ('?'..'[')  // excludes: '>', '\\'
	| (']'..'\uFFFE')  // Note: '\u10FFFF' in Turtle
	| '\\u' HEX HEX HEX HEX
	| '\\U' HEX HEX HEX HEX HEX HEX HEX HEX
	| '\\' ('\\' | '>')
	;

fragment
LANGUAGE
	: ( '@'! ('a'..'z')+ ('-' (('a'..'z') | ('0'..'9'))+)* )
//		{ adapter.setLanguageTag( $getText ); }
	;

STRING
	: '\"'!
		{ /* RESTOREME adapter.setLanguageTag( null );*/ }
		( SCHARACTER )* '\"'! ( lang=LANGUAGE! /*RESTOREME {adapter.setLanguageTag( lang.getText() ); }*/ )?
	;

/*
LONG_STRING
	: "\"\"\""!
		{ adapter.setLanguageTag( null ); }
		( SCHARACTER )* "\"\"\""! ( LANGUAGE! )?
	;
*/

URIREF
	: '<'! ( UCHARACTER )* '>'!
	;

fragment
DIGIT
	: ('0' .. '9')
	;

fragment
NAME_START_CHAR_NOUSC
	: ('A' .. 'Z') | ('a' .. 'z')
	| ('\u00C0'..'\u00D6')
	| ('\u00D8'..'\u00F6')
	| ('\u00F8'..'\u02FF')
	| ('\u0370'..'\u037D')
	| ('\u037F'..'\u1FFF')
	| ('\u200C'..'\u200D')
	| ('\u2070'..'\u218F')
	| ('\u2C00'..'\u2FEF')
	| ('\u3001'..'\uD7FF')
	| ('\uF900'..'\uFDCF')
	| ('\uFDF0'..'\uFFFD')
//	| ('\u10000'..'\uEFFFF')
	;

fragment
NAME_CHAR
	: ( NAME_START_CHAR_NOUSC | '_' )
	| '-' | DIGIT
	| '\u00B7'
	| ('\u0300'..'\u036F')
	| ('\u203F'..'\u2040')
	;

// Could be a name or a prefixName.
NAME_OR_PREFIX
	: NAME_START_CHAR_NOUSC ( NAME_CHAR )*
	;

// Can only be a name.
NAME_NOT_PREFIX
	: '_' (NAME_CHAR)*
	;

NODEID_PREFIX : '_:' ;

// Note: the '+' prefix (e.g. in +42) is excluded, as it interferes with the '+'
// operator.
NUMBER
	: ('-' /*| '+'*/)? ( DIGIT )+
		(('.' DIGIT ) => ( '.' ( DIGIT )+ )
		| ()) {System.out.println("found a number");}
	;

// Ignore comments.
COMMENT
	: ( '#' ( ~('\n') )* ) { $channel=HIDDEN; }
	;

/*
MULTI_LINE_COMMENT
	: "(:" ((~':') | (':' ~')'))* ":)"
		{ $setType( Token.SKIP ); }
	;
*/

DOUBLE_HAT : '^^' ;

L_PAREN : '(' ;
R_PAREN : ')' ;

L_BRACKET : '[' ;
R_BRACKET : ']' ;

L_CURLY : '{' ;
R_CURLY : '}' ;

SEMI : ';' ;
PERIOD : '.' ;
COMMA : ',';

COLON : ':' ;

EQUAL : '=';

OP_APPLY_PRE : '/' ;
OP_APPLY_POST : '!' ;
OP_OPTIONAL : '?' ;
OP_STAR : '*';
OP_PLUS : '+';

fragment
DRCTV : '@' ;

DRCTV_COUNT     : DRCTV ( 'count'         | 'c' ) ;
DRCTV_DEFINE    : DRCTV ( 'define'        | 'd' ) ;
DRCTV_EXPORT    : DRCTV ( 'export'        | 'e' ) ;
DRCTV_HELP      : DRCTV ( 'help'          | 'h' ) ;
DRCTV_LIST      : DRCTV ( 'list'          | 'l' ) ;
DRCTV_PREFIX    : DRCTV ( 'prefix'        | 'p' ) ;
DRCTV_QUIT      : DRCTV ( 'quit'          | 'q' ) {System.out.println("TOKEN quit!");} ;
DRCTV_REDEFINE  : DRCTV ( 'redefine'      | 'r' ) ;
DRCTV_UNDEFINE  : DRCTV ( 'undefine'      | 'u' ) ;

/*
PLUS 	: '+' ;
MINUS	: '-' ;
MULT	: '*' ;
DIV	: '/' ;

NUMBER	: (DIGIT)+ ;

WHITESPACE : ( '\t' | ' ' | '\r' | '\n'| '\u000C' )+ 	{ $channel = HIDDEN; } ;

fragment DIGIT	: '0'..'9' ;
*/
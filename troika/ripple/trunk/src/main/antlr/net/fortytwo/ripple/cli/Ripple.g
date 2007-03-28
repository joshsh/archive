header
{
package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.cli.ast.Ast;
import net.fortytwo.ripple.cli.ast.BooleanAst;
import net.fortytwo.ripple.cli.ast.BlankNodeAst;
import net.fortytwo.ripple.cli.ast.DoubleAst;
import net.fortytwo.ripple.cli.ast.IntegerAst;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.cli.ast.NameAst;
import net.fortytwo.ripple.cli.ast.OperatorAst;
import net.fortytwo.ripple.cli.ast.QNameAst;
import net.fortytwo.ripple.cli.ast.StringAst;
import net.fortytwo.ripple.cli.ast.TypedLiteralAst;
import net.fortytwo.ripple.cli.ast.UriAst;
}


////////////////////////////////////////////////////////////////////////////////


class RippleLexer extends Lexer;

options
{
	k = 3;

	// Use custom error recovery.
	defaultErrorHandler = false;

	buildAST = false;
}

{
	private Interpreter interpreter = null;

	public void initialize( Interpreter r )
	{
		interpreter = r;
	}

	private void endOfLineEvent()
//        throws Exception
	{
		if ( null == interpreter )
			System.err.println( "RippleLexer instance has not been initialized" );

		else if ( !interpreter.readLine() )
			throw new ParserQuitException();
	}
}


protected
WS_CHAR_NOBREAKS
	: ' ' | '\t'
	;

protected
WS_CHAR
	: ( WS_CHAR_NOBREAKS
		|   '\r' '\n' { newline(); }
		|   '\n'      { newline(); endOfLineEvent(); }
	)
	;

WS
	: (WS_CHAR)+
//        { $setType(Token.SKIP); } //ignore this token
	;

protected
NORMAL
	: '$' | '%' | '\'' | '-' | '=' | '!' | ('A' .. 'Z') | '&' | '_' | '`' | ('a' .. 'z') | '{' | '}' | '~'
//    | '\\' ( '\"' | '\\' | WS )
	;

protected
DIGIT
	: ('0' .. '9')
	;

protected
SPECIAL_0
	: '#' | '@' | '^' | '(' | ')' | '*' | '+' | '/' | ';' | '?' | '|' | ':' | '.' | '[' | ']' | ','
	;

protected
SPECIAL
	: SPECIAL_0 | '<' | '>'
	;

protected
ESC
	: '\\' SPECIAL
	;


protected
LANGUAGE
	: ( '@'! ('a'..'z')+ ('-' (('a'..'z') | ('0'..'9'))+)* )
		{ interpreter.setLanguageTag( $getText ); }
	;

STRING
	: '\"'! {
		interpreter.setLanguageTag( null );
	} ( NORMAL | DIGIT | SPECIAL | ESC | WS_CHAR )* '\"'! ( LANGUAGE! )?
	;

URI
	: '<'! ( NORMAL | DIGIT | SPECIAL_0 | WS_CHAR_NOBREAKS | "\\<" | "\\>" | "\\\\" )+ '>'!
	;


OPER
	: '/'
	;


protected
LETTER
	: ('A' .. 'Z') | ('a' .. 'z')
	;

protected
NAME_OR_PREFIX_START_CHAR
	: LETTER | ('\u00C0'..'\u00D6') | ('\u00D8'..'\u00F6') | ('\u00F8'..'\u02FF') | ('\u0370'..'\u037D') | ('\u037F'..'\u1FFF') | ('\u200C'..'\u200D') | ('\u2070'..'\u218F') | ('\u2C00'..'\u2FEF') | ('\u3001'..'\uD7FF') | ('\uF900'..'\uFDCF') | ('\uFDF0'..'\uFFFD') /*| ('\u10000'..'\uEFFFF')*/
	;

protected
NAME_OR_PREFIX_CHAR
	: NAME_OR_PREFIX_START_CHAR | '-' | DIGIT | '\u00B7' | ('\u0300'..'\u036F') | ('\u203F'..'\u2040')
	;

NAME_OR_PREFIX
	: NAME_OR_PREFIX_START_CHAR (NAME_OR_PREFIX_CHAR)*
	;

NAME_NOT_PREFIX
	: '_' (NAME_OR_PREFIX_CHAR)*
	;

BNODEREF_HEAD
	: "_:"
	;


protected
EXPONENT
	: ('e' | 'E') ('-' | '+')? (DIGIT)+
	;

/*


INTEGER
	: ('-' | '+')? (DIGIT)+
	;



DECIMAL
	: ('-' | '+')?
	(
		(DIGIT)+ '.' (DIGIT)*
	| '.' (DIGIT)+
	| (DIGIT)+ )
	;

DOUBLE
	: ('-' | '+')?
	( (DIGIT)+ ( '.' (DIGIT)* )?
	| '.' (DIGIT)+ ) EXPONENT
	;
*/




NUMBER
	: ('-')? ( DIGIT )+ ( '.' ( DIGIT )+ )?
	;


// Identical to Turtle comments.
SINGLE_LINE_COMMENT
	: ( '#' ( ~('\r' | '\n') )* )
		{ $setType( Token.SKIP ); }
	;

/*
MULTI_LINE_COMMENT
	: "(:" ((~':') | (':' ~')'))* ":)"
		{ $setType( Token.SKIP ); }
	;
*/

DOUBLE_HAT
options { paraphrase = "double hat"; } : "^^" ;

L_PAREN
options { paraphrase = "opening parenthesis"; } : '(' ;

R_PAREN
options { paraphrase = "closing parenthesis"; } : ')' ;

EOS
options { paraphrase = "end-of-statement"; } : '.' ;

COLON
options { paraphrase = "colon"; } : ':' ;

protected
DRCTV
	: '@'
	;

DRCTV_COUNT       : DRCTV ( "count"         | "c" ) ;
DRCTV_DEFINE      : DRCTV ( "define"        | "d" ) ;
DRCTV_EXPORT      : DRCTV ( "export"        | "e" ) ;
DRCTV_HELP        : DRCTV ( "help"          | "h" ) ;
DRCTV_LIST        : DRCTV ( "list"          | "l" ) ;
DRCTV_PREFIX      : DRCTV ( "prefix"        | "p" ) ;
DRCTV_QUIT        : DRCTV ( "quit"          | "q" ) ;
DRCTV_SAVEAS      : DRCTV ( "saveas"        | "sa" ) ;
DRCTV_SERQL       : DRCTV ( "serql"         | "sr" ) ;
DRCTV_UNDEFINE    : DRCTV ( "undefine"      | "u" ) ;


////////////////////////////////////////////////////////////////////////////////


class RippleParser extends Parser;
options
{
	k = 1;
	buildAST = false;

	// Use custom error recovery.
	defaultErrorHandler = false;
}

{
	private Interpreter interpreter = null;

	public void initialize( Interpreter r )
	{
		interpreter = r;
	}
}


nt_Document
{
}
	: ( (WS)? nt_Statement )*
	;


nt_Statement
{
	ListAst r;
}
	// A directive is executed as soon as EOS is matched in the individual
	// rule.
	: nt_Directive

	// Query statements are always lists.
	| r=nt_List (WS)? EOS
		{
			interpreter.evaluate( r );
		}

	// Empty statements are simply ignored.
	| EOS
	;


nt_List returns [ ListAst s ]
{
	Ast i;
	s = null;
	boolean modified = false;
}
	: ( OPER { modified = true; } )? i=nt_Node
	( ( WS ~(EOS) ) => ( WS s=nt_List )
	| ( L_PAREN | OPER ) => ( s=nt_List )
	| {}
	)
		{
			// Note: the resulting list will be in the same order as the input.
			if ( modified )
				s = new ListAst( new OperatorAst(), s );
			s = new ListAst( i, s );
		}
	;


nt_Node returns [ Ast r ]
{
	r = null;
}
	: r=nt_Resource
	| r=nt_Literal
	| r=nt_ParenthesizedList
	;


nt_ParenthesizedList returns [ ListAst r ]
{
	r = null;
}
	: L_PAREN (WS)? ( r=nt_List )? R_PAREN
	;


nt_Literal returns [ Ast r ]
{
	r = null;
	Ast dataType = null;
}
	: ( t:STRING

		/* Note: for agreement with Turtle, the grammar allows any resource
				reference as the data type of a literal (i.e. a URI or a blank
				node).  However, the Sesame back end will only accept a URI. */
		( DOUBLE_HAT dataType=nt_Resource )?
	)
		{
			r = ( null == dataType )
				? new StringAst( t.getText(), interpreter.getLanguageTag() )
				: new TypedLiteralAst( t.getText(), dataType );
		}
	| u:NUMBER
		{
			try
			{
				String s = u.getText();
				if ( s.contains( "." ) )
					r = new DoubleAst( ( new Double( s ) ).doubleValue() );
				else
					r = new IntegerAst( ( new Integer( s ) ).intValue() );
			}

			catch ( NumberFormatException e )
			{
				System.err.println( "a NumberFormattingException was encountered (this shouldn't happen)" );
				r = null;
				interpreter.quit();
			}
		}
	;


nt_Resource returns [ Ast r ]
{
	r = null;
}
	: r=nt_URIRef
	| ( (NAME_OR_PREFIX)? COLON ) => r=nt_QName
	| r=nt_SimpleName
	| r=nt_BNodeRef
	;


nt_URIRef returns [ UriAst r ]
{
	r = null;
}
	: uri:URI
		{
			r = new UriAst( uri.getText() );
		}
	;


nt_Prefix returns [ String prefix ]
{
	prefix = null;
}
	: t:NAME_OR_PREFIX { prefix = t.getText(); }
	;


nt_SimpleName returns [ Ast r ]
{
	String localName;
	r = null;
}
	: localName=nt_Name { r = new NameAst( localName ); }
	;


nt_QName returns [ Ast r ]
{
	String nsPrefix = "", localName = "";
	r = null;
}
	: ( ( nsPrefix=nt_Prefix )?
		COLON
		( localName=nt_Name )? )
		{
			r = new QNameAst( nsPrefix, localName );
		}
	;


nt_BNodeRef returns [ Ast r ]
{
	r = null;
	String localName = null;
}
	: BNODEREF_HEAD localName=nt_Name
		{
			r = new BlankNodeAst( localName );
		}
	;


nt_Name returns [ String name ]
{
	name = null;
}
	: t1:NAME_OR_PREFIX { name = t1.getText(); }
	| t2:NAME_NOT_PREFIX { name = t2.getText(); }
	;


nt_Directive
{
	UriAst ns;

	// Default to the empty (but not null) prefix.
	String nsPrefix = "";

	String localName = null;
	ListAst rhs;
}
	: DRCTV_COUNT WS "statements" (WS)? EOS
		{
			interpreter.countStatements();
		}

	| DRCTV_DEFINE WS localName=nt_Name (WS)? COLON (WS)? rhs=nt_List (WS)? EOS
		{
			interpreter.evaluateAndDefine( rhs, localName );
		}

	| DRCTV_EXPORT ( WS ( nsPrefix=nt_Prefix (WS)? )? )? COLON (WS)? exFile:STRING (WS)? EOS
		{
			interpreter.exportNs( nsPrefix, exFile.getText() );
		}

	| DRCTV_HELP (WS)? EOS
		{
			System.out.println( "\nSorry, the @help directive is just a placeholder for now.\n" );
		}

	| DRCTV_LIST WS
		( "contexts" (WS)? EOS
			{
				interpreter.showContexts();
			}
		| "prefixes" (WS)? EOS
			{
				interpreter.showNamespaces();
			}
		)

	| DRCTV_PREFIX WS ( nsPrefix=nt_Prefix (WS)? )? COLON (WS)? ns=nt_URIRef (WS)? EOS
		{
			interpreter.setNamespace( nsPrefix, ns );
		}

	| DRCTV_QUIT (WS)? EOS
		{
			interpreter.quit();
		}

	| DRCTV_SAVEAS WS saFile:STRING (WS)? EOS
		{
			interpreter.saveAs( saFile.getText() );
		}

	| DRCTV_SERQL WS query:STRING (WS)? EOS
		{
			interpreter.evaluateGraphQuery( query.getText() );
		}

	| DRCTV_UNDEFINE WS localName=nt_Name (WS)? EOS
		{
			interpreter.undefine( localName );
		}
	;


// kate: tab-width 4

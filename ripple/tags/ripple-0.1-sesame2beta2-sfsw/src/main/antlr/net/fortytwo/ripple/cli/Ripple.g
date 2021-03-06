header
{
package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.cli.ast.Ast;
import net.fortytwo.ripple.cli.ast.BooleanAst;
import net.fortytwo.ripple.cli.ast.BlankNodeAst;
import net.fortytwo.ripple.cli.ast.DoubleAst;
import net.fortytwo.ripple.cli.ast.IntegerAst;
import net.fortytwo.ripple.cli.ast.KeywordAst;
import net.fortytwo.ripple.cli.ast.ListAst;
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
}

{
	private Interpreter interpreter = null;

	public void initialize( Interpreter r )
	{
		interpreter = r;
	}

	private void endOfLineEvent()
	{
		if ( null == interpreter )
			System.err.println( "RippleLexer instance has not been initialized" );

		else if ( !interpreter.readLine() )
			throw new ParserQuitException();
	}
}


protected
WS_CHAR
	: ' ' | '\t' | '\r'
	| '\n'      { newline(); endOfLineEvent(); }
	;

WS
	: (WS_CHAR)+
	;

protected
HEX
	: ('0'..'9')
	| ('A'..'F')
	;

protected
CHARACTER_NOQUOTE_NOGT
	: ' ' | '!' | ('#'..'=') | ('?'..'[')
	| (']'..'\uFFFF')  // Note: '\u10FFFF in Turtle
	| "\\u" HEX HEX HEX HEX
	| "\\U" HEX HEX HEX HEX HEX HEX HEX HEX
//	| "\\\\"
	;

protected
ECHARACTER_NOQUOTE
	: ( CHARACTER_NOQUOTE_NOGT | '>' )
//	| "\\t" | "\\n" | "\\r"
	;

protected
ECHARACTER
	: ECHARACTER_NOQUOTE | '\"'
	;

protected
SCHARACTER
	: ECHARACTER_NOQUOTE
//	| "\\\""
	;

protected
LANGUAGE
	: ( '@'! ('a'..'z')+ ('-' (('a'..'z') | ('0'..'9'))+)* )
		{ interpreter.setLanguageTag( $getText ); }
	;

/*
LONG_STRING
	: "\"\"\""!
		{ interpreter.setLanguageTag( null ); }
		( SCHARACTER )* "\"\"\""! ( LANGUAGE! )?
	;
*/

STRING
	: '\"'!
		{ interpreter.setLanguageTag( null ); }
		( SCHARACTER )* '\"'! ( LANGUAGE! )?
	;

protected
UCHARACTER
	: ( CHARACTER_NOQUOTE_NOGT | '\"' )
	| "\\>"
	;

URIREF
	: '<'! ( UCHARACTER )* '>'!
	;

OPER
	: '/'
	;

protected
DIGIT
	: ('0' .. '9')
	;

protected
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

protected
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

NODEID_PREFIX : "_:" ;

NUMBER
	: ('-' | '+')? ( DIGIT )+ //( '.' ( DIGIT )+ )?
	;

// Ignore comments.
COMMENT
	: ( '#' ( ~('\r' | '\n' ) )* ) { $setType( Token.SKIP ); }
//	: ( '#' (( ('\u0000'..'\u0009') | '\u000B' | '\u000C' | ('\u000E'..'\uFFFF')){System.out.println(".");} )* )
//		{ $setType( Token.SKIP ); }
	;

/*
MULTI_LINE_COMMENT
	: "(:" ((~':') | (':' ~')'))* ":)"
		{ $setType( Token.SKIP ); }
	;
*/

DOUBLE_HAT : "^^" ;

L_PAREN : '(' ;

R_PAREN : ')' ;

EOS : '.' ;

COLON : ':' ;

protected
DRCTV : '@' ;

DRCTV_COUNT     : DRCTV ( "count"         | "c" ) ;
DRCTV_DEFINE    : DRCTV ( "define"        | "d" ) ;
DRCTV_EXPORT    : DRCTV ( "export"        | "e" ) ;
DRCTV_HELP      : DRCTV ( "help"          | "h" ) ;
DRCTV_LIST      : DRCTV ( "list"          | "l" ) ;
DRCTV_PREFIX    : DRCTV ( "prefix"        | "p" ) ;
DRCTV_QUIT      : DRCTV ( "quit"          | "q" ) ;
DRCTV_SAVEAS    : DRCTV ( "saveas"        | "sa" ) ;
DRCTV_SERQL     : DRCTV ( "serql"         | "sr" ) ;
DRCTV_UNDEFINE  : DRCTV ( "undefine"      | "u" ) ;


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
				System.err.println( "a NumberFormatException was encountered (this shouldn't happen)" );
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
	: uri:URIREF
		{
			r = new UriAst( uri.getText() );
		}
	;


nt_PrefixName returns [ String prefix ]
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
	: localName=nt_Name { r = new KeywordAst( localName ); }
	;


nt_QName returns [ Ast r ]
{
	String nsPrefix = "", localName = "";
	r = null;
}
	: ( ( nsPrefix=nt_PrefixName )?
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
	: NODEID_PREFIX localName=nt_Name
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

	| DRCTV_EXPORT ( WS ( nsPrefix=nt_PrefixName (WS)? )? )? COLON (WS)? exFile:STRING (WS)? EOS
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

	| DRCTV_PREFIX WS ( nsPrefix=nt_PrefixName (WS)? )? COLON (WS)? ns=nt_URIRef (WS)? EOS
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

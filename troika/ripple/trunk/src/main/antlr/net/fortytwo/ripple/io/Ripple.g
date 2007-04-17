header
{
package net.fortytwo.ripple.io;

import net.fortytwo.ripple.ast.Ast;
import net.fortytwo.ripple.ast.BooleanAst;
import net.fortytwo.ripple.ast.BlankNodeAst;
import net.fortytwo.ripple.ast.DoubleAst;
import net.fortytwo.ripple.ast.IntegerAst;
import net.fortytwo.ripple.ast.KeywordAst;
import net.fortytwo.ripple.ast.ListAst;
import net.fortytwo.ripple.ast.OperatorAst;
import net.fortytwo.ripple.ast.QNameAst;
import net.fortytwo.ripple.ast.StringAst;
import net.fortytwo.ripple.ast.TypedLiteralAst;
import net.fortytwo.ripple.ast.UriAst;
import net.fortytwo.ripple.query.commands.CountStatementsCmd;
import net.fortytwo.ripple.query.commands.DefinePrefixCmd;
import net.fortytwo.ripple.query.commands.DefineTermCmd;
import net.fortytwo.ripple.query.commands.ExportNsCmd;
import net.fortytwo.ripple.query.commands.QuitCmd;
import net.fortytwo.ripple.query.commands.SaveAsCmd;
import net.fortytwo.ripple.query.commands.SerqlQueryCmd;
import net.fortytwo.ripple.query.commands.ShowContextsCmd;
import net.fortytwo.ripple.query.commands.ShowPrefixesCmd;
import net.fortytwo.ripple.query.commands.UndefineTermCmd;
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

		else
			interpreter.readLine();
	}
}


protected
WS_CHAR
	: ' ' | '\t' | '\r'
	| '\n'  { newline(); endOfLineEvent(); }
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
SCHARACTER
	: ' ' | '!' | ('#'..'[')  // excludes: '\"', '\\'
	| (']'..'\uFFFF')  // Note: '\u10FFFF' in Turtle
	| "\\u" HEX HEX HEX HEX
	| "\\U" HEX HEX HEX HEX HEX HEX HEX HEX
	| '\\' ('\\' | '\"' | 't' | 'n' | 'r' )
	;

protected
UCHARACTER
	: (' '..'=') | ('?'..'[')  // excludes: '>', '\\'
	| (']'..'\uFFFF')  // Note: '\u10FFFF' in Turtle
	| "\\u" HEX HEX HEX HEX
	| "\\U" HEX HEX HEX HEX HEX HEX HEX HEX
	| '\\' ('\\' | '>')
	;

protected
LANGUAGE
	: ( '@'! ('a'..'z')+ ('-' (('a'..'z') | ('0'..'9'))+)* )
		{ interpreter.setLanguageTag( $getText ); }
	;

STRING
	: '\"'!
		{ interpreter.setLanguageTag( null ); }
		( SCHARACTER )* '\"'! ( LANGUAGE! )?
	;

/*
LONG_STRING
	: "\"\"\""!
		{ interpreter.setLanguageTag( null ); }
		( SCHARACTER )* "\"\"\""! ( LANGUAGE! )?
	;
*/

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
	: ( '#' ( ~('\n') )* ) { $setType( Token.SKIP ); }
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
	: ( (nt_Ws)? nt_Statement )*
	;


nt_Ws
	// Note: consecutive WS tokens occur when the lexer matches a COMMENT
	//       between them.
	: (WS)+
	;


nt_Statement
{
	ListAst r;
}
	// A directive is executed as soon as EOS is matched in the individual
	// rule.
	: nt_Directive

	// Query statements are always lists.
	| r=nt_List (nt_Ws)? EOS
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
	: ( ( OPER { modified = true; } )? i=nt_Node
		( ( WS ~(EOS | R_PAREN) ) => ( nt_Ws s=nt_List )
		| ( ~(WS | EOS | R_PAREN) ) => ( s=nt_List )
		| {}  // end of list
		) )
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
	: L_PAREN (nt_Ws)? (
		( r=nt_List (nt_Ws)? R_PAREN )
		| R_PAREN )
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
			// Note: number format exceptions are handled at a higher level.
			String s = u.getText();
			if ( s.contains( "." ) )
				r = new DoubleAst( ( new Double( s ) ).doubleValue() );
			else
				r = new IntegerAst( ( new Integer( s ) ).intValue() );
		}
	;


nt_Resource returns [ Ast r ]
{
	r = null;
}
	: r=nt_URIRef
	| ( (NAME_OR_PREFIX)? COLON ) => r=nt_QName
	| r=nt_Keyword
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


nt_Keyword returns [ Ast r ]
{
	String keyword;
	r = null;
}
	: keyword=nt_Name { r = new KeywordAst( keyword ); }
	;


nt_QName returns [ Ast r ]
{
	String nsPrefix = "", localName = "";
	r = null;
}
	: ( ( nsPrefix=nt_PrefixName )?
		COLON
		( localName=nt_Name )?
//		( localName=nt_Name
//		| (~(NAME_OR_PREFIX | NAME_NOT_PREFIX)) => {} )
	  )
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
	: DRCTV_COUNT nt_Ws "statements" (nt_Ws)? EOS
		{
			interpreter.execute( new CountStatementsCmd() );
		}

	| DRCTV_DEFINE nt_Ws localName=nt_Name (nt_Ws)? COLON (nt_Ws)? rhs=nt_List (nt_Ws)? EOS
		{
			interpreter.execute( new DefineTermCmd( rhs, localName ) );
		}

	| DRCTV_EXPORT ( nt_Ws ( nsPrefix=nt_PrefixName (nt_Ws)? )? )? COLON (nt_Ws)? exFile:STRING (nt_Ws)? EOS
		{
			interpreter.execute( new ExportNsCmd( nsPrefix, exFile.getText() ) );
		}

	| DRCTV_HELP (nt_Ws)? EOS
		{
			System.out.println( "\nSorry, the @help directive is just a placeholder for now.\n" );
		}

	| DRCTV_LIST nt_Ws
		( "contexts" (nt_Ws)? EOS
			{
				interpreter.execute( new ShowContextsCmd() );
			}
		| "prefixes" (nt_Ws)? EOS
			{
				interpreter.execute( new ShowPrefixesCmd() );
			}
		)

	| DRCTV_PREFIX nt_Ws ( nsPrefix=nt_PrefixName (nt_Ws)? )? COLON (nt_Ws)? ns=nt_URIRef (nt_Ws)? EOS
		{
			interpreter.execute( new DefinePrefixCmd( nsPrefix, ns ) );
		}

	| DRCTV_QUIT (nt_Ws)? EOS
		{
			interpreter.execute( new QuitCmd() );
		}

	| DRCTV_SAVEAS nt_Ws saFile:STRING (nt_Ws)? EOS
		{
			interpreter.execute( new SaveAsCmd( saFile.getText() ) );
		}

	| DRCTV_SERQL nt_Ws query:STRING (nt_Ws)? EOS
		{
			interpreter.execute( new SerqlQueryCmd( query.getText() ) );
		}

	| DRCTV_UNDEFINE nt_Ws localName=nt_Name (nt_Ws)? EOS
		{
			interpreter.execute( new UndefineTermCmd( localName ) );
		}
	;


// kate: tab-width 4

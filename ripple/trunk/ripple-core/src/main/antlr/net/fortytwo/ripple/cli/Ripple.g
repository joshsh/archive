header
{
package net.fortytwo.ripple.cli;

import java.util.Properties;

import net.fortytwo.ripple.cli.ast.Ast;
import net.fortytwo.ripple.cli.ast.BooleanAst;
import net.fortytwo.ripple.cli.ast.BlankNodeAst;
import net.fortytwo.ripple.cli.ast.DoubleAst;
import net.fortytwo.ripple.cli.ast.IntegerAst;
import net.fortytwo.ripple.cli.ast.KeywordAst;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.cli.ast.OperatorAst;
import net.fortytwo.ripple.cli.ast.PropertyAnnotatedAst;
import net.fortytwo.ripple.cli.ast.QNameAst;
import net.fortytwo.ripple.cli.ast.StringAst;
import net.fortytwo.ripple.cli.ast.TypedLiteralAst;
import net.fortytwo.ripple.cli.ast.UriAst;
import net.fortytwo.ripple.query.Command;
import net.fortytwo.ripple.query.commands.CountStatementsCmd;
import net.fortytwo.ripple.query.commands.DefinePrefixCmd;
import net.fortytwo.ripple.query.commands.DefineTermCmd;
import net.fortytwo.ripple.query.commands.ExportNsCmd;
import net.fortytwo.ripple.query.commands.QuitCmd;
import net.fortytwo.ripple.query.commands.SaveAsCmd;
import net.fortytwo.ripple.query.commands.ShowContextsCmd;
import net.fortytwo.ripple.query.commands.ShowNamespacesCmd;
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
	RecognizerAdapter adapter = null;

	public void initialize( final RecognizerAdapter i )
	{
		adapter = i;
	}

	void matchEndOfLine()
	{
		adapter.putEvent( RecognizerEvent.NEWLINE );
	}

/*
	void matchEscapeCharacter()
	{
System.out.println( "matchEscapeCharacter" );
		adapter.putEvent( RecognizerEvent.ESCAPE );
	}
*/
}


protected
WS_CHAR
	: ' ' | '\t' | '\r'
	| '\n'  { newline(); matchEndOfLine(); }
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
		{ adapter.setLanguageTag( $getText ); }
	;

STRING
	: '\"'!
		{ adapter.setLanguageTag( null ); }
		( SCHARACTER )* '\"'! ( LANGUAGE! )?
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
	: ('-' | '+')? ( DIGIT )+
		(('.' DIGIT ) => ( '.' ( DIGIT )+ )
		| ())
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

L_BRACKET : '[';
R_BRACKET : ']';

SEMI : ';' ;
PERIOD : '.' ;
COMMA : ',';

COLON : ':' ;

EQUAL : '=';

OP_PRE : '/' ;
OP_POST : '!' ;

protected
DRCTV : '@' ;

DRCTV_COUNT     : DRCTV ( "count"         | "c" ) ;
DRCTV_DEFINE    : DRCTV ( "define"        | "d" ) ;
DRCTV_EXPORT    : DRCTV ( "export"        | "e" ) ;
DRCTV_HELP      : DRCTV ( "help"          | "h" ) ;
DRCTV_LIST      : DRCTV ( "list"          | "l" ) ;
DRCTV_PREFIX    : DRCTV ( "prefix"        | "p" ) ;
DRCTV_QUIT      : DRCTV ( "quit"          | "q" ) ;
DRCTV_SAVEAS    : DRCTV ( "saveas"        | "s" ) ;
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
	private RecognizerAdapter adapter = null;

	public void initialize( final RecognizerAdapter i )
	{
		adapter = i;
	}

	public void matchCommand( final Command cmd )
	{
		adapter.putCommand( cmd );
	}

	public void matchQuery( final ListAst ast )
	{
		adapter.putQuery( ast );
	}

	public void matchContinuingQuery( final ListAst ast )
	{
		adapter.putContinuingQuery( ast );
	}

	public void matchQuit()
	{
		adapter.putEvent( RecognizerEvent.QUIT );
	}
}


nt_Document
{
	// Request a first line of input from the interface (the lexer will request
	// additional input as it matches newlines).
	adapter.putEvent( RecognizerEvent.NEWLINE );
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
	// A directive is executed as soon as PERIOD is matched in the individual
	// rule.
	: nt_Directive

	// Query statements are always lists.
	| r=nt_List (
		PERIOD { matchQuery( r ); }
 		| SEMI { matchContinuingQuery( r ); } )

	// Empty statements are effectively ignored.
	| PERIOD { matchQuery( new ListAst() ); }
	| SEMI { matchContinuingQuery( new ListAst() ); }
	;


nt_List returns [ ListAst list ]
{
	Ast first;
	ListAst rest = null;
	list = null;
	boolean modified = false;
}
		// Optional slash operator.
	:	( OP_PRE (WS)? { modified = true; } )?

		// Head of the list.
		first = nt_Node

		(	(WS) => ( nt_Ws
				( (~(PERIOD | SEMI | R_PAREN )) => rest = nt_List
				| {}
				) )

			// Tail of the list.
		|	(~(WS | PERIOD | SEMI | R_PAREN)) => rest = nt_List

			// End of the list.
		|	()
		)
			{
				if ( null == rest )
				{
					rest = new ListAst();
				}

				if ( modified )
				{
					rest = new ListAst( new OperatorAst(), rest );
				}

				list = new ListAst( first, rest );
			}
	;


nt_Node returns [ Ast r ]
{
	r = null;
	Properties props;
}
	: ( r=nt_Resource
		| r=nt_Literal
		| r=nt_ParenthesizedList
		| OP_POST { r = new OperatorAst(); }
		)
	  (( (WS)? L_BRACKET ) => ( (WS)? props=nt_Properties { r = new PropertyAnnotatedAst( r, props ); } )
	  | ())
	;


nt_Properties returns [ Properties props ]
{
	props = new Properties();
}
	: L_BRACKET (WS)? nt_PropertyList[props] R_BRACKET
	;

	
nt_PropertyList[ Properties props ]
{
	String name;
}
	: name=nt_PropertyName EQUAL value:STRING (WS)? { props.setProperty( name, value.getText() ); }
		( COMMA (WS)? nt_PropertyList[props] )?
	;
	
	
nt_PropertyName returns [ String name ]
{
	String rest;
}
	: name=nt_Name
		( PERIOD rest=nt_PropertyName { name += rest; } )?
	;
	
	
nt_ParenthesizedList returns [ ListAst r ]
{
	r = null;
}
	: L_PAREN (nt_Ws)? (
		( r = nt_List /*(nt_Ws)?*/ R_PAREN )
		| R_PAREN { r = new ListAst(); } )
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
				? new StringAst( t.getText(), adapter.getLanguageTag() )
				: new TypedLiteralAst( t.getText(), dataType );
		}
	| u:NUMBER
		{
			// Note: number format exceptions are handled at a higher level.
			String s = u.getText();

			if ( s.contains( "." ) )
			{
				r = new DoubleAst( ( new Double( s ) ).doubleValue() );
			}

			else
			{
				r = new IntegerAst( ( new Integer( s ) ).intValue() );
			}
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
	: DRCTV_COUNT nt_Ws "statements" (nt_Ws)? PERIOD
		{
			matchCommand( new CountStatementsCmd() );
		}

	| DRCTV_DEFINE nt_Ws localName=nt_Name (nt_Ws)? COLON (nt_Ws)? rhs=nt_List /*(nt_Ws)?*/ PERIOD
		{
			matchCommand( new DefineTermCmd( rhs, localName ) );
		}

	| DRCTV_EXPORT ( nt_Ws ( nsPrefix=nt_PrefixName (nt_Ws)? )? )? COLON (nt_Ws)? exFile:STRING (nt_Ws)? PERIOD
		{
			matchCommand( new ExportNsCmd( nsPrefix, exFile.getText() ) );
		}

	| DRCTV_HELP (nt_Ws)? PERIOD
		{
			System.out.println( "\nSorry, the @help directive is just a placeholder for now.\n" );
		}

	| DRCTV_LIST nt_Ws
		( "contexts" (nt_Ws)? PERIOD
			{
				matchCommand( new ShowContextsCmd() );
			}
		| "prefixes" (nt_Ws)? PERIOD
			{
				matchCommand( new ShowNamespacesCmd() );
			}
		)

	| DRCTV_PREFIX nt_Ws ( nsPrefix=nt_PrefixName (nt_Ws)? )? COLON (nt_Ws)? ns=nt_URIRef (nt_Ws)? PERIOD
		{
			matchCommand( new DefinePrefixCmd( nsPrefix, ns ) );
		}

	| DRCTV_QUIT (nt_Ws)? PERIOD
		{
			matchQuit();
//			matchCommand( new QuitCmd() );
		}

	| DRCTV_SAVEAS nt_Ws saFile:STRING (nt_Ws)? PERIOD
		{
			matchCommand( new SaveAsCmd( saFile.getText() ) );
		}

	| DRCTV_UNDEFINE nt_Ws localName=nt_Name (nt_Ws)? PERIOD
		{
			matchCommand( new UndefineTermCmd( localName ) );
		}
	;


// kate: tab-width 4

/*
    Note: this grammar was converted from ANTLR v2 to v3 like so:

        java -cp .:/home/josh/.m2/repository/org/antlr/antlr-runtime/3.0/antlr-runtime-3.0.jar v3me -combined ~/projects/ripple/trunk/ripple-core/src/main/antlr/net/fortytwo/ripple/cli/Ripple.g > ~/projects/ripple/trunk/ripple-core/src/main/antlr/net/fortytwo/ripple/cli/Ripple-new.g

    ...and then aggressively fine-tuned.
*/

grammar Ripple;


@lexer::header {
package net.fortytwo.ripple.cli;
}

@lexer::members {
	private RecognizerAdapter adapter = null;

	public void initialize( final RecognizerAdapter i )
	{
		adapter = i;
	}

	public void matchEndOfLine()
	{
//System.out.println("matching end of line.........");
        if ( null != adapter )
        {
		    adapter.putEvent( RecognizerEvent.NEWLINE );
		}
	}

/*
	void matchEscapeCharacter()
	{
System.out.println( "matchEscapeCharacter" );
		adapter.putEvent( RecognizerEvent.ESCAPE );
	}
*/
}

@parser::header {
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
import net.fortytwo.ripple.query.commands.RedefineTermCmd;
import net.fortytwo.ripple.query.commands.ShowContextsCmd;
import net.fortytwo.ripple.query.commands.ShowNamespacesCmd;
import net.fortytwo.ripple.query.commands.UndefineTermCmd;
import net.fortytwo.ripple.query.PipedIOStream;

import java.io.InputStream;
import java.io.FileInputStream;
import org.antlr.runtime.ModifiedANTLRInputStream;
}

/*options
{
	k = 1;
	output=AST;

	// Use custom error recovery.
	defaultErrorHandler = false;
}*/

@parser::members {
	private RecognizerAdapter adapter = null;

	public void initialize( final RecognizerAdapter i )
	{
		adapter = i;
	}

	public void matchEndOfLine()
	{
//System.out.println("matching end of line.........");
        if ( null != adapter )
        {
		    adapter.putEvent( RecognizerEvent.NEWLINE );
		}
	}

	public void matchEndOfInput()
	{
System.out.println("matching end of input.........");
        if ( null != adapter )
        {
	        adapter.putEvent( RecognizerEvent.END_OF_INPUT );
	    }
    }

	public void matchCommand( final Command cmd )
	{
System.out.println("#### matching a command: " + cmd);
        if ( null != adapter )
        {
		    adapter.putCommand( cmd );
		}
	}

	public void matchQuery( final ListAst ast )
	{
System.out.println("#### matching a query: " + ast);
        if ( null != adapter )
        {
		    adapter.putQuery( ast );
		}
	}

	public void matchContinuingQuery( final ListAst ast )
	{
        if ( null != adapter )
        {
		    adapter.putContinuingQuery( ast );
		}
	}

	public void matchQuit()
	{
        if ( null != adapter )
        {
		    adapter.putEvent( RecognizerEvent.QUIT );
		}
	}

	public static void main(String[] args) throws Exception {
	    InputStream is = new FileInputStream(args[0]);
	    PipedIOStream pios = new PipedIOStream();
	    while (is.available() > 0) {
	        pios.write(is.read());
        }
        pios.write(10);
        //pios.write(-1);
        is.close();
        CharStream cs = new ModifiedANTLRInputStream(pios);

        //CharStream cs = new ANTLRFileStream(args[0]);
        RippleLexer lex = new RippleLexer(cs);
       	CommonTokenStream tokens = new CommonTokenStream(lex);

        RippleParser parser = new RippleParser(tokens);

        try {
            parser.nt_Document();
        } catch (RecognitionException e)  {
            e.printStackTrace();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

/*options
{
	k = 3;

	// Use custom error recovery.
	defaultErrorHandler = false;
}*/

fragment
WS_CHAR
	: ' ' | '\t' | '\r'
	| '\n'  { /*newline();*/ matchEndOfLine(); }
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
		{ adapter.setLanguageTag( null ); }
		( SCHARACTER )* '\"'! ( lang=LANGUAGE! {adapter.setLanguageTag( lang.getText() ); } )?
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

////////////////////////////////////////////////////////////////////////////////

nt_Document
@init {
	// Request a first line of input from the interface (the lexer will request
	// additional input as it matches newlines).
	matchEndOfLine();
}
	: ( (nt_Ws)? nt_Statement )*
	EOF { matchEndOfInput(); } (nt_Ws)?
	;

nt_Ws
	// Note: consecutive WS tokens occur when the lexer matches a COMMENT
	//       between them.
	: (WS)+ {System.out.println("found some whitespace!");}
	;

nt_Statement
@init {
	ListAst st = null;
}
	// A directive is executed as soon as PERIOD is matched in the individual
	// rule.
	: nt_Directive

	// Query statements are always lists.
	| r=nt_List { st = r; } (
		PERIOD { matchQuery( st ); }
 		| SEMI { matchContinuingQuery( st ); } )

	// Empty statements are effectively ignored.
	| PERIOD { matchQuery( new ListAst() ); }
	| SEMI { matchContinuingQuery( new ListAst() ); }
	;

nt_List returns [ ListAst list ]
@init {
	Ast first = null;
	ListAst rest = null;
	list = null;
	boolean modified = false;
}
		// Optional slash operator.
	:	( OP_APPLY_PRE (nt_Ws)? { modified = true; } )?

		// Head of the list.
		f=nt_Node { first = f; }

		(	(WS) => ( nt_Ws
				( (~(PERIOD | SEMI | R_PAREN )) => r0=nt_List { rest = r0; }
				| {}
				) )

			// Tail of the list.
		|	(~(WS | PERIOD | SEMI | R_PAREN)) => r1=nt_List { rest = r1; }

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

nt_Node returns [ Ast node ]
@init {
	node = null;
}
	: ( nres=nt_Resource { node = nres; }
		| nlit=nt_Literal { node = nlit; }
		| nparen=nt_ParenthesizedList { node = nparen; }
		| nop=nt_Operator { node = nop; }
		)
	  (( (nt_Ws)? L_BRACKET ) => ( (nt_Ws)? props=nt_Properties { node = new PropertyAnnotatedAst( node, props ); } )
	  | ())
	;

nt_Properties returns [ Properties props ]
@init {
	props = new Properties();
}
	: L_BRACKET (nt_Ws)? nt_PropertyList[props] R_BRACKET
	;

nt_PropertyList[ Properties props ]
	: name=nt_PropertyName EQUAL value=STRING (nt_Ws)? { props.setProperty( name, value.getText() ); }
		( COMMA (nt_Ws)? nt_PropertyList[props] )?
	;
	
nt_PropertyName returns [ String name ]
	: n=nt_Name { name = n; }
		( PERIOD rest=nt_PropertyName { name += rest; } )?
	;
	
nt_ParenthesizedList returns [ ListAst list ]
@init {
	list = null;
}
	: L_PAREN (nt_Ws)? (
		( l=nt_List { list = l; } /*(nt_Ws)?*/ R_PAREN )
		| R_PAREN { list = new ListAst(); } )
	;

nt_Literal returns [ Ast r ]
@init {
	r = null;
	Ast dataType = null;
}
	: ( t=STRING

		/* Note: for agreement with Turtle, the grammar allows any resource
				reference as the data type of a literal (i.e. a URI or a blank
				node).  However, the Sesame back end will only accept a URI. */
		( DOUBLE_HAT dt=nt_Resource { dataType = dt; } )?
	)
		{
			r = ( null == dataType )
				? new StringAst( t.getText(), adapter.getLanguageTag() )
				: new TypedLiteralAst( t.getText(), dataType );
		}
	| u=NUMBER
		{
			// Note: number format exceptions are handled at a higher level.
			String s = u.getText();

			if ( s.contains( ".") )
			{
				r = new DoubleAst( ( new Double( s ) ).doubleValue() );
			}

			else
			{
				r = new IntegerAst( ( new Integer( s ) ).intValue() );
			}
		}
	;

nt_Resource returns [ Ast res ]
@init {
	res = null;
}
	: ruri=nt_URIRef { res = ruri; }
	| ( (NAME_OR_PREFIX)? COLON ) => rqname=nt_QName { res = rqname; }
	| rkeywd=nt_Keyword { res = rkeywd; }
	| rbnode=nt_BNodeRef { res = rbnode; }
	;

nt_URIRef returns [ UriAst ref ]
@init {
	ref = null;
}
	: uri=URIREF
		{
			ref = new UriAst( uri.getText() );
		}
	;

nt_PrefixName returns [ String prefix ]
@init {
	prefix = null;
}
	: t=NAME_OR_PREFIX { prefix = t.getText(); }
	;

nt_Keyword returns [ Ast r ]
@init {
	r = null;
}
	: keyword=nt_Name { r = new KeywordAst( keyword ); }
	;

nt_QName returns [ Ast r ]
@init {
	String nsPrefix = "";
}
	: ( ( pre=nt_PrefixName { nsPrefix = pre; } )?
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
@init {
	r = null;
}
	: NODEID_PREFIX localName=nt_Name
		{
			r = new BlankNodeAst( localName );
		}
	;

nt_Name returns [ String name ]
@init {
	name = null;
}
	: t1=NAME_OR_PREFIX { name = t1.getText(); }
	| t2=NAME_NOT_PREFIX { name = t2.getText(); }
	;

nt_Operator returns [ OperatorAst ast ]
@init {
	ast = null;
}
	: OP_APPLY_POST { ast = new OperatorAst(); }
	| OP_OPTIONAL { ast = new OperatorAst( OperatorAst.Type.Option ); }
	| OP_STAR { ast = new OperatorAst( OperatorAst.Type.Star ); }
	| OP_PLUS { ast = new OperatorAst( OperatorAst.Type.Plus ); }
	| L_CURLY (nt_Ws)? min=NUMBER (nt_Ws)? ( COMMA (nt_Ws)? max=NUMBER (nt_Ws)? )? R_CURLY
		{
			// Note: floating-point values are syntactically valid, but will be
			// truncated to integer values.
			int minVal = new Double( min.getText() ).intValue();
			
			if ( null == max )
			{
				ast = new OperatorAst( minVal );
			}

			else
			{
				int maxVal = new Double( max.getText() ).intValue();
				ast = new OperatorAst( minVal, maxVal );
			}
		}
	;

nt_Directive
@init {
	// Default to the empty (but not null) prefix.
	String nsPrefix = "";
}
	: DRCTV_COUNT nt_Ws 'statements' (nt_Ws)? PERIOD
		{
			matchCommand( new CountStatementsCmd() );
		}

	| DRCTV_DEFINE nt_Ws localName=nt_Name (nt_Ws)? COLON (nt_Ws)? rhs=nt_List /*(nt_Ws)?*/ PERIOD
		{
			matchCommand( new DefineTermCmd( localName, rhs ) );
		}

	| DRCTV_EXPORT ( nt_Ws ( expre=nt_PrefixName { nsPrefix = expre; } (nt_Ws)? )? )? COLON (nt_Ws)? exFile=STRING (nt_Ws)? PERIOD
		{
			matchCommand( new ExportNsCmd( nsPrefix, exFile.getText() ) );
		}

	| DRCTV_HELP (nt_Ws)? PERIOD
		{
			System.out.println( "\nSorry, the @help directive is just a placeholder for now.\n" );
		}
	| DRCTV_LIST nt_Ws
		( 'contexts' (nt_Ws)? PERIOD
			{
				matchCommand( new ShowContextsCmd() );
			}
		| 'prefixes' (nt_Ws)? PERIOD
			{
				matchCommand( new ShowNamespacesCmd() );
			}
		)

	| DRCTV_PREFIX nt_Ws ( prepre=nt_PrefixName { nsPrefix = prepre; } (nt_Ws)? )? COLON (nt_Ws)? ns=nt_URIRef (nt_Ws)? PERIOD
		{
			matchCommand( new DefinePrefixCmd( nsPrefix, ns ) );
		}

	| DRCTV_QUIT {System.out.println("QUIT!");} (nt_Ws)? PERIOD
		{
			matchQuit();
//			matchCommand( new QuitCmd() );
		}
		
	| DRCTV_REDEFINE nt_Ws localName=nt_Name (nt_Ws)? COLON (nt_Ws)? rhs=nt_List /*(nt_Ws)?*/ PERIOD
		{
			matchCommand( new RedefineTermCmd( localName, rhs ) );
		}
		
	| DRCTV_UNDEFINE nt_Ws localName=nt_Name (nt_Ws)? PERIOD
		{
			matchCommand( new UndefineTermCmd( localName ) );
		}
	;

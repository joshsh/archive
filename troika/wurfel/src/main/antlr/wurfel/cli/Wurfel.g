header
{
package wurfel.cli;

import wurfel.WurfelException;

import wurfel.cli.ast.Ast;
import wurfel.cli.ast.BooleanNode;
import wurfel.cli.ast.BNodeNode;
import wurfel.cli.ast.DoubleNode;
import wurfel.cli.ast.IntNode;
import wurfel.cli.ast.NameNode;
import wurfel.cli.ast.NullNode;
import wurfel.cli.ast.QNameNode;
import wurfel.cli.ast.StringNode;
import wurfel.cli.ast.SequenceNode;
import wurfel.cli.ast.TypedLiteralNode;
import wurfel.cli.ast.UriNode;
}


////////////////////////////////////////////////////////////////////////////////


class WurfelLexer extends Lexer;

options
{
    k = 2;

    // Do not attempt to recover from lexer errors.
    defaultErrorHandler = false;

    //buildAST = true;
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
//            throw new Exception( "WurfelLexer has no caller to receive an event" );
            System.err.println( "WurfelLexer instance has not been initialized" );

        else if ( !interpreter.readLine() )
            throw new ParserQuitException();
    }

    private void updateCompletors( CompletorState state )
    {
        interpreter.updateCompletors( state );
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
        updateCompletors( CompletorState.NONE );
        interpreter.setLanguageTag( null );
      } ( NORMAL | DIGIT | SPECIAL | ESC | WS_CHAR )+ '\"'! ( LANGUAGE! )?
    ;

URI
    : '<'! ( NORMAL | DIGIT | SPECIAL_0 | WS_CHAR_NOBREAKS | "\\<" | "\\>" | "\\\\" )+ '>'!
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

BNODE_HEAD
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
    : ( DIGIT )+ ( '.' ( DIGIT )+ )?
    ;

COMMENT
    : "(:" ((~':') | (':' ~')'))* ":)"
        { $setType( Token.SKIP ); }
    ;

COMMENT2
    : ( '#' ( ~('\r' | '\n') )* )
        { $setType( Token.SKIP ); }
    ;

DOUBLE_HAT
options { paraphrase = "double hat"; } : "^^" ;

L_PAREN
options { paraphrase = "opening parenthesis"; } : '(' ;

R_PAREN
options { paraphrase = "closing parenthesis"; } : ')' ;

L_SQ_BRACE
options { paraphrase = "opening square bracket"; } : '[' ;

R_SQ_BRACE
options { paraphrase = "closing square bracket"; } : ']' ;

AND options
{ paraphrase = "conjunction"; } : '^' ;

OR
options { paraphrase = "disjunction"; } : '|' ;

SLASH
options { paraphrase = "exclusion"; } : '/' ;

PLUS
options { paraphrase = "plus quantifier"; } : '+' ;

STAR
options { paraphrase = "star quantifier"; } : '*' ;

CHOICE
options { paraphrase = "choice quantifier"; } : '?' ;

SEMI
options { paraphrase = "semicolon"; } : ';' ;

DOT
options { paraphrase = "dot operator"; } : '.' ;

COMMA
options { paraphrase = "comma"; } : ',' ;

COLON
options { paraphrase = "colon"; } : ':' ;

protected
DIRECTIVE_HEAD
    : '@'
    ;

ADD         : DIRECTIVE_HEAD ( "add"           | "a" ) ;
COUNT       : DIRECTIVE_HEAD ( "count"         | "c" ) ;
DEFINE      : DIRECTIVE_HEAD ( "define"        | "d" ) ;
GRAPHQUERY  : DIRECTIVE_HEAD ( "graphQuery"    | "g" ) ;
LIST        : DIRECTIVE_HEAD ( "list"          | "l" ) ;
PREFIX      : DIRECTIVE_HEAD ( "prefix"        | "p" ) ;
QUIT        : DIRECTIVE_HEAD ( "quit"          | "q"
                                   | "exit"          | "x" ) ;
SAVEAS      : DIRECTIVE_HEAD ( "saveas"        | "s" ) ;
//URI         : DIRECTIVE_HEAD ( "uri"           | "u" ) ;


////////////////////////////////////////////////////////////////////////////////


class WurfelParser extends Parser;
options
{
    k = 1;
    buildAST = false;

    // Do not attempt to recover from parser errors.
    defaultErrorHandler = false;
}


{
//    private boolean active = true;

    private Interpreter interpreter = null;

    public void initialize( Interpreter r )
    {
        interpreter = r;
    }
}


nt_Input
{
}
    : ( (WS)? nt_Statement )*
    ;


nt_Statement
{
    Ast r;
}
    : r=nt_Sequence (WS)? SEMI
        {
            interpreter.evaluate( r );
        }

    // Directives are executed lazily, as soon as the semicolon is encountered.
    | nt_Directive

    // Empty statements are simply ignored.
    | SEMI
    ;


nt_Sequence returns [ SequenceNode s ]
{
    Ast i;
    s = null;
}
    : i=nt_Item

      ( (WS ~(SEMI)) => ( WS s=nt_Sequence )
        {
            s.push( i );
        }
      |
        {
            s = new SequenceNode();
            s.push( i );
        }
      )
    ;


nt_Item returns [ Ast r ]
{
    r = null;
}
    : r=nt_Resource
    | r=nt_Literal
    | r=nt_ParenthesizedExpression
    | r=nt_QuantifiedItem
    | r=nt_IndexExpression
    ;


nt_ParenthesizedExpression returns [ Ast r ]
{
    r = new NullNode();
}
    : L_PAREN (WS)? ( r=nt_Sequence )? R_PAREN
    ;


nt_QuantifiedItem returns [ Ast r ]
{
    r = null;
}
    : DOT r=nt_Item
    | AND r=nt_Item
    | OR r=nt_Item
    | SLASH r=nt_Item
    | CHOICE r=nt_Item
    | STAR r=nt_Item
    | PLUS r=nt_Item
    ;


nt_IndexExpression returns [ Ast r ]
{
    Ast i;
    r = null;
}
    : L_SQ_BRACE (WS)? r=nt_Item (WS)? ( COMMA (WS)? i=nt_Item (WS)? )? R_SQ_BRACE
    ;


nt_Literal returns [ Ast r ]
{
    r = null;
    Ast dataType = null;
 //   String language = null;
}
    : ( t:STRING /*
        ( AMP*/

        ( /* Note: for symmetry with Turtle, the grammar allows any resource
                   reference as the data type of a literal (i.e. a URI or a blank
                   node).  However, the Sesame back end will only accept a URI. */
          ( DOUBLE_HAT dataType=nt_Resource )

    /*      | l:LANGUAGE { language = l.getText(); } */
        )?
      )
        {
            r = ( null == dataType )
                ? new StringNode( t.getText(), interpreter.getLanguageTag() )
                : new TypedLiteralNode( t.getText(), dataType );
        }
    | u:NUMBER
        {
            try
            {
                String s = u.getText();
                if ( s.contains( "." ) )
                    r = new DoubleNode( ( new Double( s ) ).doubleValue() );
                else
                    r = new IntNode( ( new Integer( s ) ).intValue() );
            }

            catch ( NumberFormatException e )
            {
                System.err.println( "a NumberFormattingException was encountered (this shouldn't happen)" );
                r = null;
                interpreter.quit();
            }
        }
    ;


nt_URIRef returns [ Ast r ]
{
    r = null;
}
    : uri:URI
        {
            r = new UriNode( uri.getText() );
        }
    ;


nt_Name returns [ String name ]
{
    name = null;
}
    : t1:NAME_OR_PREFIX { name = t1.getText(); }
    | t2:NAME_NOT_PREFIX { name = t2.getText(); }
    ;
/*
            if ( name.equals( "true" ) )
                r = new BooleanNode( true );
            else if ( name.equals( "false" ) )
                r = new BooleanNode( false );
            else
*/


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
    : localName=nt_Name { r = new NameNode( localName ); }
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
            r = new QNameNode( nsPrefix, localName );
        }
    ;


nt_BNode returns [ Ast r ]
{
    r = null;
    String localName = null;
}
    : BNODE_HEAD localName=nt_Name
        {
            r = new BNodeNode( localName );
        }
    ;


nt_Resource returns [ Ast r ]
{
    r = null;
}
    : r=nt_URIRef
    | ( (NAME_OR_PREFIX)? COLON ) => r=nt_QName
    | r=nt_SimpleName
    | r=nt_BNode
    ;


nt_Directive
{
    Ast subj, pred, obj;
    Ast rhs;
}
    : ADD WS subj=nt_Item WS pred=nt_Item WS obj=nt_Item (WS)? SEMI
        {
            interpreter.addStatement( subj, pred, obj );
        }

    | COUNT WS "statements" (WS)? SEMI
        {
            interpreter.countStatements();
        }

    | DEFINE WS name:NAME WS uri:STRING (WS)? SEMI
        {
            interpreter.define( name.getText(), uri.getText() );
        }

    | GRAPHQUERY WS query:STRING (WS)? SEMI
        {
            interpreter.evaluateGraphQuery( query.getText() );
        }

    | LIST WS
        ( ( "contexts" SEMI )
            {
                interpreter.showContexts();
            }
        | ( "prefixes" SEMI )
            {
                interpreter.showNamespaces();
            }
        )

    | PREFIX WS ( pre:NAME (WS)? )? COLON (WS)? rhs=nt_URIRef (WS)? SEMI
        {
// TODO
        }

    | QUIT (WS)? SEMI
        {
            interpreter.quit();
        }

    | SAVEAS WS file:STRING (WS)? SEMI
        {
            interpreter.saveAs( file.getText() );
        }
/*
    | URI uri:NAME COLON rhs=nt_Sequence SEMI
        {
            interpreter.evaluateAndDefine( rhs, uri.getText() );
        }
*/
    ;


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

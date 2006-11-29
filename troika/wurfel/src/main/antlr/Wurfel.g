header
{
package wurfel.parser;

import wurfel.ParserRunnable;
import wurfel.CompletorState;
import wurfel.Context;
import wurfel.WurfelException;

import wurfel.Node;
import wurfel.ApplyNode;
import wurfel.PrimitiveNode;
import wurfel.StringNode;
import wurfel.NameNode;

import java.util.List;
import java.util.ArrayList;

import java.net.URL;
import java.net.MalformedURLException;
}


////////////////////////////////////////////////////////////////////////////////


class WurfelLexer extends Lexer;

options
{
    // Lookahead of 2 avoids various "lexical nondeterminism" conflicts.
    k = 3;

    // Do not attempt to recover from lexer errors.
    defaultErrorHandler = false;

    //buildAST = true;
}

{
    private ParserRunnable parserRunnable = null;

    public void initialize( ParserRunnable r )
    {
        parserRunnable = r;
    }

    private void endOfLineEvent()
//        throws Exception
    {
        if ( null == parserRunnable )
//            throw new Exception( "WurfelLexer has no caller to receive an event" );
            System.err.println( "WurfelLexer instance has not been initialized" );

        else
            parserRunnable.readLine();
    }

    private void setCompletorState( CompletorState state )
    {
        parserRunnable.setCompletorState( state );
    }
}


// Ignore whitespace when it appears between tokens.
WS  :   (   ' '
        |   '\t'
        |   '\r' '\n' { newline(); }
        |   '\n'      { newline(); endOfLineEvent(); }
        )
        { $setType(Token.SKIP); } //ignore this token
    ;

protected
NORMAL
    : '#' | '$' | '%' | '\'' ',' | '-' | '.' | ('0' .. '9') | '<' | '=' | '>' | '@' | ('A' .. 'Z') | '[' | ']' | '^' | '_' | '`' | ('a' .. 'z') | '{' | '}' | '~'
//    | '\\' ( '\"' | '\\' | WS )
    ;

protected
SPECIAL
    : '!' | '&' | '(' | ')' | '*' | '+' | '/' | ';' | '?' | '|' | ':'
    ;

protected
ESC
    : '\\' SPECIAL
    ;

protected
EXTENDED
    : '/' | ':'
    ;

/*
(
  ([a-zA-Z][0-9a-zA-Z+\\-\\.]*:)?
  /{0,2}
  [0-9a-zA-Z;/?:@&=+$\\.\\-_!~*'()%]+
)?
(
  #
  [0-9a-zA-Z;/?:@&=+$\\.\\-_!~*'()%]+
)?
*/
/*
protected
URICHAR
    : '0'..'9' | 'a'..'z' | 'A'..'Z' | ':' | '.' | '-' | '_' | '/' //| ';' | '?'  | '@' | '&' | '=' | '+' | '$' | '!' | '~' | '*' | '\'' | '(' | ')' | '%' | ']'
    ;

IDENTIFIER
    : //(
        ( ( 'a'..'z' | 'A'..'Z' ) ( '0'..'9' | 'a'..'z' | 'A'..'Z' | '-' | '.' )* ':' )?
        ( "//" )?
        ( URICHAR )+
      //)?
      ( '#' ( URICHAR )+ )?
    ;
*/

STRING
    : '\"'! {
        setCompletorState( CompletorState.NONE );
      } ( NORMAL | SPECIAL | ESC | WS )+ '\"'!
    ;

IDENTIFIER
    : ( NORMAL | EXTENDED | ESC )+
    ;



/*
COMMENT
   : "(:"
*/

L_PAREN
options { paraphrase = "opening parenthesis"; } : '(' ;

R_PAREN
options { paraphrase = "closing parenthesis"; } : ')' ;

/*
OPEN_COMMENT
options { paraphrase = "open comment"; } : "(:" ;

CLOSE_COMMENT
options { paraphrase = "close comment"; } : ":)" ;
*/

AND options
{ paraphrase = "conjunction"; } : '&' ;

OR
options { paraphrase = "disjunction"; } : '|' ;

//WITHOUT
//options { paraphrase = "exclusion"; } : '/' ;

PLUS
options { paraphrase = "plus quantifier"; } : '+' ;

STAR
options { paraphrase = "star quantifier"; } : '*' ;

CHOICE
options { paraphrase = "choice quantifier"; } : '?' ;

SEMI
options { paraphrase = "semicolon"; } : ';' ;

COUNT   : "!count"  | "!c" ;
DEFINE  : "!define" | "!d" ;
IMPORT  : "!import" | "!i" ;
PRINT   : "!print"  | "!p" ;
RESOLVE : "!resolve" | "!r" ;
QUIT    : "!quit"   | "!q"
        | "!exit"   | "!x" ;


////////////////////////////////////////////////////////////////////////////////


class WurfelParser extends Parser;
options
{
    buildAST = true;

    // Do not attempt to recover from parser errors.
    defaultErrorHandler = false;
}


{
    private ParserRunnable parserRunnable = null;

    public void initialize( ParserRunnable r )
    {
        parserRunnable = r;
    }

    private URL strToURL( final String s )
        throws WurfelException
    {
        try
        {
            return new URL( s );
        }

        catch ( MalformedURLException e )
        {
            throw new WurfelException( e );
        }
    }
}


nt_Input
{
    Node q;
}
    : q=nt_Query
      ( nt_Input )?
    ;


nt_Query returns [ Node r ]
{
}
    : r=nt_Sequence SEMI
      {
        System.out.println( r.toString() );
      }
    | nt_Command SEMI
        {
r = null;
        }
    ;


nt_ParenthesizedExpression returns [ Node r ]
{
}
    : L_PAREN r=nt_Sequence R_PAREN
      ( nt_Quantifier )?
    ;


nt_Quantifier
{
}
    : CHOICE | STAR | PLUS
    ;


nt_Sequence returns [ Node r ]
{
    Node i;
}
    : r=nt_Item
      ( i=nt_Item
        {
            List<Node> left = new ArrayList<Node>();
            left.add( i );
            List<Node> right = new ArrayList<Node>();
            right.add( r );
            r = new ApplyNode( left, right );
        }
        | AND i=nt_Item
        | OR i=nt_Item
//        | WITHOUT i=nt_Item
      )*
    ;


nt_Item returns [ Node r ]
{
}
    : r=nt_Name
    | r=nt_String
    | r=nt_ParenthesizedExpression
    ;


nt_String returns [ StringNode r ]
{
}
    : t:STRING
      {
        r = new StringNode( t.getText() );
      }
    ;


nt_Name returns [ NameNode r ]
{
}
    : t:IDENTIFIER
      {
        r = new NameNode( t.getText() );
      }
    ;


nt_Command
{
}
    : COUNT "statements"
        {
            System.out.println( "" + parserRunnable.getContext().countStatements() );
        }

    | DEFINE name:IDENTIFIER uri:IDENTIFIER
        {
            parserRunnable.getContext().define( name.getText(), uri.getText() );
        }

    | IMPORT url:IDENTIFIER ( baseURI:IDENTIFIER )?
        {
            try
            {
                String baseUriStr = ( baseURI == null ) ? null : baseURI.getText();
                parserRunnable.getContext().add( strToURL( url.getText() ), baseUriStr );
            }

            catch ( WurfelException e )
            {
                System.err.println( e.getMessage() );
            }
        }

    | PRINT "statements"
        {
            try
            {
                parserRunnable.getContext().printStatements();
            }

            catch ( WurfelException e )
            {
                System.err.println( e.getMessage() );
            }
        }

    | QUIT
        {
            //return;
System.out.println( "You can't give up now..." );
        }

    | RESOLVE name0:IDENTIFIER
        {
            System.out.println(
                parserRunnable.getContext().resolve( name0.getText() ) );
        }
    ;


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

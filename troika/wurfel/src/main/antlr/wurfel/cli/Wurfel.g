header
{
package wurfel.cli;

import wurfel.Context;
import wurfel.WurfelException;

import wurfel.cli.ast.Ast;
import wurfel.cli.ast.DoubleNode;
import wurfel.cli.ast.IntNode;
import wurfel.cli.ast.StringNode;
import wurfel.cli.ast.IdentifierNode;
import wurfel.cli.ast.SequenceNode;
}


////////////////////////////////////////////////////////////////////////////////


class WurfelLexer extends Lexer;

options
{
    k = 3;

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

    private void setCompletorState( CompletorState state )
    {
        interpreter.setCompletorState( state );
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
    : '#' | '$' | '%' | '\'' | '-' | '<' | '=' | '>' | '@' | ('A' .. 'Z') | '&' | '_' | '`' | ('a' .. 'z') | '{' | '}' | '~'
//    | '\\' ( '\"' | '\\' | WS )
    ;

protected
DIGIT
    : ('0' .. '9')
    ;

protected
SPECIAL
    : '!' | '^' | '(' | ')' | '*' | '+' | '/' | ';' | '?' | '|' | ':' | '.' | '[' | ']' | ','
    ;

protected
ESC
    : '\\' SPECIAL
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
      } ( NORMAL | DIGIT | SPECIAL | ESC | WS )+ '\"'!
    ;

IDENTIFIER
    : ( NORMAL | ESC ) ( NORMAL | DIGIT | ESC )*
    ;

NUMBER
    : ( DIGIT )+ ( '.' ( DIGIT )+ )?
    ;

COMMENT
   : "(:" ((~':') | (':' ~')'))* ":)"
   { $setType(Token.SKIP); }
   ;

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

WITHOUT
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

protected
COMMAND
    : '!'
    ;

ADD     : COMMAND ( "add"       | "a" ) ;
COUNT   : COMMAND ( "count"     | "c" ) ;
DEFINE  : COMMAND ( "define"    | "d" ) ;
IMPORT  : COMMAND ( "import"    | "i" ) ;
PRINT   : COMMAND ( "print"     | "p" ) ;
RESOLVE : COMMAND ( "resolve"   | "r" ) ;
SAVEAS  : COMMAND ( "saveas"    | "s" ) ;
QUIT    : COMMAND ( "quit"      | "q"
                    | "exit"    | "x" ) ;


////////////////////////////////////////////////////////////////////////////////


class WurfelParser extends Parser;
options
{
//    buildAST = true;

    // Do not attempt to recover from parser errors.
    defaultErrorHandler = false;
}


{
    private Interpreter interpreter = null;

    public void initialize( Interpreter r )
    {
        interpreter = r;
    }
}


nt_Input
{
}
    : nt_Query
      ( nt_Input )?
    ;


nt_Query
{
    Ast r;
}
    : r=nt_Sequence SEMI
      {
        interpreter.evaluate( r );
      }

    // Note: commands are executed greedily, before the semicolon is encountered.
    | nt_Command SEMI

    // Empty queries are simply ignored.
    | SEMI
    ;


nt_Sequence returns [ Ast r ]
{
    Ast i;
    SequenceNode s;
}
    : i=nt_Item
        {
            s = new SequenceNode();
            s.add( i );
            r = s;
        }
      ( i=nt_Item
        {
            s.add( i );
        }
      )*
    ;


nt_Item returns [ Ast r ]
{
}
    : r=nt_Name
    | r=nt_Literal
    | r=nt_ParenthesizedExpression
    | r=nt_QuantifiedItem
    | r=nt_IndexExpression
    ;


nt_ParenthesizedExpression returns [ Ast r ]
{
}
    : L_PAREN r=nt_Sequence R_PAREN
    ;


nt_QuantifiedItem returns [ Ast r ]
{
}
    : DOT r=nt_Item
    | AND r=nt_Item
    | OR r=nt_Item
    | WITHOUT r=nt_Item
    | CHOICE r=nt_Item
    | STAR r=nt_Item
    | PLUS r=nt_Item
    ;


nt_IndexExpression returns [ Ast r ]
{
    Ast i;
}
    : L_SQ_BRACE r=nt_Item ( COMMA i=nt_Item )? R_SQ_BRACE
    ;


nt_Literal returns [ Ast r ]
{
}
    : t:STRING
        {
            r = new StringNode( t.getText() );
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


nt_Name returns [ Ast r ]
{
}
    : t:IDENTIFIER
        {
            r = new IdentifierNode( t.getText() );
        }
    ;


nt_Command
{
    Ast subj, pred, obj;
}
    : ADD subj=nt_Item pred=nt_Item obj=nt_Item
        {
            interpreter.addStatement( subj, pred, obj );
        }

    | COUNT "statements"
        {
            System.out.println( "" + interpreter.countStatements() );
        }

    | DEFINE name:IDENTIFIER uri:STRING
        {
            interpreter.define( name.getText(), uri.getText() );
        }

    | IMPORT url:STRING ( baseURI:STRING )?
        {
            String baseUriStr = ( baseURI == null ) ? null : baseURI.getText();
            interpreter.importModel( url.getText(), baseUriStr );
        }

    | PRINT "statements"
        {
            interpreter.printStatements();
        }

    | QUIT
        {
            interpreter.quit();
        }

    | RESOLVE name0:IDENTIFIER
        {
            interpreter.show( name0.getText() );
        }

    | SAVEAS file:STRING
        {
            interpreter.saveAs( file.getText() );
        }
    ;


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

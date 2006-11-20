header
{
    package wurfel.parser;

    import wurfel.ParserRunnable;
    import wurfel.CompletorState;
    import wurfel.Node;
    import wurfel.ApplyNode;
    import wurfel.PrimitiveNode;
    import wurfel.StringNode;
    import wurfel.NameNode;

    import java.util.List;
    import java.util.ArrayList;
}


////////////////////////////////////////////////////////////////////////////////


class WurfelLexer extends Lexer;

options
{
    // Lookahead of 2 avoids various "lexical nondeterminism" conflicts.
    k = 2;

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
      : '!' | '#' | '$' | '%' | '\'' ',' | '-' | '.' | ('0' .. '9') | ':' | '<' | '=' | '>' | '@' | ('A' .. 'Z') | '[' | ']' | '^' | '_' | '`' | ('a' .. 'z') | '{' | '}' | '~'
//    : ~( '\"' | '&' | '(' | ')' | '*' | '+' | '/' | ';' | '?' | '\\' | '_' | '|' )
    ;

protected
ESC : '\\' ( '\"' | '&' | '(' | ')' | '*' | '+' | '/' | ';' | '?' | '\\' | '|' )
    ;

STRING
    : '\"'! {
        setCompletorState( CompletorState.NONE );
      } ( NORMAL | ESC | WS )+ '\"'!
    ;

IDENTIFIER
    : ( NORMAL )+
    ;

/*
COMMENT
   : "(:"
*/

L_PAREN
options { paraphrase = "opening parenthesis"; } : '(' ;

R_PAREN
options { paraphrase = "closing parenthesis"; } : ')' ;

OPEN_COMMENT
options { paraphrase = "open comment"; } : "(:" ;

CLOSE_COMMENT
options { paraphrase = "close comment"; } : ":)" ;

AND options
{ paraphrase = "conjunction"; } : '&' ;

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


////////////////////////////////////////////////////////////////////////////////


class WurfelParser extends Parser;
options
{
    buildAST = true;

    // Do not attempt to recover from parser errors.
    defaultErrorHandler = false;
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
      | WITHOUT i=nt_Item )*
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


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

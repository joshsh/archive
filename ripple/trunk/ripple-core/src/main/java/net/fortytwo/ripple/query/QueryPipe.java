package net.fortytwo.ripple.query;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.cli.Interpreter;
import net.fortytwo.ripple.cli.ParserExceptionSink;
import net.fortytwo.ripple.cli.ParserQuitException;
import net.fortytwo.ripple.cli.RecognizerAdapter;
import net.fortytwo.ripple.cli.RecognizerEvent;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.query.commands.RippleQueryCmd;
import net.fortytwo.ripple.util.Buffer;
import net.fortytwo.ripple.util.Collector;
import net.fortytwo.ripple.util.CollectorHistory;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.Source;

import java.io.IOException;
import java.io.ByteArrayInputStream;
import java.io.InputStream;

/**
 * A simple query pipeline.  Each submitted String must be a complete, valid
 * expression.
 */
public class QueryPipe implements Sink<String>
{
	private RecognizerAdapter recognizerAdapter;
    private Sink<Exception> parserExceptionSink;

    private Buffer<RippleList> resultBuffer;
	
	private CollectorHistory<RippleList> queryResultHistory
		= new CollectorHistory<RippleList>( 2 );
	private boolean lastQueryContinued = false;

	public QueryPipe( final QueryEngine queryEngine, final Sink<RippleList> resultSink ) throws RippleException
	{
		resultBuffer = new Buffer<RippleList>( resultSink );
		final Object mutex = resultBuffer;

		final Collector<RippleList> nilSource = new Collector<RippleList>();
		nilSource.put( RippleList.NIL );

		// Handling of queries
		Sink<ListAst> querySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast ) throws RippleException
			{
//System.out.println( "### received: " + ast );
				synchronized ( mutex )
				{
					Source<RippleList> composedWith = lastQueryContinued
							? queryResultHistory.getSource( 1 ) : nilSource;

					ModelConnection mc = queryEngine.getConnection();
                    try {
                        new RippleQueryCmd( ast, resultBuffer, composedWith ).execute( queryEngine, mc );
                    } finally {
                        mc.close();
                    }

                    lastQueryContinued = false;
					queryResultHistory.advance();
				}
			}
		};

		// Handling of "continuing" queries
		Sink<ListAst> continuingQuerySink = new Sink<ListAst>()
		{
			public void put( final ListAst ast ) throws RippleException
			{
				synchronized ( mutex )
				{
					Source<RippleList> composedWith = lastQueryContinued
							? queryResultHistory.getSource( 1 ) : nilSource;

					ModelConnection mc = queryEngine.getConnection();
                    try {
                        new RippleQueryCmd( ast, resultBuffer, composedWith ).execute( queryEngine, mc );
                    } finally {
                        mc.close();
                    }

                    lastQueryContinued = true;
					queryResultHistory.advance();
				}
			}
		};

		// Handling of commands
		Sink<Command> commandSink = new Sink<Command>()
		{
			public void put( final Command cmd ) throws RippleException
			{
				ModelConnection mc = queryEngine.getConnection();
                try {
                    cmd.execute( queryEngine, mc );
                } finally {
                    mc.close();
                }
            }
		};

		// Handling of parser events
		Sink<RecognizerEvent> eventSink = new Sink<RecognizerEvent>()
		{
			public void put( final RecognizerEvent event )
				throws RippleException
			{
				/*if ( RecognizerEvent.QUIT == event )
				{
					throw new ParserQuitException();
				}*/
				
				// (ignore other events)
			}
		};
		
		recognizerAdapter = new RecognizerAdapter(
				querySink, continuingQuerySink, commandSink, eventSink, queryEngine.getErrorPrintStream() );

		parserExceptionSink = new ParserExceptionSink(
				queryEngine.getErrorPrintStream() );
	}
	
	public void close() throws RippleException
	{
	}
	
	public void put( final String expr ) throws RippleException
	{
//System.out.println("interpreting query: " + expr);
        InputStream is = new ByteArrayInputStream( expr.getBytes() );

        // TODO: creating a new Interpreter for each expression is not very efficient
        Interpreter interpreter = new Interpreter( recognizerAdapter, is, parserExceptionSink );
        interpreter.parse();

        try {
            is.close();
        } catch ( IOException e ) {
            throw new RippleException( e );
        }

		resultBuffer.flush();
	}
}

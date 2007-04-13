package net.fortytwo.ripple.cli;

import java.io.InputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.PrintStream;
import java.io.OutputStream;
import java.io.FileOutputStream;

import java.net.URL;

import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import java.util.Observable;
import java.util.Observer;

import jline.Completor;
import jline.MultiCompletor;
import jline.ConsoleReader;

import org.openrdf.model.Literal;
import org.openrdf.model.Resource;
import org.openrdf.model.Value;
import org.openrdf.model.Statement;
import org.openrdf.model.URI;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.Container;
import net.fortytwo.ripple.model.ContainerSink;
import net.fortytwo.ripple.model.Dereferencer;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Evaluator;
import net.fortytwo.ripple.model.LazyEvaluator;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.Model;
//import net.fortytwo.ripple.model.DebugEvaluator;
import net.fortytwo.ripple.model.ObservableContainer;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RipplePrintStream;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.model.ListContainerSink;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.cli.ast.ListAst;
import net.fortytwo.ripple.cli.ast.UriAst;
import net.fortytwo.ripple.util.Sink;

import org.apache.log4j.Logger;

/**
*  Console input:
*    System.in --> reader --> readOut --> writeIn --> RippleLexer
*
*  Normal output:
*    valueSetObserver --> printStream --> System.out
*    evaluateGraphQuery() --> printStream --> System.out
*
*  Error output:
*    alert() --> errorPrintStream = System.err
*/
public class Interpreter extends Thread implements Observer
{
	private final static Logger s_logger
		= Logger.getLogger( Interpreter.class );

	private Model model;
	private Evaluator evaluator;

	private PipedInputStream  writeIn;
	private PipedOutputStream readOut;

	private ConsoleReader reader;
	private int lineNumber = 0;

	private RipplePrintStream printStream;
	private PrintStream errorPrintStream;

	private ObservableContainer valueSet;
	private ContainerTreeView valueSetObserver;

	private QueryContext queryContext;

	////////////////////////////////////////////////////////////////////////////

	// A helper variable for the lexer and parser.
	private String languageTag;

	public String getLanguageTag()
	{
		return languageTag;
	}

	public void setLanguageTag( final String tag )
	{
		languageTag = tag;
	}

	private void chooseEvaluator()
	{
		switch ( Ripple.getEvaluationStyle() )
		{
			case APPLICATIVE:
// TODO: not implemented
				break;

			case COMPOSITIONAL:
				evaluator = new LazyEvaluator();
				break;
		}
//        evaluator = new DebugEvaluator( new LazyEvaaluator( model ) );
	}

	////////////////////////////////////////////////////////////////////////////

	public Interpreter( Model model ) throws RippleException
	{
		this.model = model;

		chooseEvaluator();

		String jLineDebugOutput = Ripple.getJLineDebugOutput();

		try
		{
			reader = new ConsoleReader();

			if ( null != jLineDebugOutput )
				reader.setDebug(
					new PrintWriter(
						new FileWriter( jLineDebugOutput, true ) ) );
		}

		catch ( Throwable t )
		{
			throw new RippleException( t );
		}

		try
		{
			writeIn = new PipedInputStream();
			readOut = new PipedOutputStream( writeIn );
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		errorPrintStream = System.err;

		queryContext = new QueryContext( model, System.out, errorPrintStream );
		queryContext.getLexicon().addObserver( this );

		valueSet = new ObservableContainer( model, null );
		printStream = queryContext.getPrintStream();
		valueSetObserver = new ContainerTreeView( valueSet, printStream );
		valueSet.addObserver( valueSetObserver );

		update( queryContext.getLexicon(), null );
	}

	////////////////////////////////////////////////////////////////////////////

	private void updateCompletors( final Lexicon lexicon )
	{
		s_logger.debug( "updating completors" );
		List completors = new ArrayList();

		try
		{
			completors.add( lexicon.getCompletor() );

			ArrayList<String> directives = new ArrayList<String>();
			directives.add( "@count" );
			directives.add( "@define" );
			directives.add( "@export" );
			directives.add( "@help" );
			directives.add( "@list" );
			directives.add( "@prefix" );
			directives.add( "@quit" );
			directives.add( "@saveas" );
			directives.add( "@serql" );
			directives.add( "@undefine" );

			completors.add(
				new net.fortytwo.ripple.cli.jline.DirectiveCompletor(
					directives ) );

			try
			{
				// This makes candidates from multiple completors available at once.
				Completor multiCompletor = new MultiCompletor( completors );

				reader.addCompletor( multiCompletor );
			}

			catch ( Throwable t )
			{
				throw new RippleException( t );
			}
		}

		catch ( RippleException e )
		{
			s_logger.error( "Failed to update completors.  Continuing nonetheless." );
		}
	}

	public void readLine()
	{
System.out.println( "readLine()" );System.out.flush();
		try
		{
			++lineNumber;
			String line = reader.readLine( "" + lineNumber + " >>  " );
	
			if ( null != line )
			{
				byte[] bytes = line.getBytes();
				readOut.write( bytes, 0, bytes.length );
	
				// Add a deliberate "end of line" character so the lexer knows
				// to call readLine() again when it gets there.
				byte[] terminator = { '\n' };
				readOut.write( terminator, 0, 1 );
	
				readOut.flush();
			}
		}

		// This has never happened.
		catch ( java.io.IOException e )
		{
			alert( "IOException: " + e.toString() );
		}
	}

	private void runPrivate()
	{
// TODO: revisit parser error recovery
		// Break out when a @quit directive is encountered
		for (;;)
		{
System.out.println( "--- 0 ---" );
			readLine();
System.out.println( "--- 1 ---" );
			RippleLexer lexer = new RippleLexer( writeIn );
			lexer.initialize( this );
			RippleParser parser = new RippleParser( lexer );
			parser.initialize( this );
System.out.println( "--- 2 ---" );

			try
			{
				parser.nt_Document();

				// If the parser has exited normally, then we're done.
				break;
			}

			catch ( antlr.RecognitionException e )
			{
				// Report the error, then begin parsing again.
				alert( "RecognitionException: " + e.toString() );
			}

			catch ( antlr.TokenStreamException e )
			{
				// Report the error, then begin parsing again.
				alert( "TokenStreamException: " + e.toString() );
			}

			catch ( ParserQuitException e )
			{
				s_logger.debug( "quit() called on Interpreter" );

				// The user has instructed the parser to quit.
				break;
			}
System.out.println( "--- 3 ---" );
		}
	}

	public void run()
	{
		try
		{
			s_logger.debug( "running Interpreter as a new thread" );
			runPrivate();
		}

		catch ( Throwable t )
		{
			alert( "Error: " + t.toString() );

			new RippleException( t );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public void evaluate( ListAst ast )
	{
		try
		{
			queryContext.getLexicon().suspendEventHandling();
			valueSetObserver.suspendEventHandling();
			evaluatePrivate( ast );
			queryContext.getLexicon().resumeEventHandling();
			valueSetObserver.resumeEventHandling();
		}

		catch ( RippleException e )
		{
			alert( "Error: " + e.getMessage() );
		}
	}

	// E.g.
	//      CONSTRUCT * FROM {x} p {y}
	public void evaluateGraphQuery( final String query )
	{
		ModelConnection mc = null;

		try
		{
			mc = new ModelConnection( model, "for evaluateGraphQuery" );
			Iterator<Statement> stmtIter = mc.graphQuery( query ).iterator();

			printStream.println( "" );
			printStream.print( stmtIter );
			printStream.println( "" );

			mc.close();
		}

		catch ( RippleException e )
		{
			try
			{
				mc.close();
			}

			catch ( RippleException e2 )
			{
				// ...
			}

			alert( "Error: " + e.getMessage() );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	private void dereferenceResultSet( Collection<RippleValue> values, ModelConnection mc )
		throws RippleException
	{
		Dereferencer d = model.getDereferencer();

		Iterator<RippleValue> iter = values.iterator();
		while ( iter.hasNext() )
		{
			RippleValue value = iter.next();
if ( value instanceof RippleList )
value = ( (net.fortytwo.ripple.model.RippleList) value ).getFirst();

			try
			{
				d.dereference( value.toRdf( mc ), mc );
			}

			catch ( RippleException e )
			{
				// (soft fail)
			}
		}
	}

	private void evaluatePrivate( ListAst ast )
		throws RippleException
	{
		ModelConnection mc = null;

		try
		{
			mc = new ModelConnection( model, "for Interpreter evaluate()" );

			ContainerSink expressions = new ContainerSink();
			ast.evaluate( expressions, queryContext, mc );

			ListContainerSink evaluatedExpressions = new ListContainerSink();
			for ( Iterator<RippleValue> iter = expressions.iterator(); iter.hasNext(); )
			{
				RippleValue expr = iter.next();
				RippleList list = ( expr instanceof RippleList )
					? (RippleList) expr
					: new RippleList( expr );
//System.out.println( "applying to: " + list );
	
				evaluator.applyTo( list, evaluatedExpressions, mc );
			}

// TODO: this should dereference as many levels as Ripple.getTreeViewDepth(),
//       and should probably be moved into the tree view itself if possible.
			dereferenceResultSet( evaluatedExpressions, mc );
	
			valueSet.setValues( evaluatedExpressions );

			mc.close();
		}

		catch ( RippleException e )
		{
			mc.close();
			throw e;
		}
	}

	private void alert( String s )
	{
		errorPrintStream.println( "\n" + s + "\n" );
	}

	////////////////////////////////////////////////////////////////////////////

	public void update( Observable o, Object arg )
	{
		if ( o instanceof Lexicon )
			updateCompletors( (Lexicon) o );
	}

	////////////////////////////////////////////////////////////////////////////

	public void put( final Command cmd )
	{
		ModelConnection mc = null;
		boolean connectionEstablished = false, commandCompleted = false;

		try
		{
			mc = new ModelConnection( model, "Command" );
			connectionEstablished = true;
			cmd.execute( queryContext, mc );
			commandCompleted = true;
			mc.close();
		}

		catch ( ParserQuitException e )
		{
			try
			{
				mc.close();
			}

			catch ( RippleException e2 )
			{
				alert( "Error: failed to close connection" );
			}

			throw e;
		}

		catch ( RippleException e )
		{
			alert( "Error: " + e.getMessage() );

			if ( !connectionEstablished )
				alert( "Error: failed to establish connection" );

			else if ( !commandCompleted )
			{
				try
				{
					mc.close();
				}

				catch ( RippleException e2 )
				{
					alert( "Error: failed to close connection" );
				}
			}

			else
				alert( "Error: failed to close connection" );
		}
	}
}

// kate: tab-width 4

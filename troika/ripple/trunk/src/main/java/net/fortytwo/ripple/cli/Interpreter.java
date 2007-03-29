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
*    System.in --> reader --> readOut --> writeIn --> lexer
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

public Model getModel()
{
	return model;
}

	private PipedInputStream  writeIn;
	private PipedOutputStream readOut;

	private PrintWriter out;

	private ConsoleReader reader;
	private int lineNumber = 0;

	private RipplePrintStream printStream;
	private PrintStream errorPrintStream;

	private ObservableContainer valueSet;
	private ContainerTreeView valueSetObserver;

	private Lexicon lexicon;

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

		lexicon = new Lexicon( model );
		lexicon.addObserver( this );

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

			out = new PrintWriter( System.out );
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}

		errorPrintStream = System.err;

		valueSet = new ObservableContainer( model, null );
ModelConnection mc = new ModelConnection( model, "for ConsoleValueSet constructor" );
		printStream = new RipplePrintStream( System.out, lexicon, mc );
		valueSetObserver = new ContainerTreeView( valueSet, printStream );
mc.close();
		valueSet.addObserver( valueSetObserver );

		update( lexicon, null );
	}

	////////////////////////////////////////////////////////////////////////////

	public void updateCompletors()
	{
System.out.println( "########## updating completors" );
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

	public boolean readLine()
	{
		try
		{
			++lineNumber;
			String line = reader.readLine( "" + lineNumber + " >>  " );

			if ( null == line )
				return false;

			else
			{
				byte[] bytes = line.getBytes();
				readOut.write( bytes, 0, bytes.length );

				// Add a deliberate "end of line" character so the lexer knows
				// to call readLine() again when it gets there.
				byte[] terminator = { '\n' };
				readOut.write( terminator, 0, 1 );

				out.flush();
			}

			return true;
		}

		catch( java.io.IOException e )
		{
			new RippleException( e );
			return false;
		}
	}

	public void quit()
	{
		s_logger.debug( "quit() called on Interpreter" );
		throw new ParserQuitException();
	}

	private void runPrivate() throws Throwable
	{
		if ( !readLine() )
			return;

// TODO: revisit parser error recovery
		while ( true )
		{
			RippleLexer lexer = new RippleLexer( writeIn );
			lexer.initialize( this );
			RippleParser parser = new RippleParser( lexer );
			parser.initialize( this );

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

			catch ( antlr.TokenStreamRecognitionException e )
			{
				// Report the error, then begin parsing again.
				alert( "TokenStreamRecognitionException: " + e.toString() );
			}

			catch ( ParserQuitException e )
			{
				// The user has instructed the parser to quit.
				break;
			}
		}
	}

	public void run()
	{
		try
		{
			s_logger.debug( "running Interpreter in a new thread" );
			runPrivate();
		}

		catch ( Throwable t )
		{
			alert( "Error: " + t.toString() );

			new RippleException( t );
		}
	}

	////////////////////////////////////////////////////////////////////////////

	public void countStatements()
	{
		try
		{
			System.out.println( "\n" + model.countStatements() + "\n" );
		}

		catch ( RippleException e ) {}
	}

	public void showContexts()
	{
		try
		{
//            valueSet.setValues( model.getContexts() );

// FIXME: this is a kludge to keep the print stream from using namespace prefixes instead of full URI references
			printStream.println( "" );
			Iterator<RippleValue> models = model.getContexts().iterator();
			int i = 0;
			while ( models.hasNext() )
			{
				printStream.print( "[" + i++ + "] " );
				printStream.println( models.next() );
			}
			printStream.println( "" );
		}

		catch ( RippleException e ) {}
	}

	private void saveAsPrivate( String fileName )
		throws RippleException
	{
		OutputStream out;

		try
		{
			out = new FileOutputStream( fileName );
		}

		catch ( java.io.FileNotFoundException e )
		{
			throw new RippleException( e );
		}

		model.writeTo( out );
//        model.writeTrix( out );

		try
		{
			out.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}

	private void exportNsPrivate( String nsPrefix, String fileName )
		throws RippleException
	{
		OutputStream out;

		String ns = lexicon.resolveNamespacePrefix( nsPrefix );
		if ( null == ns )
			throw new RippleException( "namespace prefix '" + nsPrefix + "' is not defined" );

		try
		{
			out = new FileOutputStream( fileName );
		}

		catch ( java.io.FileNotFoundException e )
		{
			throw new RippleException( e );
		}

		ModelConnection mc = new ModelConnection( model );

		try
		{
			mc.exportNs( ns, out );
		}

		catch ( RippleException e )
		{
			mc.close();
			throw e;
		}

		mc.close();

		try
		{
			out.close();
		}

		catch ( java.io.IOException e )
		{
			throw new RippleException( e );
		}
	}

	public void saveAs( final String fileName )
	{
		try
		{
			saveAsPrivate( fileName );

			System.out.println( "\nSaved data set as " + fileName + "\n" );
		}

		catch ( RippleException e ) {}
	}

	public void exportNs( final String nsPrefix, final String fileName )
	{
		try
		{
			exportNsPrivate( nsPrefix, fileName );

			System.out.println( "\nExported namespace " + nsPrefix + " to " + fileName + "\n" );
		}

		catch ( RippleException e ) {}
	}

	public void setNamespace( final String prefix, final UriAst uri )
	{
		ModelConnection mc = null;

		try
		{
			mc = new ModelConnection( model, "for setNamespace" );
			ContainerSink sink = new ContainerSink();
			uri.evaluate( sink, this, mc );
			if ( sink.size() == 0 )
				throw new RippleException( "URI could not be constructed from " + uri );
			else if ( sink.size() > 1 )
				throw new RippleException( "multiple values constructed from " + uri );

			URI ns = mc.uriValue( sink.iterator().next() );
			mc.setNamespace( prefix, ns );
			mc.close();
			mc = null;

			lexicon.update();
		}

		catch ( RippleException e )
		{
			if ( null != mc )
			{
				try
				{
					mc.close();
				}

				catch ( RippleException e2 )
				{
					// ...
				}
			}

			alert( "Error: " + e.getMessage() );
		}
	}

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

	public void resolveKeyword( final String localName, Sink<RippleValue> sink )
		throws RippleException
	{
		Collection<URI> options = lexicon.resolveKeyword( localName );
		if ( 0 == options.size() )
			errorPrintStream.println( "Warning: no values resolved for " + localName );
		else if ( 1 < options.size() )
			errorPrintStream.println( "Warning: multiple values resolved for " + localName );

		for ( Iterator<URI> optIter = options.iterator(); optIter.hasNext(); )
			sink.put( model.getBridge().get(
				new RdfValue( optIter.next() ) ) );
	}

	public void resolveQName( final String nsPrefix,
								final String localName,
								final Sink<RippleValue> sink )
		throws RippleException
	{
		Value v = lexicon.resolveQName( nsPrefix, localName );

		if ( null == v )
			errorPrintStream.println( "Warning: no values resolved for " + nsPrefix + ":" + localName );

		else
			sink.put( model.getBridge().get(
				new RdfValue( v ) ) );
	}

	private String getDefaultNamespace()
		throws RippleException
	{
		String defaultNs = lexicon.resolveNamespacePrefix( "" );

		if ( null == defaultNs )
			throw new RippleException( "no default namespace is defined.  Use '@prefix : <...>.'" );

		return defaultNs;
	}

	private void evaluate( ListAst ast, final String name )
		throws RippleException
	{
		ModelConnection mc = null;

		try
		{
			mc = new ModelConnection( model, "for Interpreter evaluate()" );

			ContainerSink expressions = new ContainerSink();
			ast.evaluate( expressions, this, mc );

			// Define the term *before* reduction.
			if ( null != name && expressions.size() > 0 )
			{
				if ( expressions.size() > 1 )
					errorPrintStream.println( "Warning: expression has multiple values.  Choosing the first value for assignment." );

				RippleValue expr = expressions.iterator().next();

				if ( !( expr instanceof RippleList ) )
					throw new RippleException( "term assignment for non-lists is not implemented" );

				RippleList exprList = (RippleList) expr;

// TODO: check for collision with an existing URI
				URI uri = mc.createUri( getDefaultNamespace() + name );

				mc.copyStatements( exprList.toRdf( mc ), new RdfValue( uri ) );

				lexicon.update();
			}

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

	public void undefine( final String localName )
	{
		try
		{
			ModelConnection mc = new ModelConnection( model, "for Interpreter undefine()" );

			mc.removeStatementsAbout(
				mc.createUri( getDefaultNamespace() + localName ) );

// TODO: close on Exception
			mc.close();
		}

		catch ( RippleException e )
		{
			alert( "Error: " + e.getMessage() );
		}
	}

	public void evaluate( ListAst ast )
	{
		try
		{
			lexicon.suspendEventHandling();
			valueSetObserver.suspendEventHandling();
			evaluate( ast, null );
			lexicon.resumeEventHandling();
			valueSetObserver.resumeEventHandling();
		}

		catch ( RippleException e )
		{
			alert( "Error: " + e.getMessage() );
		}
	}

	public void evaluateAndDefine( ListAst ast, String name )
	{
		try
		{
			lexicon.suspendEventHandling();
			evaluate( ast, name );
			lexicon.resumeEventHandling();
		}

		catch ( RippleException e )
		{
			alert( "Error: " + e.getMessage() );
		}
	}

	public void showNamespaces()
	{
		try
		{
			model.showNamespaces();
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

	private void alert( String s )
	{
		errorPrintStream.println( "\n" + s + "\n" );
	}

	////////////////////////////////////////////////////////////////////////////

	public void update( Observable o, Object arg )
	{
		if ( o == lexicon )
			updateCompletors();
	}
}

// kate: tab-width 4

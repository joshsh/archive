package net.fortytwo.ripple.cli;

import net.fortytwo.ripple.Ripple;
import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.Lexicon;
import net.fortytwo.ripple.model.Model;
import net.fortytwo.ripple.model.ObservableContainer;
import net.fortytwo.ripple.model.RipplePrintStream;

import org.openrdf.model.Resource;
import org.openrdf.model.URI;
import org.openrdf.model.Value;

import java.util.Collection;
import java.util.Iterator;
import java.util.Observer;
import java.util.Observable;
import java.util.Set;

public class ContainerTreeView implements Observer
{
    private ObservableContainer valueSet;
    private RipplePrintStream ps;

    private Lexicon lexicon;

    // A three-space-indented tree seems to be the most readable.
    private static final String indent = "   ";

    private static final String indexSeparator = "  ";

    private static final int maxDepth = Ripple.getTreeViewDepth();

    public ContainerTreeView( ObservableContainer valueSet, RipplePrintStream printStream )
        throws RippleException
    {
        this.valueSet = valueSet;
        this.lexicon = lexicon;
        ps = printStream;
    }

    private void printTreeView( Value subject, int depth, String wsPrefix, ModelConnection mc )
        throws RippleException
    {
        if ( depth != maxDepth )
            ps.print( wsPrefix );

        for ( int i = 0; i < ( maxDepth - depth ) * 2; i++ )
            ps.print( indent );

        ps.print( subject );
        ps.print( "\n" );

        if ( depth > 0 )
        {
if ( subject instanceof net.fortytwo.ripple.model.ListNode )
subject = ( (net.fortytwo.ripple.model.ListNode<Value>) subject ).getFirst();

            if ( subject instanceof Resource )
            {
                Set<URI> predicates = mc.getPredicates( (Resource) subject );
                Iterator<URI> predIter = predicates.iterator();

                while ( predIter.hasNext() )
                {
                    URI predicate = predIter.next();

                    ps.print( wsPrefix );

                    for ( int i = 0; i < 1 + ( maxDepth - depth ) * 2; i++ )
                        ps.print( indent );

                    ps.print( predicate );
                    ps.print( "\n" );

                    Iterator<Value> objIter = valueSet.getModel().multiply( subject, predicate, mc ).iterator();

                    while ( objIter.hasNext() )
                        printTreeView( objIter.next(), depth - 1, wsPrefix, mc );
                }
            }
        }
    }

    private void refresh()
        throws RippleException
    {
        Model model = valueSet.getModel();
        Collection<Value> values = valueSet.getValues();

        if ( 0 < values.size() )
            ps.println( "" );

        int treeViewDepth = Ripple.getTreeViewDepth();
        ModelConnection mc = new ModelConnection( model, "for ContainerTreeView refresh()" );

        try
        {
            int index = 0;
            Iterator<Value> valuesIter = values.iterator();
            while ( valuesIter.hasNext() )
            {
//                String indexPrefix = "_" + ++index + indexSeparator;
                String indexPrefix = "rdf:_" + ++index + indexSeparator;
//                String wsPrefix = "                ".substring( 0, indexPrefix.length() );
                String wsPrefix = "";

                ps.print( indexPrefix );

                printTreeView( valuesIter.next(), treeViewDepth, wsPrefix, mc );
            }
        }

        catch ( RippleException e )
        {
            mc.close();
            throw e;
        }

        mc.close();

        ps.print( "\n" );
    }

    ////////////////////////////////////////////////////////////////////////////

    private boolean changed = false;
    private boolean suspended = false;

    public synchronized void suspendEventHandling()
    {
        suspended = true;
    }

    public synchronized void resumeEventHandling()
        throws RippleException
    {
        if ( suspended )
        {
            if ( changed )
            {
                refresh();

                changed = false;
            }

            suspended = false;
        }
    }

    public void update( Observable o, Object arg )
    {
        try
        {
            if ( o == valueSet )
            {
                if ( suspended )
                    changed = true;

                else
                    refresh();
            }
        }

        catch ( RippleException e )
        {
            System.err.println( "\nError: " + e.toString() + "\n" );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

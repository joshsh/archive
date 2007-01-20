package wurfel.model;

import wurfel.Context;
import wurfel.WurfelException;

import org.openrdf.repository.Connection;

public class EvaluationContext
{
    private Context context;
    private Connection connection;

    public EvaluationContext( Context context )
        throws WurfelException
    {
        this.context = context;

        try
        {
            connection = context.getRepository().getConnection();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }

    public Context getContext()
    {
        return context;
    }

    public Connection getConnection()
    {
        return connection;
    }

    public void close()
        throws WurfelException
    {
        try
        {
            connection.close();
        }

        catch ( Throwable t )
        {
            throw new WurfelException( t );
        }
    }
}

// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

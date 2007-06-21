package net.fortytwo.ripple.extensions.media;

	import javax.swing.*;
	import java.awt.*;
	import javax.imageio.*;

import java.io.InputStream;

import java.net.URL;
import java.net.URLConnection;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RdfValue;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.model.RippleValue;
import net.fortytwo.ripple.util.Sink;
import net.fortytwo.ripple.util.HttpUtils;

import org.openrdf.model.URI;

public class Show extends PrimitiveFunction
{
	public Show( RdfValue v, ModelConnection mc )
		throws RippleException
	{
		super( v, mc );
	}

	public int arity()
	{
		return 1;
	}

	public void applyTo( RippleList stack,
						Sink<RippleList> sink,
						ModelConnection mc )
		throws RippleException
	{
		URI uri;

		uri = mc.uriValue( stack.getFirst() );
		stack = stack.getRest();

		ImagePanel panel;

		try
		{
			panel = new ImagePanel( uri );
		}

		catch ( RippleException e )
		{
			// Discard the exception (it has already been logged).
			return;
		}

		JFrame f = new JFrame();

		f.getContentPane().add( panel );
		
		int width = panel.img.getWidth( null );
		int height = panel.img.getHeight( null );

		//show frame
		f.setBounds( 0, 0, width, height );
		f.setVisible( true );
	}

	static String[] mimeTypes = ImageIO.getReaderMIMETypes();

	//panel used to draw image on
	private class ImagePanel extends JPanel
	{
		//path of image
		private String path;
		
		//image object
		public Image img;
		
		public ImagePanel( final URI uri )
			throws RippleException
		{
			//save path
			this.path = path;	
			
			try
			{
				URL url = new URL( uri.toString() );
				URLConnection conn = url.openConnection();
				HttpUtils.prepareUrlConnectionForRequest( conn, mimeTypes );
				conn.connect();
				InputStream is = conn.getInputStream();

				//load image
				img = ImageIO.read( is );

				is.close();
			}

			catch ( Exception e )
			{
				throw new RippleException( e );
			}
		}
		
		//override paint method of panel
		public void paint( Graphics g )
		{
			//draw the image
			if( img != null )
				g.drawImage( img, 0, 0, this );
		}
	}
}

// kate: tab-width 4

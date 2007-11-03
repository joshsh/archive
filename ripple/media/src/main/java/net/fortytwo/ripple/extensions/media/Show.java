/*
 * $URL$
 * $Revision$
 * $Author$
 *
 * Copyright (C) 2007 Joshua Shinavier
 */


package net.fortytwo.ripple.extensions.media;

import java.awt.Graphics;
import java.awt.Image;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

import javax.imageio.ImageIO;
import javax.swing.JFrame;
import javax.swing.JPanel;

import net.fortytwo.ripple.RippleException;
import net.fortytwo.ripple.model.ModelConnection;
import net.fortytwo.ripple.model.PrimitiveFunction;
import net.fortytwo.ripple.model.RippleList;
import net.fortytwo.ripple.util.HttpUtils;
import net.fortytwo.ripple.util.Sink;

import org.openrdf.model.URI;

public class Show extends PrimitiveFunction
{
	private static final int ARITY = 1;

	private static String[] mimeTypes = ImageIO.getReaderMIMETypes();

	public Show()
		throws RippleException
	{
		super();
	}

	public int arity()
	{
		return ARITY;
	}

	public void applyTo( RippleList stack,
						final Sink<RippleList> sink,
						final ModelConnection mc )
		throws RippleException
	{
		URI uri;

		uri = mc.uriValue( stack.getFirst() );
//		stack = stack.getRest();

		ImagePanel panel;

		try
		{
			panel = new ImagePanel( uri );

			JFrame f = new JFrame();
	
			f.getContentPane().add( panel );
			
			int width = panel.img.getWidth( null );
			int height = panel.img.getHeight( null );
	
			//show frame
			f.setBounds( 0, 0, width, height );
			f.setVisible( true );
		}

		catch ( RippleException e )
		{
			e.logError();
		}

		// Pass the stack along, unaltered.
		sink.put( stack );
	}

	//panel used to draw image on
	private class ImagePanel extends JPanel
	{
		private static final long serialVersionUID = -1290238560297598746L;
		
		//image object
		public Image img;
		
		public ImagePanel( final URI uri )
			throws RippleException
		{
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

			if ( null == img )
				throw new RippleException( "no displayable image found at URI " + uri );
		}
		
		//override paint method of panel
		public void paint( final Graphics g )
		{
			//draw the image
			if( img != null )
			{
				g.drawImage( img, 0, 0, this );
			}
		}
	}
}

// kate: tab-width 4

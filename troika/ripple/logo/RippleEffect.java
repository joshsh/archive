import java.awt.Color;
import java.awt.Toolkit;
import java.awt.Graphics;
import java.awt.Image;

import java.awt.geom.Point2D;
import java.awt.geom.Point2D.Double;

import java.awt.image.MemoryImageSource;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class RippleEffect
{
	Color [][] matrix;
	int width, height;

	Point2D mid;
	Point2D[] centers;
	double radius, rotateBy, frequencyFactor;
	double radiusRest, tanFactor;

	Color[] colors;
	Color backgroundColor;

	Image image;

	double dampen( double distance )
	{
		return Math.cos( ( distance / radiusRest ) * Math.PI / 2.0 );
	}

	double waveFunc( double distance )
	{
		if ( distance >= radiusRest )
			return 0;
		else
			return
				dampen( distance ) *
				Math.cos(
					frequencyFactor * Math.tan( distance * tanFactor ) );
//				dampen( distance ) *
//				( 1.0 + Math.cos(
//					frequencyFactor * Math.tan( distance * tanFactor ) ) ) / 2.0;
//				* ( 1.0 + Math.cos( frequencyFactor / ( radiusRest - distance ) ) ) / 2.0;
	}

	double intensify( double val, int n )
	{
		for ( int i = 0; i < n; i++ )
			val = Math.sin( val * Math.PI / 2.0 );

		return val;
	}

/*
	double fieldFunc( Point2D point )
	{
		
	}
*/

	void createPoints()
	{
		radiusRest = 0.5 - radius;
		tanFactor = Math.PI / ( radiusRest * 2 );

		mid = new Point2D.Double( 0.5, 0.5 );
		centers = new Point2D.Double[3];

		for ( int i = 0; i < 3; i++ )
		{
			double angle = ( rotateBy + i/3.0 ) * 2 * Math.PI;
			double x = mid.getX() + radius * Math.cos( angle );
			double y = mid.getY() + radius * Math.sin( angle );
			centers[i] = new Point2D.Double( x, y );
		}
	}

	void createField()
		throws Exception
	{
		matrix = new Color[height][width];

		double r0 = colors[0].getRed() / 255.0;
		double g0 = colors[0].getGreen() / 255.0;
		double b0 = colors[0].getBlue() / 255.0;
		double r1 = colors[1].getRed() / 255.0;
		double g1 = colors[1].getGreen() / 255.0;
		double b1 = colors[1].getBlue() / 255.0;
		double r2 = colors[2].getRed() / 255.0;
		double g2 = colors[2].getGreen() / 255.0;
		double b2 = colors[2].getBlue() / 255.0;

		double rBack = backgroundColor.getRed() / 255.0;
		double gBack = backgroundColor.getGreen() / 255.0;
		double bBack = backgroundColor.getBlue() / 255.0;

		for ( int i = 0; i < height; i++ )
		{
			for ( int j = 0; j < width; j++ )
			{
				Point2D p = new Point2D.Double(
					(double) i / (double) height,
					1.0 - ( (double) j / (double) width ) );

				if ( p.distance( mid ) >= 0.5 )
				{
					matrix[i][j] = backgroundColor;
					continue;
				}

				double dist0 = p.distance( centers[0] );
				double dist1 = p.distance( centers[1] );
				double dist2 = p.distance( centers[2] );

if ( dist0 >= radiusRest && dist1 >= radiusRest && dist2 >= radiusRest )
{
	matrix[i][j] = backgroundColor;
	continue;
}

				double val0 = waveFunc( dist0 );
				double val1 = waveFunc( dist1 );
				double val2 = waveFunc( dist2 );

				double intensity = ( val0 + val1 + val2 ) / 3.0;
				if ( intensity < 0 )
					intensity = 0;

/*
intensity = intensify( intensity );
int v = (int) ( 255 * intensity );
Color result = new Color( v, v, v );
//*/

//*
				if ( val0 < 0 )
					val0 = 0;
				if ( val1 < 0 )
					val1 = 0;
				if ( val2 < 0 )
					val2 = 0;

				double m0_a = val0;
				double m1_a = val1;
				double m2_a = val2;

				double m0_b = dist0 > radiusRest ? 0 : 1.0 - ( dist0 / radiusRest );
				double m1_b = dist1 > radiusRest ? 0 : 1.0 - ( dist1 / radiusRest );
				double m2_b = dist2 > radiusRest ? 0 : 1.0 - ( dist2 / radiusRest );

		double favor = 500.0;

				double m0 = m0_a + favor*m0_b;
				double m1 = m1_a + favor*m1_b;
				double m2 = m2_a + favor*m2_b;
				double sum = m0 + m1 + m2;

				double w0 = m0 / sum;
				double w1 = m1 / sum;
				double w2 = m2 / sum;

				double r = ( r0*w0 + r1*w1 + r2*w2 ) / 3.0;
				double g = ( g0*w0 + g1*w1 + g2*w2 ) / 3.0;
				double b = ( b0*w0 + b1*w1 + b2*w2 ) / 3.0;

				int n = 7;
				double dred = intensify( intensity * r, n );
				double dgreen = intensify( intensity * g, n );
				double dblue = intensify( intensity * b, n );
//				double sumRgb = r + g + b;
//				double dred = intensify( intensity * r / sumRgb )
//				double dgreen = intensify( intensity * g / sumRgb )
//				double dblue = intensify( intensity * b / sumRgb )

//*
double sat = dred;
if ( dgreen > sat )
	sat = dgreen;
if ( dblue > sat )
	sat = dblue;

dred = dred * sat + ( rBack * ( 1 - sat ) );
dgreen = dgreen * sat + ( gBack * ( 1 - sat ) );
dblue = dblue * sat + ( bBack * ( 1 - sat ) );
//*/

				int red = (int) ( dred * 255 );
				int green = (int) ( dgreen * 255 );
				int blue = (int) ( dblue * 255 );

				if ( red > 255 )
					red = 255;
				if ( green > 255 )
					green = 255;
				if ( blue > 255 )
					blue = 255;

				Color result = new Color( red, green, blue );
//*/
				matrix[i][j] = result;
			}
		}
	}

	void createImage()
		throws Exception
	{
		int []bytes = new int[width * height];
		int k = 0;
		int alpha = 255;
		for ( int i = 0; i < height; i++ )
		{
			for ( int j = 0; j < width; j++ )
			{
				Color c = matrix[i][j];
				bytes[k++] = (alpha << 24)
					|  (c.getRed() << 16)
					|  (c.getGreen() << 8 )
					| c.getBlue();
			}
		}

		int off = 0;
		int scan = width;

		image = Toolkit.getDefaultToolkit().createImage(
			new MemoryImageSource( width, height, bytes, off, scan ) );
	}

	void init()
		throws Exception
	{
		radius = 0.15;
		rotateBy = 1/2.0 + 1/20.0;
		frequencyFactor = 9.0;
		width = 1000;
		height = 1000;

		colors = new Color[3];
		colors[0] = Color.RED;
		colors[1] = Color.GREEN;
		colors[2] = Color.BLUE;

		backgroundColor = Color.WHITE;

		createPoints();
		createField();
		createImage();
	}

	public void show()
		throws Exception
	{
		ImagePanel panel = new ImagePanel( image );
		JFrame f = new JFrame();
		f.getContentPane().add( panel );
		
		int width = panel.img.getWidth( null );
		int height = panel.img.getHeight( null );

		//show frame
		f.setBounds( 0, 0, width, height );
		f.setVisible( true );
	}

	//panel used to draw image on
	private class ImagePanel extends JPanel
	{
		Image img;
		
		public ImagePanel( final Image img )
		{
			this.img = img;
		}
		
		//override paint method of panel
		public void paint( Graphics g )
		{
			g.drawImage( img, 0, 0, this );
		}
	}

	public static void main( String [] args )
	{
		try
		{
			RippleEffect effect = new RippleEffect();
			effect.init();
			effect.show();
		}

		catch ( Exception e )
		{
			System.out.println( "Error: " + e.toString() );
		}
	}
}

// kate: tab-width 4

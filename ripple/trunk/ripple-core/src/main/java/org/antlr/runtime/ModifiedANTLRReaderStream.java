package org.antlr.runtime;

import java.io.*;

/** Vacuum all input from a Reader and then treat it like a StringStream.
 *  Manage the buffer manually to avoid unnecessary data copying.
 *
 *  If you need encoding, use ANTLRInputStream.
 */
public class ModifiedANTLRReaderStream extends ModifiedANTLRStringStream {
	/** The data being scanned */
	private char[] data;

    // Note: the Reader is never closed
    private Reader reader;
private InputStream inputStream;

    private int readChunkSize;
    private int numRead = 0;

    public static final int READ_BUFFER_SIZE = 1;
//    public static final int READ_BUFFER_SIZE = 1024;
	public static final int INITIAL_BUFFER_SIZE = 1024;

	public ModifiedANTLRReaderStream() {
	}

	public ModifiedANTLRReaderStream(Reader r) throws IOException {
		this(r, INITIAL_BUFFER_SIZE, READ_BUFFER_SIZE);
	}

	public ModifiedANTLRReaderStream(Reader r, int size) throws IOException {
		this(r, size, READ_BUFFER_SIZE);
	}

	public ModifiedANTLRReaderStream(Reader r, int size, int readChunkSize) throws IOException {
		load(r, size, readChunkSize);
	}

protected void setInputStream(final InputStream is) {
    inputStream = is;
}

    protected int getChar(final int i) {
System.out.print("getChar(" + i + ") --> ");
/*try {
    System.out.println("    ready? -- " + reader.ready());
    System.out.println("    encoding = " + ((InputStreamReader) reader).getEncoding());
} catch (IOException e) {
    e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
}*/
        while (i >= numRead) {
//System.out.println("reading " + readChunkSize + " bytes (numRead = " + numRead + ")" );
            // read all the data in chunks of readChunkSize
            if ( numRead + readChunkSize > data.length ) { // overflow?
                char[] newdata = new char[data.length*2]; // resize
                System.arraycopy(data, 0, newdata, 0, data.length);
                data = newdata;
            }
            int num;
            try {
//System.out.println("    before");
                // FIXME: for some reason, the Reader refuses to read only as many bytes as characters it has been asked to read
                //num = reader.read(data, numRead, readChunkSize);
//data[numRead] = (char) reader.read();
int c = inputStream.read();
if (c != -1) {
    data[numRead] = (char) c;
    num = 1;
} else {
    num = -1;
}
//System.out.println("    after");
            } catch (IOException e) {
                e.printStackTrace();  //To change body of catch statement use File | Settings | File Templates.
                break;
            }
            if (-1 == num) {
                break;
            }
            numRead += num;
        }

        super.renameMeBackToN = numRead;

        if (i >= numRead) {
            return CharStream.EOF;
        }
        
System.out.println((int) data[i] + " (" + data[i] + ")");
        return data[i];
    }

    public String substring(int start, int stop) {
System.out.println("substring(" + start + ", " + stop+ ")");
        getChar(stop);
        return new String(data,start,stop-start+1);
	}

    public void load(Reader r, int size, int readChunkSize)
		throws IOException
	{
System.out.println("load(..., " + size + ", " + readChunkSize + ")");
        if ( r==null ) {
			return;
		}

        this.reader = r;
        this.readChunkSize = readChunkSize;

        if ( size<=0 ) {
			size = INITIAL_BUFFER_SIZE;
		}
		if ( readChunkSize<=0 ) {
			size = READ_BUFFER_SIZE;
		}

        // alloc initial buffer size.
        data = new char[size];

        /*
int c = 0, i = 0;
while (c != CharStream.EOF) {
    c = getChar(i++);
}       */
    }
}

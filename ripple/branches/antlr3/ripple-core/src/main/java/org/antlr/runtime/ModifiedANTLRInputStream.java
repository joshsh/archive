package org.antlr.runtime;

import java.io.*;

/** A kind of ReaderStream that pulls from an InputStream.
 *  Useful for reading from stdin and specifying file encodings etc...
  */
public class ModifiedANTLRInputStream extends ModifiedANTLRReaderStream {
	public ModifiedANTLRInputStream() {
	}

	public ModifiedANTLRInputStream(InputStream input) throws IOException {
		this(input, null);
	}

	public ModifiedANTLRInputStream(InputStream input, int size) throws IOException {
		this(input, size, null);
	}

	public ModifiedANTLRInputStream(InputStream input, String encoding) throws IOException {
		this(input, INITIAL_BUFFER_SIZE, encoding);
	}

	public ModifiedANTLRInputStream(InputStream input, int size, String encoding) throws IOException {
		this(input, size, READ_BUFFER_SIZE, encoding);
	}

	public ModifiedANTLRInputStream(InputStream input,
							int size,
							int readBufferSize,
							String encoding)
		throws IOException
	{
setInputStream(input);
        InputStreamReader isr;
		if ( encoding!=null ) {
			isr = new InputStreamReader(input, encoding);
		}
		else {
			isr = new InputStreamReader(input);
		}
		load(isr, size, readBufferSize);
	}
}

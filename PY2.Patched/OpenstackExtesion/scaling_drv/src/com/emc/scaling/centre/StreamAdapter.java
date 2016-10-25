package com.emc.scaling.centre;
import java.io.InputStream;
import java.io.OutputStream;

public class StreamAdapter extends Thread {
	private InputStream is;
	private OutputStream os;

	public StreamAdapter(InputStream pis, OutputStream pos) {
		is = pis;
		os = pos;
	}

	public void run() {
		try {
			byte[] buffer = new byte[102400];
			int sz = is.read(buffer);
			while (sz > 0) {
				os.write(buffer, 0, sz);
				sz = is.read(buffer);
			}
		} catch (Exception ex) {
		} finally {
			try {
				is.close();
			} catch (Exception ex) {
			}
			try {
				os.close();
			} catch (Exception ex) {
			}
		}
	}
}
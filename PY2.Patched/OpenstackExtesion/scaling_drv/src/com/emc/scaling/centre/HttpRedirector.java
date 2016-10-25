package com.emc.scaling.centre;
import java.net.Socket;

public class HttpRedirector extends Thread {
	private Socket client, node;

	public HttpRedirector(Socket client, Socket node) {
		this.client = client;
		this.node = node;
	}

	public void run() {
		try {
			//System.err.println("[INFO] Use node: " + node.getInetAddress());
			new StreamAdapter(client.getInputStream(), node.getOutputStream())
					.start();
			new StreamAdapter(node.getInputStream(), client.getOutputStream())
					.run();
			node.close();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
}
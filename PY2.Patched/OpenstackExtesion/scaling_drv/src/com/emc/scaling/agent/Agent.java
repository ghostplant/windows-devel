package com.emc.scaling.agent;

import java.io.*;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Scanner;

import com.emc.scaling.centre.Centre;

class HttpServer extends Thread {

	public void doDownload(int kbytes, PrintWriter out) {
		int total = 0;
		ArrayList<Integer> array = new ArrayList<Integer>();
		for (int i = kbytes; i >= 0; i--)
			for (int j = kbytes; j >= 0; j--) {
				total = ((i + j) ^ total >> 2);
				if (array.size() >= 1000000)
					array.remove(0);
				array.add(total);
			}
		out.println("HTTP/1.1 200 OK");
		out.println("Content-type: application/octet-stream");
		out.println("Accept-Length: " + (1024 * kbytes));
		out.println("Content-Disposition: attachment; filename=test.txt");
		out.println();
		for (int i = 0; i < kbytes; i++) {
			for (int j = 0; j < 1024; j++)
				out.print('!');
		}
	}

	public void run() {
		ServerSocket server = null;
		try {
			server = new ServerSocket(Centre.tomcatPort);
			while (true) {
				try {
					Socket client = server.accept();
					Scanner sc = new Scanner(client.getInputStream());
					String line = sc.nextLine();
					String[] parts = line.split("\\ ")[1].split("\\=");
					int length = 1;
					if (parts.length > 1)
						length = Integer.parseInt(parts[1]);
					if (length <= 0)
						length = 1;
					PrintWriter pw = new PrintWriter(client.getOutputStream());
					doDownload(length, pw);
					pw.println();
					pw.close();
					sc.close();
					client.close();
				} catch (Exception ex) {
				}
			}
		} catch (Exception ex) {
		} finally {
			if (server != null)
				try {
					server.close();
				} catch (Exception e) {
				}
		}
	}
}

public class Agent {
	public static final int depay_seconds = 10;
	public static String source;

	public static String grepFileData(String path, Grepper grepper) {
		BufferedReader br = null;
		try {
			br = new BufferedReader(new FileReader(path));
			StringBuffer sb = new StringBuffer();
			String line;
			while ((line = br.readLine()) != null) {
				String ret = grepper.grep(line);
				if (ret != null)
					sb.append(ret);
			}
			return sb.toString();
		} catch (Exception ex) {
			return null;
		} finally {
			try {
				if (br != null)
					br.close();
			} catch (Exception ex) {
			}
		}
	}

	private static long prevCPUBusy, prevCPUTotal, prevNetOut, thisMemTotal,
			thisNetBandwidth;

	public static String trimRate(double a, double b) {
		double rate = (a * 100.0 + 1e-8) / (b + 1e-8);
		return String.valueOf(rate);// String.valueOf(Math.min(100.0, Math.max(, 0.0)));
	}

	public static String generateUsages() {
		StringBuffer messages = new StringBuffer("usage:");
		// This VM IP
		messages.append(source);
		// Memory Rate
		String memFree = grepFileData("/proc/meminfo", new Grepper() {
			public String grep(String data) {
				if (data.startsWith("MemFree:"))
					return data.substring(8, data.length() - 2).trim();
				return null;
			}
		}), memTotal = grepFileData("/proc/meminfo", new Grepper() {
			public String grep(String data) {
				if (data.startsWith("MemTotal:"))
					return data.substring(9, data.length() - 2).trim();
				return null;
			}
		}), memRate = trimRate(
				Long.parseLong(memTotal) - Long.parseLong(memFree),
				Long.parseLong(memTotal));
		messages.append(";").append(memRate);
		// CPU Rate -- reference:
		// http://www.cnblogs.com/no7dw/archive/2011/07/04/2097300.html
		String cpuRate = grepFileData("/proc/stat", new Grepper() {
			public String grep(String data) {
				if (data.startsWith("cpu ")) {
					String[] clip = data.split("\\ ");
					long thisCPUBusy = 0, thisCPUTotal = 0;
					for (int i = 2; i <= 8; i++) {
						long l = Long.parseLong(clip[i]);
						thisCPUTotal += l;
						if (i <= 4)
							thisCPUBusy += l;
					}
					String rate = trimRate(thisCPUBusy - prevCPUBusy,
							thisCPUTotal - prevCPUTotal);
					prevCPUBusy = thisCPUBusy;
					prevCPUTotal = thisCPUTotal;
					return rate;
				}
				return null;
			}
		});
		messages.append(";").append(cpuRate);
		// Net Rate from eth0
		messages.append(";").append(
				grepFileData("/proc/net/dev", new Grepper() {
					public String grep(String data) {
						data = data.trim();
						if (data.startsWith("eth0:")) {
							String[] clip = data.split("\\ ");
							for (int i = 0, j = 0; i < clip.length; i++) {
								if (clip[i].length() > 0) {
									j++;
									if (j == 10) {
										long thisOut = Long.parseLong(clip[i]);
										String rate = trimRate(thisOut
												- prevNetOut, depay_seconds
												* thisNetBandwidth);
										prevNetOut = thisOut;
										return rate;
									}
								}
							}
						}
						return null;
					}
				}));
		return messages.toString();
	}

	public static void sendTcpMessage(String message, String host, int port) {
		try {
			Socket socket = new Socket(host, port);
			socket.getOutputStream().write(message.getBytes());
			socket.close();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}

	public static void sendUdpMessageToAnalyzer(String message) {
		try {
			DatagramSocket ds = new DatagramSocket();
			byte[] ipaddr = message.getBytes();
			DatagramPacket packet = new DatagramPacket(ipaddr, ipaddr.length,
					InetAddress.getByName("10.32.170.103"), 8002);
			ds.send(packet);
			ds.close();
		} catch (Exception ex) {
		}
	}

	public static String getMyRealIpAddress() {
		try {
			Enumeration<InetAddress> en = NetworkInterface.getByName("eth0")
					.getInetAddresses();
			while (en.hasMoreElements()) {
				InetAddress ia = en.nextElement();
				if (!ia.isLinkLocalAddress())
					return ia.getHostAddress();
			}
			return "no-netcard";
		} catch (Exception ex) {
			return "unknown";
		}

	}

	public static void main(String[] args) throws Exception {
		System.err.println("[INFO] Agent Starts.");

		source = getMyRealIpAddress();

		thisMemTotal = Long.parseLong(grepFileData("/proc/meminfo",
				new Grepper() {
					public String grep(String data) {
						if (data.startsWith("MemTotal:"))
							return data.substring(9, data.length() - 2).trim();
						return null;
					}
				}));
		if (thisMemTotal <= 400000 || thisMemTotal >= 500000
				&& thisMemTotal <= 600000)
			thisNetBandwidth = 102400;
		else
			thisNetBandwidth = 204800;

		sendTcpMessage("+" + source, "10.32.170.112", 8000);

		new HttpServer().start();

		sendUdpMessageToAnalyzer("started:" + source + ";" + thisNetBandwidth);

		generateUsages();

		while (true) {
			Thread.sleep(depay_seconds << 10);
			String messages = generateUsages();
			sendUdpMessageToAnalyzer(messages);
			System.err.println(messages);
		}
	}
}

package com.emc.scaling.centre;

import java.net.*;
import java.util.Scanner;

import com.emc.scaling.agent.Agent;
import com.emc.scaling.agent.Grepper;

public class Centre {
	public static int balancerPort = 8000;
	public static int tomcatPort = 8086;

	public static NodeQueue nq = new NodeQueue();

	public static void main(String[] args) throws Exception {
		System.err.println("[INFO] Balancer Started");
		ServerSocket sc = new ServerSocket(balancerPort);
		try {
			Agent.sendUdpMessageToAnalyzer("boot");
			while (true) {
				Socket client = sc.accept();
				int type = client.getInputStream().read();
				if (type == '!') {
					String netstat = Agent.grepFileData("/proc/net/dev",
							new Grepper() {
								public String grep(String data) {
									data = data.trim();
									if (data.startsWith("eth0:")) {
										String[] clip = data.split("\\ ");
										long received = 0;
										for (int i = 0, j = 0; i < clip.length; i++) {
											if (clip[i].length() > 0) {
												j++;
												if (j == 2) {
													received = Long
															.parseLong(clip[i]);
													break;
												}
											}
										}
										return String.valueOf(received);
									}
									return null;
								}
							});
					StringBuffer sb = new StringBuffer(netstat);
					Object[] objs = nq.getSet().toArray();
					for (int i = 0; i < objs.length; i++)
						sb.append(";").append((String) objs[i]);
					client.getOutputStream().write(sb.toString().getBytes());
					client.close();
				} else if (type == '+') {
					Scanner reader = new Scanner(client.getInputStream());
					nq.plus(reader.next());
					reader.close();
					client.close();
				} else if (type == '-') {
					Scanner reader = new Scanner(client.getInputStream());
					nq.minus(reader.next());
					reader.close();
					client.close();
				} else {
					Socket node = null;
					String nodeName = nq.selectNextNode(), prevNodeName = null;
					boolean selected = false;
					if (nodeName != null) {
						while (!selected) {
							try {
								node = new Socket(nodeName, tomcatPort);
								selected = true;
							} catch (Exception ex) {
								prevNodeName = nodeName;
								nodeName = nq.selectNextNode();
								if (nodeName == null)
									break;
								nq.minus(prevNodeName);
							}
						}
					}
					if (selected) {
						node.getOutputStream().write(type);
						new HttpRedirector(client, node).start();
					} else
						client.close();
				}
			}
		} catch (Exception ex) {
		} finally {
			try {
				sc.close();
			} catch (Exception e) {
			}
		}
	}
}

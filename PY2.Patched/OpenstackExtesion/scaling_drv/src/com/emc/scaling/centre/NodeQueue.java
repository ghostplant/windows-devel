package com.emc.scaling.centre;

import java.util.*;

public class NodeQueue {

	private TreeSet<String> set = new TreeSet<String>();
	private Iterator<String> it = set.iterator();

	public Set<String> getSet() {
		return set;
	}

	public synchronized String selectNextNode() {
		if (set.isEmpty())
			return null;
		if (it.hasNext())
			return (String) it.next();
		it = set.iterator();
		return (String) it.next();
	}

	public synchronized void plus(String item) {
		if (!set.contains(item)) {
			set.add(item);
			it = set.iterator();
			System.err.println("[INFO] Agent Got: " + item + ", (cur-cnt="
					+ Centre.nq.count() + ")");
		}
	}

	public synchronized void minus(String item) {
		if (set.contains(item)) {
			set.remove(item);
			it = set.iterator();
			System.err.println("[INFO] Agent Lost: " + item + ", (cur-cnt="
					+ Centre.nq.count() + ")");
		}
	}

	public int count() {
		return set.size();
	}
}

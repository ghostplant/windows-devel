package com.emc.scaling.exec;

import com.emc.scaling.agent.Agent;
import com.emc.scaling.centre.Centre;

public class Main {

	public static void main(String[] args) throws Exception {
		if (args.length == 1 && "init".equals(args[0])) {
			if (new java.io.File("/dev/vda").exists())
				Centre.main(null);
			else
				Agent.main(null);
		} else {
			Centre.main(null);
		}
	}
}

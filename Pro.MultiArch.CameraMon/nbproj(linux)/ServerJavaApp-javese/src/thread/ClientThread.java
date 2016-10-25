package thread;

import frame.*;
import java.io.*;
import java.net.Socket;
import java.text.SimpleDateFormat;

public class ClientThread extends Thread {

    private MainFrame frame;
    public Socket socket;

    public ClientThread(MainFrame frame, Socket socket) {
        this.frame = frame;
        this.socket = socket;
    }

    private boolean readConstantly(InputStream is, int size, byte[] out) throws Exception {
        int offset = 0;
        while (offset != size) {
            int rcv = is.read(out, offset, size - offset);
            if (rcv <= 0) {
                return false;
            }
            offset += rcv;
        }
        return true;
    }

    private int readInteger(InputStream is) throws Exception {
        int dat[] = new int[4];
        for (int i = 0; i < 4; i++) {
            dat[i] = is.read();
            if (dat[i] < 0) {
                return -1;
            }
        }
        return (dat[3] << 24) | (dat[2] << 16) | (dat[1] << 8) | dat[0];
    }

    public void run() {
        String ipAddress = socket.getRemoteSocketAddress().toString();
        ipAddress = ipAddress.substring(ipAddress.lastIndexOf('/') + 1, ipAddress.indexOf(':'));
        if (frame.model.contains(ipAddress)) {
            try {
                socket.getOutputStream().write('x');
            } catch (Exception ex) {
            }
            try {
                socket.close();
            } catch (Exception ex) {
            }
            return;
        }
        frame.model.addElement(ipAddress);
        frame.ipToClient.put(ipAddress, this);

        mysql.SQLServer.insert("Java: client from [ip:" + ipAddress + "] booted at [" + new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new java.util.Date()) + "].");
        try {
            int length;
            InputStream is = socket.getInputStream();
            while ((length = readInteger(is)) >= 0) {
                byte[] data = new byte[length];
                if (!readConstantly(is, length, data)) {
                    throw new Exception();
                }
                MonitorFrame monitor = frame.ipToMonitor.get(ipAddress);
                if (monitor == null) {
                    continue;
                }
                monitor.setImageData(data);
            }
        } catch (Exception ex) {
        }
        MonitorFrame monitor = frame.ipToMonitor.get(ipAddress);
        if (monitor != null) {
            monitor.dispose();
        }
        mysql.SQLServer.insert("Java: client from [ip:" + ipAddress + "] exited at [" + new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new java.util.Date()) + "].");
        frame.ipToMonitor.remove(ipAddress);
        frame.model.removeElement(ipAddress);
        frame.ipToClient.remove(ipAddress);
    }
}
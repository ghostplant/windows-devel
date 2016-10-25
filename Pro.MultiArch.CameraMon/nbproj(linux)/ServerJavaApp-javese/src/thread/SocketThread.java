package thread;

import frame.MainFrame;
import java.net.*;
import javax.swing.JOptionPane;

public class SocketThread extends Thread {

    private MainFrame frame;

    public SocketThread(MainFrame frame) {
        this.frame = frame;
    }

    public void run() {
        try {
            ServerSocket server = new ServerSocket(8000);
            while (true) {
                new ClientThread(frame, server.accept()).start();
            }
        } catch (Exception ex) {
            if ("Address already in use".equals(ex.getMessage())) {
                JOptionPane.showMessageDialog(frame, "绑定端口失败，该端口已经被占用！");
                frame.dispose();
            }
        }
    }
}
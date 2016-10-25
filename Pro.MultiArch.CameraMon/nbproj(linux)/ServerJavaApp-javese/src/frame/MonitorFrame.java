package frame;

import java.awt.BorderLayout;
import java.awt.Image;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import thread.ClientThread;

public class MonitorFrame extends JFrame implements WindowListener {

    private MainFrame frame;
    private String ipAddress;
    private ClientThread client;
    private JLabel label;
    public byte current;

    public MonitorFrame(MainFrame frame, String ipAddress, boolean webcam) {
        super("来自 " + ipAddress + "的远程视频");
        this.frame = frame;
        this.ipAddress = ipAddress;

        frame.ipToMonitor.put(ipAddress, this);
        addWindowListener(this);
        label = new JLabel();
        add(label,BorderLayout.CENTER);

        setSize(640, 480);
        setVisible(true);

        client = frame.ipToClient.get(ipAddress);
        current = (byte) (webcam ? 'c' : 'd');
        requestNext();
    }

    private void requestNext() {
        try {
            client.socket.getOutputStream().write(current);
            client.socket.getOutputStream().flush();
        } catch (Exception ex) {
        }
    }

    public void setImageData(byte[] data) {
        label.setIcon(new ImageIcon(new ImageIcon(data).getImage().getScaledInstance(getWidth(), getHeight(), Image.SCALE_FAST)));
        requestNext();
    }

    @Override
    public void windowOpened(WindowEvent e) {
    }

    @Override
    public void windowClosing(WindowEvent e) {
        frame.ipToMonitor.remove(ipAddress);
    }

    @Override
    public void windowClosed(WindowEvent e) {
    }

    @Override
    public void windowIconified(WindowEvent e) {
    }

    @Override
    public void windowDeiconified(WindowEvent e) {
    }

    @Override
    public void windowActivated(WindowEvent e) {
    }

    @Override
    public void windowDeactivated(WindowEvent e) {
    }
}
package frame;

import java.awt.*;
import java.awt.event.*;
import java.net.Socket;
import java.util.HashMap;
import javax.swing.*;
import thread.*;

public class MainFrame extends JFrame implements MouseListener, ActionListener {

    public DefaultListModel model = new DefaultListModel();
    public HashMap<String, ClientThread> ipToClient = new HashMap<String, ClientThread>();
    public HashMap<String, MonitorFrame> ipToMonitor = new HashMap<String, MonitorFrame>();
    private JList list;
    private JPopupMenu popupMenu;
    private String selected;
    private JMenuItem menuCamera = new JMenuItem("监控摄像头"),
            menuDesktop = new JMenuItem("监控桌面"),
            menuEliminate = new JMenuItem("逐出列表"),
            menuHistory = new JMenuItem("查看系统日志"),
            menuExit = new JMenuItem("退出");
    private JButton button;
    private JTextField textbox;

    public MainFrame(String ipAddress) {
        super("视频监控 - 服务器 (java-dev)");

        JPanel panel = new JPanel();
        textbox = new JTextField(ipAddress);
        button = new JButton("启动客户端");
        JLabel label = new JLabel("下面显示客户端IP地址：");
        button.addActionListener(this);
        panel.setLayout(new BorderLayout());
        panel.add(textbox, BorderLayout.CENTER);
        panel.add(button, BorderLayout.EAST);

        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("操作");
        menuHistory.addActionListener(this);
        menuExit.addActionListener(this);
        menu.add(menuHistory);
        menu.add(menuExit);
        menuBar.add(menu);
        setJMenuBar(menuBar);

        list = new JList(model);
        popupMenu = new JPopupMenu();
        popupMenu.add(menuCamera);
        popupMenu.add(menuDesktop);
        popupMenu.add(menuEliminate);
        menuCamera.addActionListener(this);
        menuDesktop.addActionListener(this);
        menuEliminate.addActionListener(this);
        list.addMouseListener(this);

        add(label, BorderLayout.NORTH);
        add(list, BorderLayout.CENTER);
        add(panel, BorderLayout.SOUTH);

        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(640, 480);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    public static void main(String[] args) {
        String ipAddress = "127.0.0.1";
        if (args.length > 0) {
            ipAddress = args[0];
        }
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception ex) {
        }
        new SocketThread(new MainFrame(ipAddress)).start();
    }

    @Override
    public void mouseClicked(MouseEvent e) {
    }

    @Override
    public void mousePressed(MouseEvent e) {
        if (e.getButton() == MouseEvent.BUTTON3) {
            list.setSelectedIndex(list.locationToIndex(e.getPoint()));
            int index = list.getSelectedIndex();
            if (index != -1) {
                selected = (String) list.getModel().getElementAt(index);
                popupMenu.show(e.getComponent(), e.getX(), e.getY());
            }
        }
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == button) {
            try {
                Socket socket = new Socket(textbox.getText(), 8001);
                socket.close();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, "链接失败，不能启动客户端！");
            }
            return;
        } else if (e.getSource() == menuExit) {
            System.exit(0);
        } else if (e.getSource() == menuHistory) {
            new HistoryFrame();
        }
        if (selected == null) {
            return;
        }
        try {
            if (ipToClient.get(selected) == null || ipToMonitor.get(selected) != null) {
                return;
            }
            if (e.getSource() == menuCamera) {
                new MonitorFrame(this, selected, true);
            } else if (e.getSource() == menuDesktop) {
                new MonitorFrame(this, selected, false);
            } else if (e.getSource() == menuEliminate) {
                ipToClient.get(selected).socket.close();
            }
        } catch (Exception ex) {
        }
    }
}

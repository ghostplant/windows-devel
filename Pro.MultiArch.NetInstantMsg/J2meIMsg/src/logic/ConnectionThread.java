/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package logic;

import comapi.CommonLib;
import frame.ConnectionFrame;
import frame.LoginFrame;
import javax.microedition.io.Connector;
import javax.microedition.io.SocketConnection;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.Display;

/**
 *
 * @author Administrator
 */
public class ConnectionThread extends Thread {

    public static final int CMD_EXIT = 1002;
    ConnectionFrame cf;
    public ClientThread ct;

    public ConnectionThread(ConnectionFrame cf) {
        this.cf = cf;
    }

    public void run() {
        try {
            cf.msgString = "正在连接服务器……";
            cf.repaint();
            cf.sc = (SocketConnection) Connector.open("socket://202.119.81.70:4502");
            CommonLib.setIOStream(cf.sc.openInputStream(), cf.sc.openOutputStream());
            ct = new ClientThread();
            ct.start();
            Display.getDisplay(CommonLib.midlet).setCurrent(new LoginFrame());
        } catch (Exception ex) {
            cf.msgString = "网络连接失败，无法连接服务器！";
            Command conn = new Command("退出", Command.EXIT, CMD_EXIT);
            cf.addCommand(conn);
            cf.repaint();
        }
    }
}

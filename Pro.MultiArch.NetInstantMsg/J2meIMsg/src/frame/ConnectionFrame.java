package frame;

import comapi.CommonLib;
import frame.LoginFrame;
import java.io.UnsupportedEncodingException;
import javax.microedition.io.Connector;
import javax.microedition.io.SocketConnection;
import javax.microedition.lcdui.*;
import logic.ConnectionThread;

public class ConnectionFrame extends Canvas implements CommandListener {

    private Command conn = new Command("连接", Command.SCREEN, CMD_CONN);
    public static final int CMD_CONN = 1001;
    public SocketConnection sc;
    public String msgString = "欢迎使用即时通客户端！";

    public ConnectionFrame() {
        setCommandListener(this);
        addCommand(conn);
    }

    protected void paint(Graphics g) {
        g.setColor(0, 0, 0);
        g.fillRect(0, 0, getWidth(), getHeight());
        try {
            Image img = Image.createImage("/back.png");
            g.drawImage(img, (getWidth() - 192) >> 1, (getHeight() - 229) >> 1, 0);
        } catch (Exception ex) {
        }
        g.setColor(255, 255, 0);
        g.drawString(msgString, getWidth() / 2, getHeight() - 1, Graphics.BOTTOM | Graphics.HCENTER);
    }

    public void commandAction(Command cmd, Displayable dsp) {
        if (cmd.getPriority() == CMD_CONN) {
            this.removeCommand(cmd);
            CommonLib.connThread = new ConnectionThread(this);
            CommonLib.connThread.start();
        } else if (cmd.getPriority() == ConnectionThread.CMD_EXIT) {
            CommonLib.midlet.notifyDestroyed();
        }
    }
}

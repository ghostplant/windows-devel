package entry;


import comapi.CommonLib;
import frame.ConnectionFrame;
import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;

public class MidletIMsg extends MIDlet {

    ConnectionFrame frame;
    boolean running = false;

    public void startApp() {
        if (running) {
            return;
        }
        running = true;
        CommonLib.midlet = this;
        frame = new ConnectionFrame();
        Display.getDisplay(this).setCurrent(frame);
    }

    public void pauseApp() {
    }

    public void destroyApp(boolean unconditional) {
        try {
            CommonLib.connThread.ct.interrupt();
        } catch (Exception ex) {
        }
        try {
            CommonLib.connThread.interrupt();
        } catch (Exception ex) {
        }
        try {
            CommonLib.close();
        } catch (Exception ex) {
        }
        try {
            frame.sc.close();
        } catch (Exception ex) {
        }
    }
}

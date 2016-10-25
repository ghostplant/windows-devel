package comapi;

import entry.MidletIMsg;
import frame.MainFrame;
import frame.RegisterFrame;
import frame.SearchFriendResultFrame;
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import javax.microedition.lcdui.Alert;
import javax.microedition.lcdui.AlertType;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.StringItem;
import logic.ConnectionThread;
import vector.FriendPair;

public class CommonLib {

    private static InputStream is;
    private static OutputStream os;
    public static MidletIMsg midlet;
    public static MainFrame frame = new MainFrame();
    public static ConnectionThread connThread;
    public static RegisterFrame rf;
    public static SearchFriendResultFrame sfrf;

    public static void appendMessage(String name, String text) {
        frame.currentChat.insert(CommonLib.frame.currentChat.position++, new StringItem(name + "：", text));
    }

    public static void MessageBox(String info) {
        MessageBox(info, "提示");
    }

    public static void MessageBox(String info, String title) {
        Alert alert = new Alert(title, info, null, AlertType.INFO);
        alert.setTimeout(Alert.FOREVER);
        Display.getDisplay(CommonLib.midlet).setCurrent(alert);
    }

    public static StringItem getItemByFid(String fid) {
        StringItem ret = null;
        for (int i = 0; i < CommonLib.connThread.ct.friends.size(); i++) {
            FriendPair fp = (FriendPair) CommonLib.connThread.ct.friends.elementAt(i);
            if (fp.friendId.equals(fid)) {
                return (StringItem) CommonLib.frame.get(fp.assignedId);
            }
        }
        return ret;
    }

    public static String toUnicode(byte[] s, int length) {
        char out[] = new char[length];
        int l = 0;
        for (int i = 0; i < length;) {
            if (s[i] >= 0) { //7位
                out[l++] = (char) s[i++];
            } else if ((s[i] & (byte) 0x20) == 0) { //11位
                int a = (s[i] ^ (byte) 0xC0) << 6;
                int b = s[i + 1] ^ (byte) 0x80;
                out[l++] = (char) (a | b);
                i += 2;
            } else {
                int a = (s[i] ^ (byte) 0xE0) << 12;
                int b = (s[i + 1] ^ (byte) 0x80) << 6;
                int c = (s[i + 2] ^ (byte) 0x80);
                out[l++] = (char) (a | b | c);
                i += 3;
            }
        }
        return new String(out, 0, l);
    }
/*
    public static byte[] toUTF8(String str) {
        byte unicode[] = str.getBytes();
        
        byte[] ans;
        return ans;
    }
*/
    public static void setIOStream(InputStream is, OutputStream os) {
        CommonLib.is = is;
        CommonLib.os = os;
    }

    public static void close() {
        try {
            is.close();
            os.close();
        } catch (Exception ex) {
        }
    }

    public static String read() throws Exception {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream dos = new DataOutputStream(baos);
        int chr = is.read();
        while (chr == -1) {
            chr = is.read();
        }
        dos.write(chr);
        int length = 0;
        byte[] buf = new byte[10];
        while (is.available() > 0 && (length = is.read(buf, 0, buf.length)) != -1) {
            dos.write(buf, 0, length);
        }
        return new String(baos.toByteArray());
    }

    public static void write(String str) throws Exception {
        os.write(str.getBytes());
        os.flush();
    }

    public static void send(String key, String[] data) throws Exception {
        StringBuffer sb = new StringBuffer();
        sb.append(Base64.encode(key));
        for (int i = 0; i < data.length; i++) {
            sb.append("|");
            sb.append(Base64.encode(data[i]));
        }
        sb.append(";");
        write(sb.toString());
    }
}

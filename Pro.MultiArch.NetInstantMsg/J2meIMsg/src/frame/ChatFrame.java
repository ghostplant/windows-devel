package frame;


import comapi.CommonLib;
import javax.microedition.lcdui.*;
import vector.Message;

public class ChatFrame extends Form implements CommandListener {

    public static final int CMD_BACK = 1001;
    public static final int CMD_SEND = 1002;
    public static final int CMD_HISTORY = 1003;
    public String fid, fname;
    Command back = new Command("返回", Command.SCREEN, CMD_BACK);
    Command send = new Command("发送", Command.SCREEN, CMD_SEND);
    Command history = new Command("历史记录", Command.SCREEN, CMD_HISTORY);
    TextField tf = new TextField("发送：", "", 100, TextField.ANY);

    public ChatFrame(String fid) {
        super("");
        this.fid = fid;
        fname = CommonLib.getItemByFid(fid).getText();
        this.setTitle("与 " + fname + " 对话中");
        this.append(tf);
        addCommand(back);
        addCommand(send);
        addCommand(history);
        this.setCommandListener(this);
    }

    public void dumpNewMessage() {
        int total = CommonLib.connThread.ct.newMessage.size();
        for (int i = 0; i < total; i++) {
            Message msg = (Message) CommonLib.connThread.ct.newMessage.elementAt(i);
            if (msg.fid.equals(fid)) {
                CommonLib.appendMessage(fname, msg.msg);
                CommonLib.connThread.ct.newMessage.removeElementAt(i);
                i--; total--;
            }
        }
        CommonLib.frame.updateUnReadedMessage();
    }
    public int position = 0;
    public HistoryFrame historyFrame;

    public void commandAction(Command c, Displayable d) {
        if (c.getPriority() == CMD_BACK) {
            CommonLib.frame.currentChat = null;
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if (c.getPriority() == CMD_SEND) {
            try {
                String sendMsg = tf.getString();
                if (sendMsg.length() == 0 || sendMsg.indexOf('\'')!=-1) {
                    CommonLib.MessageBox("不能发送空消息或非法字符！");
                    return;
                }
                CommonLib.appendMessage(CommonLib.frame.nickName, sendMsg);
                tf.setString("");
                CommonLib.send("sendmsg", new String[]{fid, sendMsg});
            } catch (Exception ex) {
            }
        } else if (c.getPriority() == CMD_HISTORY) {
            historyFrame = new HistoryFrame(fid);
            Display.getDisplay(CommonLib.midlet).setCurrent(historyFrame);
        }
    }
}

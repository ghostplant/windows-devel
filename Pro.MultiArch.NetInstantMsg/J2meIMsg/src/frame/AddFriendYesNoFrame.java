package frame;


import comapi.CommonLib;
import javax.microedition.lcdui.*;

public class AddFriendYesNoFrame extends Form implements CommandListener{
    public static final int CMD_CANCEL = 1001;
    public static final int CMD_OK = 1002;
    public static String data1 = "";
    public static String data2 = "";
    public static String fid = "";
    public static String fname = "";
    Command ok = new Command("确定", Command.SCREEN, CMD_OK);
    Command cancel = new Command("取消", Command.SCREEN, CMD_CANCEL);
    
    public AddFriendYesNoFrame(String fid, String nick, String data1, String data2) {
        super("");
        this.data1 = data1;
        this.data2 = data2;
        this.fid = fid;
        this.fname = nick;
        this.setTitle("添加好友确认");
        this.append("收到用户名为 "+ fid + " 昵称为 " + nick + " 的用户发来好友请求，是否接受请求并加对方为好友？");
        addCommand(cancel);
        addCommand(ok);
        this.setCommandListener(this);
    }
    
    public void commandAction(Command c, Displayable d) {
        if(c.getPriority() == CMD_CANCEL) {
            CommonLib.frame.confirmAddFriend = null;
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if(c.getPriority() == CMD_OK) {
            try {
                CommonLib.send("acptcnf", new String[]{fid, data1, data2, CommonLib.connThread.ct.groups.elementAt(0).toString(), fname});
                Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
            } catch (Exception e){
            }
        }
    }
}

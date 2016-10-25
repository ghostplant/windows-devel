package frame;


import comapi.CommonLib;
import javax.microedition.lcdui.*;

public class AddFriendFrame extends Form implements CommandListener{
    public static final int CMD_CANCEL = 1001;
    public static final int CMD_OK = 1002;
    Command ok = new Command("添加", Command.SCREEN, CMD_OK);
    Command cancel = new Command("返回", Command.SCREEN, CMD_CANCEL);
    TextField friendID = new TextField("好友账号：", "", 20, TextField.ANY);
    TextField nick = new TextField("好友备注：", "", 20, TextField.ANY);
    
    public AddFriendFrame() {
        super("");
        this.setTitle("添加好友");
        this.append(friendID);
        this.append(nick);
        addCommand(cancel);
        addCommand(ok);
        this.setCommandListener(this);
    }
    
    public void commandAction(Command c, Displayable d) {
        if(c.getPriority() == CMD_CANCEL) {
            CommonLib.frame.currentChangeFriend = null;
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if(c.getPriority() == CMD_OK) {
            String sendFriendID = friendID.getString();
            String sendNick = nick.getString();
            if(sendFriendID.length() == 0 || sendNick.length() == 0) {
                CommonLib.MessageBox("账号、昵称不能为空！");
                return;
            }
            friendID.setString("");
            nick.setString("");
            try {
                CommonLib.send("addfrd", new String[]{sendFriendID, CommonLib.connThread.ct.groups.elementAt(0).toString(), sendNick});
                CommonLib.MessageBox("好友添加请求已成功发送！");
                Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
            } catch (Exception e){
            }
        }
    }
}

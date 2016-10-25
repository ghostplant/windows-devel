package frame;


import comapi.CommonLib;
import javax.microedition.lcdui.*;

public class ChangeUserFrame extends Form implements CommandListener{
    public static final int CMD_CANCEL = 1001;
    public static final int CMD_OK = 1002;
    Command ok = new Command("确认", Command.SCREEN, CMD_OK);
    Command cancel = new Command("返回", Command.SCREEN, CMD_CANCEL);
    TextField tf = new TextField("确认：", "", 20, TextField.ANY);
    
    public ChangeUserFrame() {
        super("");
        this.setTitle("请输入新的昵称");
        this.append(tf);
        addCommand(cancel);
        addCommand(ok);
        this.setCommandListener(this);
    }
    
    public void commandAction(Command c, Displayable d) {
        if(c.getPriority() == CMD_CANCEL) {
            CommonLib.frame.changeUser = null;
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if(c.getPriority() == CMD_OK) {
            String sendMsg = tf.getString();
            if(sendMsg.length() == 0) {
                CommonLib.MessageBox("昵称不能为空！");
                return;
            }
            tf.setString("");
            try {
                CommonLib.send("rennick", new String[]{sendMsg});
            } catch (Exception e){
            }
        }
    }
}

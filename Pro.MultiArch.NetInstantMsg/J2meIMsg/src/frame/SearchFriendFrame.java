package frame;


import comapi.CommonLib;
import javax.microedition.lcdui.*;

public class SearchFriendFrame extends Form implements CommandListener {

    public static final int CMD_EXIT = 1002;
    public static final int CMD_SEARCH = 1001;
    TextField tf = new TextField("好友账号：", "", 20, TextField.ANY);
    Command exit = new Command("返回", Command.SCREEN, CMD_EXIT);
    Command search = new Command("查询", Command.SCREEN, CMD_SEARCH);

    public SearchFriendFrame() {
        super("");
        setTitle("搜索结果");
        append(tf);
        addCommand(exit);
        addCommand(search);
        setCommandListener(this);
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getPriority() == CMD_EXIT) {
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if (c.getPriority() == CMD_SEARCH) {
            if (tf.getString().length() == 0) {
                CommonLib.MessageBox("输入不能为空！");
            } else {
                try {
                    CommonLib.send("find", new String[]{ tf.getString() });
                    CommonLib.sfrf.deleteAll();
                    Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.sfrf);
                } catch (Exception e){
                }
            }
        }
    }
}

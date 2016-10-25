package frame;


import callback.ApplyAsyncCallback;
import comapi.CommonLib;
import comapi.QuestionBox;
import comapi.StringItemEx;
import javax.microedition.lcdui.*;

public class SearchFriendResultFrame extends Form implements CommandListener, ItemCommandListener {

    public static final int CMD_EXIT = 1001;
    public static final int CMD_ADDFRD = 1002;
    Command exit = new Command("返回", Command.SCREEN, CMD_EXIT);

    public SearchFriendResultFrame() {
        super("");
        setTitle("搜索结果");
        addCommand(exit);
        CommonLib.sfrf = this;
        setCommandListener(this);
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getPriority() == CMD_EXIT) {
            this.deleteAll();
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        }
    }

    public void commandAction(Command c, Item item) {
        if (c.getPriority() == CMD_ADDFRD) {
            StringItem si = (StringItem)item;
            QuestionBox.showQuestion("添加好友", "您确定请求对方添加您为好友吗？", new ApplyAsyncCallback(si.getLabel(),si.getText()));
        }
    }
    
    public void add(StringItem si) {
        this.append(si);
        Command add = new Command("添加", Command.SCREEN, CMD_ADDFRD);
        si.addCommand(add);
        si.setItemCommandListener(this);
    }
}

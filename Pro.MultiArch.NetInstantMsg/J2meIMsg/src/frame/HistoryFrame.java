package frame;


import comapi.CommonLib;
import javax.microedition.lcdui.*;

public class HistoryFrame extends Form implements CommandListener {

    public static final int CMD_BACK = 1001;
    public String fid, fname;
    Command back = new Command("返回", Command.SCREEN, CMD_BACK);

    public HistoryFrame(String fid) {
        super("");
        this.fid = fid;
        fname = CommonLib.getItemByFid(fid).getText();
        this.setTitle("与 " + fname + " 的历史消息");
        addCommand(back);
        try{
            CommonLib.send("history", new String[]{fid});
        } catch(Exception e) {
        }
        this.setCommandListener(this);
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getPriority() == CMD_BACK) {
            CommonLib.frame.currentChat.historyFrame = null;
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame.currentChat);
        } 
    }
}

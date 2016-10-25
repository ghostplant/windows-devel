package comapi;

import callback.AsyncCallback;
import comapi.CommonLib;
import javax.microedition.lcdui.*;

public class QuestionBox extends Form implements CommandListener {

    public static final int CMD_CANCEL = 1001;
    public static final int CMD_OK = 1002;
    Command ok = new Command("确定", Command.SCREEN, CMD_OK);
    Command cancel = new Command("取消", Command.SCREEN, CMD_CANCEL);
    private final Displayable display;
    private AsyncCallback callback;
    private boolean option;

    public QuestionBox(String title, String info, AsyncCallback callback, boolean option) {
        super("");
        this.setTitle(title);
        this.append(info);
        addCommand(cancel);
        addCommand(ok);
        this.setCommandListener(this);
        display = Display.getDisplay(CommonLib.midlet).getCurrent();
        this.callback = callback;
        this.option = option;
        Display.getDisplay(CommonLib.midlet).setCurrent(this);
    }

    public void commandAction(Command c, Displayable d) {
        if (!option || c.getPriority() == CMD_OK) {
            callback.execute();
        }
        Display.getDisplay(CommonLib.midlet).setCurrent(display);
    }

    public static void showQuestion(String title, String info, AsyncCallback callback) {
        showQuestion(title, info, callback, true);
    }

    public static void showQuestion(String title, String info, AsyncCallback callback, boolean option) {
        new QuestionBox(title, info, callback, option);
    }
}

package frame;


import comapi.CommonLib;
import frame.RegisterFrame;
import javax.microedition.lcdui.*;

public class LoginFrame extends Form implements CommandListener {

    public static final int CMD_LOGIN = 1001;
    public static final int CMD_REGISTER = 1002;
    TextField username = new TextField("账号:", "", 20, TextField.ANY);
    TextField password = new TextField("密码:", "", 20, TextField.PASSWORD);
    Command login = new Command("登录", Command.SCREEN, CMD_LOGIN);
    Command register = new Command("注册", Command.SCREEN, CMD_REGISTER);

    public LoginFrame() {
        super("登录");
        this.append(username);
        this.append(password);
        this.addCommand(login);
        this.addCommand(register);
        setCommandListener(this);
    }

    protected void paint(Graphics g) {
    }
    
    public RegisterFrame registerFrame;

    public void commandAction(Command c, Displayable d) {
        if (c.getPriority() == CMD_LOGIN) {
            if (username.getString().length() == 0 || password.getString().length() == 0) {
                CommonLib.MessageBox("请填写完整的登录信息！", "提示");
            } else {
                try {
                    CommonLib.send("log", new String[]{username.getString(), password.getString()});
                } catch (Exception ex) {
                }
            }
        } else if (c.getPriority() == CMD_REGISTER) {
            registerFrame = new RegisterFrame();
            Display.getDisplay(CommonLib.midlet).setCurrent(registerFrame);
        }
    }
}

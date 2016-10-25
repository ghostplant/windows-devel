package frame;

import comapi.CommonLib;
import javax.microedition.lcdui.*;

public class RegisterFrame extends Form implements CommandListener {

    public static final int CMD_REGISTER = 1001;
    public TextField username = new TextField("新账号(1-20字符):", "", 20, TextField.ANY);
    public TextField nickname = new TextField("新昵称(1-20字符):", "", 20, TextField.ANY);
    public TextField password = new TextField("设置密码:", "", 20, TextField.PASSWORD);
    TextField confirmPswd = new TextField("密码确认:", "", 20, TextField.PASSWORD);
    Command register = new Command("确认注册", Command.SCREEN, CMD_REGISTER);

    public RegisterFrame() {
        super("注册");
        this.append(username);
        this.append(nickname);
        this.append(password);
        this.append(confirmPswd);
        this.addCommand(register);
        CommonLib.rf = this;
        setCommandListener(this);
    }

    protected void paint(Graphics g) {
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getPriority() == CMD_REGISTER) {
            if (username.getString().length() == 0 || nickname.getString().length() == 0 || password.getString().length() == 0 || confirmPswd.getString().length() == 0) {
                CommonLib.MessageBox("请填写完整的注册信息！", "提示");
            } else if (!password.getString().equals(confirmPswd.getString())) {
                CommonLib.MessageBox("两次输入的密码不一致！", "提示");
            } else if (username.getString().indexOf('\'') != -1 || username.getString().indexOf('(') != -1 || username.getString().indexOf(')') != -1
                    || nickname.getString().indexOf('\'') != -1 || nickname.getString().indexOf('(') != -1 || nickname.getString().indexOf(')') != -1) {
                CommonLib.MessageBox("用户名和昵称不允许有非法字符！", "提示");
            } else {
                try {
                    CommonLib.send("reg", new String[]{username.getString(), nickname.getString(), "TRUE", password.getString()});
                } catch (Exception e) {
                }
            }
        }
    }
}

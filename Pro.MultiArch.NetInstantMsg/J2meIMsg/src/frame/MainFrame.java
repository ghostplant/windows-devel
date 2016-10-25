package frame;

import callback.ExitAsyncCallback;
import callback.RemoveAsyncCallback;
import comapi.QuestionBox;
import comapi.CommonLib;
import comapi.StringItemEx;
import javax.microedition.lcdui.*;
import vector.Message;

public class MainFrame extends Form implements CommandListener, ItemCommandListener {

    public static final int CMD_CHAT = 1001;
    public static final int CMD_EXIT = 1002;
    public static final int CMD_RM = 1003;
    public static final int CMD_CHGFRD = 1004;
    public static final int CMD_CHGUSER = 1005;
    public static final int CMD_ADDFRD = 1006;
    public static final int CMD_SEARCHFRD = 1007;
    public String nickName = "[正在载入]";
    Command exit = new Command("退出程序", Command.EXIT, CMD_EXIT);
    public int sysIndex;

    public MainFrame() {
        super("");
        addCommand(exit);
        setCommandListener(this);
        CommonLib.sfrf = new SearchFriendResultFrame();
        sysIndex = insertFriends("(消息)", "您有 0 条未读消息");
    }

    public void setNickName(String nickName) {
        this.nickName = nickName;
        setTitle("我的即时通 - " + nickName);
    }

    public int insertFriends(String fid, String fname) {
        StringItem item = new StringItemEx(fid, fname);
        int id = append(item);
        Command chat = new Command("打开", Command.SCREEN, CMD_CHAT);
        item.addCommand(chat);
        if (fid.equals("(消息)")) {
            Command changeUser = new Command("修改个人昵称", Command.SCREEN, CMD_CHGUSER);
            item.addCommand(changeUser);
            Command addFriend = new Command("添加好友", Command.SCREEN, CMD_ADDFRD);
            item.addCommand(addFriend);
            Command searchFriend = new Command("搜索好友", Command.SCREEN, CMD_SEARCHFRD);
            item.addCommand(searchFriend);
        } else {
            Command remove = new Command("删除", Command.SCREEN, CMD_RM);
            item.addCommand(remove);
            Command changeFriend = new Command("修改好友备注", Command.SCREEN, CMD_CHGFRD);
            item.addCommand(changeFriend);
        }
        item.setItemCommandListener(this);
        return id;
    }

    public void commandAction(Command c, Displayable d) {
        if (c.getPriority() == CMD_EXIT) {
            QuestionBox.showQuestion("提示消息", "您确定要退出程序吗？", new ExitAsyncCallback());
        }
    }
    public ChatFrame currentChat;
    public ChangeFriendFrame currentChangeFriend;
    public ChangeUserFrame changeUser;
    public AddFriendFrame addFriend;
    public AddFriendYesNoFrame confirmAddFriend;
    public SearchFriendFrame searchFriend;

    public void updateUnReadedMessage() {
        int unreaded = CommonLib.connThread.ct.newMessage.size();
        ((StringItem) CommonLib.frame.get(sysIndex)).setText("您有 " + unreaded + " 条未读消息");
    }

    public void commandAction(Command c, Item item) {
        StringItemEx itemEx = (StringItemEx) item;
        if (c.getPriority() == CMD_CHAT) {
            String fid = null;

            if (itemEx.userId.equals("(消息)")) {
                if (CommonLib.connThread.ct.newMessage.isEmpty()) {
                    CommonLib.MessageBox("目前没有未读消息！");
                    return;
                } else {
                    fid = ((Message) CommonLib.connThread.ct.newMessage.elementAt(0)).fid;
                }
            } else {
                fid = itemEx.userId;
            }
            currentChat = new ChatFrame(fid);
            currentChat.dumpNewMessage();
            Display.getDisplay(CommonLib.midlet).setCurrent(currentChat);
        } else if (c.getPriority() == CMD_RM) {
            QuestionBox.showQuestion("消息", "是否确认删除该好友？", new RemoveAsyncCallback(itemEx));
        } else if (c.getPriority() == CMD_CHGFRD) {
            String fid = itemEx.userId;
            currentChangeFriend = new ChangeFriendFrame(fid);
            Display.getDisplay(CommonLib.midlet).setCurrent(currentChangeFriend);
        } else if (c.getPriority() == CMD_CHGUSER) {
            changeUser = new ChangeUserFrame();
            Display.getDisplay(CommonLib.midlet).setCurrent(changeUser);
        } else if (c.getPriority() == CMD_ADDFRD) {
            addFriend = new AddFriendFrame();
            Display.getDisplay(CommonLib.midlet).setCurrent(addFriend);
        } else if (c.getPriority() == CMD_SEARCHFRD) {
            searchFriend = new SearchFriendFrame();
            Display.getDisplay(CommonLib.midlet).setCurrent(searchFriend);
        }
    }
}

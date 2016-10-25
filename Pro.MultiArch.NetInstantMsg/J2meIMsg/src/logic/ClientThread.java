package logic;

import callback.ExitAsyncCallback;
import comapi.CommonLib;
import comapi.QuestionBox;
import comapi.StringItemEx;
import frame.AddFriendYesNoFrame;
import java.util.Vector;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.StringItem;
import vector.FriendPair;
import vector.Message;

public class ClientThread extends Thread {

    public Vector groups = new Vector(), friends = new Vector();
    public Vector newMessage = new Vector();

    public void run() {
        try {
            while (true) {
                String buf = CommonLib.read();
                BufferPool.push(buf, this);
            }
        } catch (Exception ex) {
        }
    }

    public void execute(String key, String[] data) {
        if ("accept".equals(key)) {
            try {
                String username = CommonLib.rf.username.getString();
                String pswd = CommonLib.rf.password.getString();
                CommonLib.send("log", new String[]{username, pswd});
            } catch (Exception e) {
            }
        } else if ("reject".equals(key)) {
            CommonLib.MessageBox("注册失败，您申请的账号已被注册！");
        } else if ("con".equals(key)) {
            if ("N".equals(data[0])) {
                CommonLib.MessageBox("对不起，账号或密码不正确！");
            } else if ("Y".equals(data[0])) {
                Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
            }
        } else if ("nick".equals(key)) {
            CommonLib.frame.setNickName(data[0]);
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if ("crtgroup".equals(key)) {
            groups.addElement(data[0]);
        } else if ("crtfrd".equals(key)) {
            FriendPair fp = new FriendPair();
            fp.groupId = Integer.parseInt(data[1]);
            fp.friendId = data[0];
            fp.assignedId = CommonLib.frame.insertFriends(data[0], data[2]);
            friends.addElement(fp);
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if ("delfrd".equals(key)) {
            for (int i = 0; i < friends.size(); i++) {
                if (((FriendPair) (friends.elementAt(i))).friendId.equals(data[0])) {
                    CommonLib.frame.delete(((FriendPair) (friends.elementAt(i))).assignedId);
                    break;
                }
            }
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if ("chgfrd".equals(key)) {
            for (int i = 0; i < friends.size(); i++) {
                if (((FriendPair) (friends.elementAt(i))).friendId.equals(data[0])) {
                    int id = ((FriendPair) (friends.elementAt(i))).assignedId;
                    ((StringItem)(CommonLib.frame.get(id))).setText(data[1]);
                }
            }
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
        } else if ("cnffrd".equals(key)) {
            CommonLib.frame.confirmAddFriend = new AddFriendYesNoFrame(data[0], data[1], data[2], data[3]);
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame.confirmAddFriend);
        } else if ("recvmsg".equals(key)) {
            if (CommonLib.frame.currentChat != null && CommonLib.frame.currentChat.fid.equals(data[0])) {
                CommonLib.appendMessage(CommonLib.frame.currentChat.fname, data[1]);
            } else {
                Message msg = new Message();
                msg.fid = data[0];
                msg.msg = data[1];
                msg.time = data[2];
                newMessage.addElement(msg);
                CommonLib.frame.updateUnReadedMessage();
            }
        } else if ("history".equals(key)) {
            //CommonLib.MessageBox(key);
            if ("msg".equals(data[2])) {
                String fname = "";
                for (int i=0;i<friends.size();i++) {
                    if (((FriendPair) (friends.elementAt(i))).friendId.equals(data[0])) {
                        int id = ((FriendPair) (friends.elementAt(i))).assignedId;
                        fname = ((StringItem)(CommonLib.frame.get(id))).getText();
                        break;
                    }
                }
                if ("".equals(fname)) {
                    StringItem si = new StringItem(CommonLib.frame.nickName, data[3]);
                    CommonLib.frame.currentChat.historyFrame.append(si);
                } else {
                    StringItem si = new StringItem(fname, data[3]);
                    CommonLib.frame.currentChat.historyFrame.append(si);
                }
                Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame.currentChat.historyFrame);
            }
        } else if ("option".equals(key)) {
            StringItem si = new StringItem(data[0], data[1]);
            CommonLib.sfrf.add(si);
            Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.sfrf);
        } else if ("off".equals(key)) {
            QuestionBox.showQuestion("单点登录", "您的账号在别处登录，您被迫下线！", new ExitAsyncCallback(),false);
        } else if ("frdstt".equals(key)) {
            for (int i = 0; i < friends.size(); i++) {
                if (((FriendPair) (friends.elementAt(i))).friendId.equals(data[0])) {
                    StringItem item=(StringItem)CommonLib.frame.get(((FriendPair) (friends.elementAt(i))).assignedId);
                    item.setLabel(data[1].equals("True")?"在线":"离线");
                    break;
                }
            }
        } else if ("error".equals(key)) {
            CommonLib.MessageBox(data[0]);
        }
    }
};

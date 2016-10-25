/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.index;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Message;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.SessionUtil;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import org.hibernate.Query;

/**
 *
 * @author Ghostplant
 */
public class MessageAction extends GlobalActionSupport {

    private java.util.Date xxxxxxx;

    public Date getXxxxxxx() {
        return xxxxxxx;
    }

    public void setXxxxxxx(Date xxxxxxx) {
        this.xxxxxxx = xxxxxxx;
    }
    private int diliver;

    public int getDiliver() {
        return diliver;
    }

    public void setDiliver(int diliver) {
        this.diliver = diliver;
    }

    public String castToString(Date date) {
        SimpleDateFormat formater = new SimpleDateFormat("yyyy-MM-dd");
        return formater.format(date);
    }

    public String castToString(boolean b) {
        if (b == false) {
            return "未读";
        } else {
            return "已读";
        }
    }

    public String diliver() {
        diliver = 1;
        setMenuName("message");
        return SUCCESS;
    }

    public String send() {
        diliver = 2;
        setMenuName("message");
        return SUCCESS;
    }

    private int replyId;
    public String reply() {
        //selectedUser=repl
        diliver = 3;
        setMenuName("message");
        return SUCCESS;
    }
    
    public String getSessionId() {
        return ((Teacher) SessionUtil.getSession("user")).getId();
    }

    public String getTeacherName(String id) {
        return ((Person) HibernateUtil.get(Person.class, id)).getName();
    }

    public List getDelievedLists() {
        Query query = HibernateUtil.getSession().createQuery("from Message where dstTeacherId=:teacherId");
        query.setString("teacherId", getSessionId());
        List<Message> messages = query.list();
//        setUnreadMessagesRead(messages);
        return messages;
    }

    public List getSendedLists() {
        Query query = HibernateUtil.getSession().createQuery("from Message where srcTeacherId=:teacherId");
        query.setString("teacherId", getSessionId());
        List<Message> messages = query.list();
        return messages;
    }

    private void setUnreadMessagesRead(List<Message> messages) {
        //messages.iterator().next().getSrcTeacherId()
        Iterator<Message> it = messages.iterator();
        while (it.hasNext()) {
            Message message = it.next();
            if (message.isReaded() == false) {
                Message message1 = (Message) HibernateUtil.get(Message.class, message.getId());
                message1.setReaded(true);
                HibernateUtil.update(message1);
            }
        }
    }
    private int readId;

    public int getReadId() {
        return readId;
    }

    public void setReadId(int readId) {
        this.readId = readId;
    }

    public String write() {
        diliver = 3;
        setMenuName("message");
        return SUCCESS;
    }

    public String hasBeenRead() {
        diliver = 1;
        setMenuName("message");
        Message message = (Message) HibernateUtil.get(Message.class, readId);
        message.setReaded(true);
        HibernateUtil.update(message);
        return SUCCESS;
    }
    private String selectedUser, content;

    public String getSelectedUser() {
        return selectedUser;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public void setSelectedUser(String selectedUser) {
        this.selectedUser = selectedUser;
    }

    public List getUsers() {
        List<Teacher> t = HibernateUtil.getList(Teacher.class);
        Iterator<Teacher> it = t.iterator();
        List<String> ret = new ArrayList<String>();
        Teacher teacher;
        while (it.hasNext()) {
            teacher = it.next();
            if (teacher.isFreeze() == false) {
                ret.add(teacher.getId() + ":" + teacher.getName());
            }
        }
        return ret;
    }

    public String sendMessages() {
        if (null == selectedUser || "".equals(selectedUser)) {
            setMsgTitle("没有选定收件人");
            diliver = 3;
            setMenuName("message");
            return SUCCESS;
        }
        if (null == content || "".equals(content)) {
            setMsgTitle("没有输入内容");
            diliver = 3;
            setMenuName("message");
            return SUCCESS;
        }
        String[] id = selectedUser.split(":");

        Message message = new Message();
        message.setDate(new Date(System.currentTimeMillis()));
        message.setReaded(false);
        message.setSrcTeacherId(getSessionId());
        message.setMessage(content);
        message.setDstTeacherId(id[0]);
        HibernateUtil.save(message);

        setMsgTitle("站内信已成功发送");
        diliver = 2;
        setMenuName("message");
        return SUCCESS;
    }
}

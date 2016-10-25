/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.userdoc;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Email;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.SessionUtil;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author 编程是一门艺术
 */
public class FreezeUserAction extends GlobalActionSupport {

    private String selectedUser;

    public String getSelectedUser() {
        return selectedUser;
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

    public String freeze() {
        if (null == selectedUser || "".equals(selectedUser)) {
            setMsgTitle("没有选定要解冻的老师");
            return SUCCESS;
        }
        String[] id = selectedUser.split(":");

        Teacher sessionTeacher = (Teacher) SessionUtil.getSession("user");
        if (sessionTeacher.getId().equals(id[0])) {
            setMsgTitle("不能冻结您自己：" + id[1]);
            return SUCCESS;
        }


        Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, id[0]);
        teacher.setFreeze(true);
        HibernateUtil.update(teacher);

        setMsgTitle("已冻结" + selectedUser);
        return SUCCESS;
    }
}

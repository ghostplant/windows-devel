/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.userdoc;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author 编程是一门艺术
 */
public class UnFreezeUserAction extends GlobalActionSupport {

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
            teacher=it.next();
            if (teacher.isFreeze() == true) {
                ret.add(teacher.getId() + ":" + teacher.getName());
            }
        }
        return ret;
    }

    public String unfreeze() {
        if(null==selectedUser || "".equals(selectedUser)){
            setMsgTitle("没有选定要解冻的老师");
            return SUCCESS;
        }
        
        String[] id = selectedUser.split(":");

        Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, id[0]);
        teacher.setFreeze(false);
        HibernateUtil.update(teacher);
        
        setMsgTitle("已解冻"+selectedUser);
        return SUCCESS;
    }
}

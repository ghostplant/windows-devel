/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.userdoc;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Student;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.MD5Util;
import cn.edu.njust.steduman.util.SessionUtil;

/**
 *
 * @author 编程是一门艺术
 */
public class ChangePasswordAction extends GlobalActionSupport {

    private String oldpassword, newpassword, confirm;

    public String getConfirm() {
        return confirm;
    }

    public void setConfirm(String confirm) {
        this.confirm = confirm;
    }

    public String getNewpassword() {
        return newpassword;
    }

    public void setNewpassword(String newpassword) {
        this.newpassword = newpassword;
    }

    public String getOldpassword() {
        return oldpassword;
    }

    public void setOldpassword(String oldpassword) {
        this.oldpassword = oldpassword;
    }

    public String change() {

        Teacher sessionTeacher = (Teacher) SessionUtil.getSession("user");
        if (MD5Util.md5(oldpassword).equals(sessionTeacher.getPasswordHashCode())) {
            if (newpassword.equals(confirm)) {
                sessionTeacher.setPasswordHashCode(MD5Util.md5(newpassword));
                
                 String id = sessionTeacher.getId();
                 Teacher dbTeacher = (Teacher) HibernateUtil.get(Teacher.class, id);
                 dbTeacher.setPasswordHashCode(MD5Util.md5(newpassword));
                 HibernateUtil.update(dbTeacher);
                 
                setMsgTitle("修改密码成功");

            } else {
                setMsgTitle("修改密码失败");
                setMsgInformation("密码与验证密码不匹配！");
            }

        } else {
            setMsgTitle("修改密码失败");
            setMsgInformation("旧密码错误！");
        }

        return SUCCESS;
    }
}

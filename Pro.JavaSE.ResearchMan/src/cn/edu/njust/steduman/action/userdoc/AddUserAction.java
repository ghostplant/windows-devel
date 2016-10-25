/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.userdoc;

import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Student;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.MD5Util;
import java.util.*;

/**
 *
 * @author Ghostplant
 */
public class AddUserAction extends GlobalActionSupport {

    private String userid, username, gender, password, confirm, department;

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getConfirm() {
        return confirm;
    }

    public void setConfirm(String confirm) {
        this.confirm = confirm;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getUserid() {
        return userid;
    }

    public void setUserid(String userid) {
        this.userid = userid;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String register() {
        if (userid.length() <= 6 && (password.length() == 0 || !password.equals(confirm))) {
            setMsgTitle("注册失败信息");
            setMsgInformation("密码与验证密码不匹配，且教师密码不能为空，添加失败！");
        } else if (HibernateUtil.get(Person.class, userid) != null) {
            setMsgTitle("注册失败信息");
            setMsgInformation("用户 '" + username + "' 已经存在于数据库，添加失败！");
        } else {
            Person person = new Person();
            person.setId(userid);
            person.setMale(gender.equals("男"));
            person.setTeacher(true);
            person.setName(username);
            if (userid.length() <= 6) {
                person.setTeacher(true);
                Teacher teacher = new Teacher();
                teacher.setId(userid);
                teacher.setPasswordHashCode(MD5Util.md5(password));
                teacher.setFreeze(false);
                teacher.setDepartment(department);
                HibernateUtil.save(teacher);
                
            } else {
                person.setTeacher(false);
                Student student = new Student();
                student.setId(userid);
                student.setDormNo(department);
                HibernateUtil.save(student);
            }
            HibernateUtil.save(person);
            setMsgTitle("注册成功信息");
            setMsgInformation((person.isTeacher() ? "教师" : "学生") + "'" + username + "' 已经成功添加到数据库！");
        }
        return SUCCESS;
    }
}

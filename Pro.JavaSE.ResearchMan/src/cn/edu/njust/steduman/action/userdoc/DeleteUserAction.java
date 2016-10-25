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
import cn.edu.njust.steduman.util.SessionUtil;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author 编程是一门艺术
 */
public class DeleteUserAction extends GlobalActionSupport {

    private String selectedUser, selectedStudents;

    public String getSelectedStudents() {
        return selectedStudents;
    }

    public void setSelectedStudents(String selectedStudents) {
        this.selectedStudents = selectedStudents;
    }

    public String getSelectedUser() {
        return selectedUser;
    }

    public void setSelectedUser(String selectedUser) {
        this.selectedUser = selectedUser;
    }

    public List getUsers() {

        //List<Person> persons=HibernateUtil.getList(Person.class);
        List<Teacher> t = HibernateUtil.getList(Teacher.class);
        Iterator<Teacher> it = t.iterator();
        List<String> ret = new ArrayList<String>();
        Teacher teacher;
        while (it.hasNext()) {
            teacher = it.next();
            if (teacher.isFreeze() == true) {
                ret.add(teacher.getId() + ":" + teacher.getName() + "(已冻结)");
            } else {
                ret.add(teacher.getId() + ":" + teacher.getName() + "(未冻结)");
            }
        }
        return ret;
    }

    public List getStudents() {

        List<Person> persons = HibernateUtil.getList(Person.class);
        Iterator<Person> it = persons.iterator();
        List<String> ret = new ArrayList<String>();
        Person person;
        while (it.hasNext()) {
            person = it.next();
            if (person.isTeacher() == false) {
                ret.add(person.getId() + ":" + person.getName());
            }
        }
        return ret;
    }

    public String deleteStudent() {
        if(null==selectedStudents || "".equals(selectedStudents)){
            setMsgTitle("没有选定学生");
            return SUCCESS;
        }
        String[] id = selectedStudents.split(":");
        String name = id[1];

        Student student = (Student) HibernateUtil.get(Student.class, id[0]);
        Person person = (Person) HibernateUtil.get(Person.class, id[0]);
        HibernateUtil.delete(student);
        HibernateUtil.delete(person);

        setMsgTitle("已删除学生：" + name);
        return SUCCESS;
    }

    public String delete() {
        if(null==selectedUser || "".equals(selectedUser)){
            setMsgTitle("没有选定老师");
            return SUCCESS;
        }
        
        String[] id = selectedUser.split(":");
        String name = id[1].substring(0, id[1].indexOf("("));

        Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, id[0]);

        Teacher sessionTeacher = (Teacher) SessionUtil.getSession("user");
        if (sessionTeacher.getId().equals(teacher.getId())) {
            setMsgTitle("不能删除您自己：" + name);
        } else {
            //删除Teacher表
            HibernateUtil.delete(teacher);
            //删除Person表
            Person person = (Person) HibernateUtil.get(Person.class, id[0]);
            HibernateUtil.delete(person);

            setMsgTitle("已删除老师：" + name);
        }

        return SUCCESS;
    }
}

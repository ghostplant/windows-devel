/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.talents;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Student;
import cn.edu.njust.steduman.util.HibernateUtil;
import org.hibernate.Hibernate;

/**
 *
 * @author Administrator
 */
public class QueryStuInfoAction extends GlobalActionSupport{
    private Student student;
    private String ID;

    public String getID() {
        return ID;
    }

    public void setID(String ID) {
        this.ID = ID;
    }

    public Student getStudent() {
        return student;
    }

    public void setStudent(Student student) {
        this.student = student;
    }
    
    public String getStudentName(String studentID){
        return ((Person)HibernateUtil.get(Person.class, studentID)).getName();
    }
    public String getTeacherName(String teacherID){
        return ((Person)HibernateUtil.get(Person.class, teacherID)).getName();
    }
    public String search(){
        if(ID.length()==0){
            setMsgTitle("信息填写不完整");
            setMsgInformation("请填写学号");
            return SUCCESS;
        }
        student=(Student)HibernateUtil.get(Student.class, ID);
        if(student==null){
            setMsgTitle("学生信息不存在");
            setMsgInformation("请检查输入");
            return SUCCESS;
        }
        return SUCCESS;
    }
    
}

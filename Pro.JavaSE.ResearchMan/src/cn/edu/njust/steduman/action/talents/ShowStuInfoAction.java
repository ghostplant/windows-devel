/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.talents;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Student;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.List;
import org.hibernate.Hibernate;

/**
 *
 * @author Administrator
 */
public class ShowStuInfoAction extends GlobalActionSupport{
    private String id, dormNo, dormPhone, position, awards, nation, degree, teacherId;
    private int entranceYear, graduationYear;
private String selectedId;

    public String getSelectedId() {
        return selectedId;
    }

    public void setSelectedId(String selectedId) {
        this.selectedId = selectedId;
    }
    public String getAwards() {
        return awards;
    }

    public void setAwards(String awards) {
        this.awards = awards;
    }

    public String getDegree() {
        return degree;
    }

    public void setDegree(String degree) {
        this.degree = degree;
    }

    public String getDormNo() {
        return dormNo;
    }

    public void setDormNo(String dormNo) {
        this.dormNo = dormNo;
    }

    public String getDormPhone() {
        return dormPhone;
    }

    public void setDormPhone(String dormPhone) {
        this.dormPhone = dormPhone;
    }

    public int getEntranceYear() {
        return entranceYear;
    }

    public void setEntranceYear(int entranceYear) {
        this.entranceYear = entranceYear;
    }

    public int getGraduationYear() {
        return graduationYear;
    }

    public void setGraduationYear(int graduationYear) {
        this.graduationYear = graduationYear;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getNation() {
        return nation;
    }

    public void setNation(String nation) {
        this.nation = nation;
    }

    public String getPosition() {
        return position;
    }

    public void setPosition(String position) {
        this.position = position;
    }

    public String getTeacherId() {
        return teacherId;
    }

    public void setTeacherId(String teacherId) {
        this.teacherId = teacherId;
    }
    
    public List getStudentList(){
        return HibernateUtil.getList(Student.class);
    }
    public String getStudentName(String  studentID){
        return ((Person)HibernateUtil.get(Person.class, studentID)).getName();
    }
    public String getTeacherName(String teacherID){
        return  teacherID+" "+((Person)HibernateUtil.get(Person.class,teacherID)).getName();
    }
    
    public String delete() {
       Student student = (Student) HibernateUtil.get(Student.class, selectedId);
       Person person =(Person)HibernateUtil.get(Person.class, selectedId);
        HibernateUtil.delete(student);
        HibernateUtil.delete(person);
        return SUCCESS;
    }

}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.achievement;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.*;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.*;

/**
 *
 * @author zyz
 */
public class CultivationAchievementAction extends GlobalActionSupport {

    private String studentId, essayTitle, major, subject;
    private Date date;
    private String studentName, teacherId, teacherName;

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public String getEssayTitle() {
        return essayTitle;
    }

    public void setEssayTitle(String essayTitle) {
        this.essayTitle = essayTitle;
    }

    public String getMajor() {
        return major;
    }

    public void setMajor(String major) {
        this.major = major;
    }

    public String getStudentId() {
        return studentId;
    }

    public void setStudentId(String studentId) {
        this.studentId = studentId;
    }

    public String getStudentName() {
        return studentName;
    }

    public void setStudentName(String studentName) {
        this.studentName = studentName;
    }

    public String getSubject() {
        return subject;
    }

    public void setSubject(String subject) {
        this.subject = subject;
    }

    public String getTeacherId() {
        return teacherId;
    }

    public void setTeacherId(String teacherId) {
        this.teacherId = teacherId;
    }

    public String getTeacherName() {
        return teacherName;
    }

    public void setTeacherName(String teacherName) {
        this.teacherName = teacherName;
    }

    boolean hasContent(String s) {
        if (s == null || s.trim().equals("")) {
            return false;
        }
        return true;
    }

    public List getStudents() {
        List<Student> students = HibernateUtil.getList(Student.class);
        Iterator<Student> iterator = students.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            res.add(iterator.next().getId());
        }
        return res;
    }

    public String getInfo() {
        if (!hasContent(studentId)) {
            studentName = teacherId = teacherName = "";
            return SUCCESS;
        }
        Student student = (Student) HibernateUtil.get(Student.class, studentId);
        Person person = (Person) HibernateUtil.get(Person.class, studentId);
        teacherId = student.getTeacherId();
        studentName = person.getName();
        Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, teacherId);
        teacherName = teacher.getName();
        return SUCCESS;
    }

    public String submit() {

        if (!hasContent(studentId) || !hasContent(essayTitle) || !hasContent(major) || !hasContent(subject) || date == null) {
            setMsgTitle("录入错误");
            setMsgInformation("请正确输入表中所有项目");
            return SUCCESS;
        }
        Cultivation cul = new Cultivation();
        cul.setDate(date);
        cul.setEssayTitle(essayTitle);
        cul.setMajor(major);
        cul.setStudentId(studentId);
        cul.setSubject(subject);
        HibernateUtil.save(cul);
        studentId = essayTitle = major = subject = "";
        date = null;
        setMsgTitle("录入成功");
        return SUCCESS;
    }
}

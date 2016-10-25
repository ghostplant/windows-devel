/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.talents;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Student;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Administrator
 */
public class ChangeStuInfoAction extends GlobalActionSupport {

    private String id = "", dormNo, dormPhone, position, awards, nation, degree, teacherId;
    private int entranceYear, graduationYear;
    private String studentName;
    private String selectedTeacherId;

    public String getSelectedTeacherId() {
        return selectedTeacherId;
    }

    public void setSelectedTeacherId(String selectedTeacherId) {
        this.selectedTeacherId = selectedTeacherId;
    }

    public String getStudentName() {
        return studentName;
    }

    public void setStudentName(String studentName) {
        this.studentName = studentName;
    }

    public List getStudentList() {
        return HibernateUtil.getList(Student.class);
    }

    public String getAwards() {
        return awards;
    }

    public String show() {
        if (id == null || id.length()==0) {
            setMsgTitle("你必须选择一个学生的ID然后进行后续操作。");
        } else {
            studentName = ((Person) HibernateUtil.get(Person.class, id)).getName();
            Student st = (Student) HibernateUtil.get(Student.class, id);
            dormNo = st.getDormNo();
            dormPhone = st.getDormPhone();
            position = st.getPosition();
            nation = st.getNation();
            awards = st.getAwards();
            degree = st.getDegree();
            selectedTeacherId = st.getTeacherId();
            entranceYear = st.getEntranceYear();
            graduationYear = st.getEntranceYear();
        }
        return SUCCESS;
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

    public List getTeachers() {
        List<Teacher> teachers = HibernateUtil.getList(Teacher.class);
        if (true) {
            return teachers;
        }
        Iterator<Teacher> iterator = teachers.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            Teacher nextteacher = (Teacher) iterator.next();
            res.add(nextteacher.getId() + " " + nextteacher.getName());
        }
        return res;
    }

    public String update() {
        Student student = (Student) HibernateUtil.get(Student.class, id);
        if (student == null) {
            setMsgTitle("学生未查到");
            setMsgInformation("请检查学生信息（学号）");
            return SUCCESS;
        }
        if (selectedTeacherId == null) {
            setMsgTitle("为填写导师信息");
            setMsgInformation("请选择导师");
            return SUCCESS;
        }
        Person person = (Person) HibernateUtil.get(Person.class, id);
        person.setName(studentName);
        HibernateUtil.update(person);

        student.setAwards(awards);
        student.setDegree(degree);
        student.setDormNo(dormNo);
        student.setDormPhone(dormPhone);
        student.setEntranceYear(entranceYear);
        student.setGraduationYear(graduationYear);
        student.setNation(nation);
        student.setPosition(position);
        student.setTeacherId(selectedTeacherId);
        HibernateUtil.update(student);
        setMsgTitle("更新信息成功");
        setMsgInformation("信息已经成功添加到数据库");
        return SUCCESS;
    }
}

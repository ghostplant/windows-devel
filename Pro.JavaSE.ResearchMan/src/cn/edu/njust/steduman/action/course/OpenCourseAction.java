/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.course;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Course;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import org.hibernate.Hibernate;

/**
 *
 * @author Administrator
 */
public class OpenCourseAction extends GlobalActionSupport {

    private String id, name, target, type, state,teacher;

    public String getTeacher() {
        return teacher;
    }

    public void setTeacher(String teacher) {
        this.teacher = teacher;
    }
    private int creditHour;
    private float creditValue;
    private boolean established;

    public int getCreditHour() {
        return creditHour;
    }

    public void setCreditHour(int creditHour) {
        this.creditHour = creditHour;
    }

    public float getCreditValue() {
        return creditValue;
    }

    public void setCreditValue(float creditValue) {
        this.creditValue = creditValue;
    }

    public boolean isEstablished() {
        return established;
    }

    public void setEstablished(boolean established) {
        this.established = established;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }

    public String getTarget() {
        return target;
    }

    public void setTarget(String target) {
        this.target = target;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public List getTeachers() {
        List<Teacher> teachers = HibernateUtil.getList(Teacher.class);
        Iterator<Teacher> iterator = teachers.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            Teacher nextteacher=(Teacher)iterator.next();
            res.add(nextteacher.getName()+" "+nextteacher.getId());
        }
        return res;
    }
    public String submit(){
        Course course=new Course();
        //TeacherOfCourse teach=new TeacherOfCourse();
        course.setCreditHour(creditHour);
        course.setCreditValue(creditValue);
        course.setEstablished(state.equals("已开设"));
        course.setId(id);
        course.setName(name);
        course.setTarget(target);
        course.setType(type);
        String[] temp=teacher.split(" ");
        Teacher person=(Teacher)HibernateUtil.get(Teacher.class, temp[1]);  
        course.getTeachers().add(person);
        HibernateUtil.save(course);
        setMsgTitle("成功提交信息");
        setMsgInformation( "已经成功开设课程并添加到数据库！");
        return SUCCESS;      
    }
}

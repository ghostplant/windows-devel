/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.course;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Course;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.database.TeacherOfProject;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import org.apache.struts2.ServletActionContext;

/**
 *
 * @author Administrator
 */
public class QueryCourseAction extends GlobalActionSupport {

    private String teachername = null, classname = null;
    private Course course;
    private Teacher teacher;

    public Teacher getTeacher() {
        return teacher;
    }

    public void setTeacher(Teacher teacher) {
        this.teacher = teacher;
    }

    public String getClassname() {
        return classname;
    }

    public void setClassname(String classname) {
        this.classname = classname;
    }

    public String getTeachername() {
        return teachername;
    }

    public void setTeachername(String teachername) {
        this.teachername = teachername;
    }

    public Course getCourse() {
        return course;
    }

    public void setCourse(Course course) {
        this.course = course;
    }

    public List getTeachers() {
        List<Teacher> teachers = HibernateUtil.getList(Teacher.class);
        Iterator<Teacher> iterator = teachers.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            Teacher nextteacher = (Teacher) iterator.next();
            res.add(nextteacher.getId() + " " + nextteacher.getName());
        }
        return res;
    }

    public List getCourses() {
        List<Course> courses = HibernateUtil.getList(Course.class);
        Iterator<Course> iterator = courses.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            Course nextCourse = iterator.next();
            res.add(nextCourse.getId() + " " + nextCourse.getName());
        }
        return res;
    }

    public String queryByCourse() {

        String[] temp = classname.split(" ");
        course = (Course) HibernateUtil.get(Course.class, temp[0]);
        if (course == null) {
            setMsgTitle("课程不存在，请检查课程信息！");
        }
        return SUCCESS;
    }

    public Set getTeacherSet(String courseID) {
        return ((Course) HibernateUtil.get(Course.class, courseID)).getTeachers();
    }

    public Set getCourseSet(String teacherID) {
        List<Course> list = HibernateUtil.getList(Course.class);
        Iterator<Course> it = list.iterator();
        HashSet<Course> ret = new HashSet();
        while (it.hasNext()) {
            Course course = it.next();
            Iterator<Teacher> its = course.getTeachers().iterator();
            boolean flag = false;
            while (its.hasNext()) {
                Teacher tc = its.next();
                if (tc.getId().equals(teacherID)) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                ret.add(course);
            }
        }
        return ret;
    }

    public String queryByTeacher() {
        String[] temp = teachername.split(" ");
        if (teachername == null) {
            setMsgTitle("老师信息不存在，请检查输入！");
            return SUCCESS;
        }
        teacher = (Teacher) HibernateUtil.get(Teacher.class, temp[0]);
        if (teacher == null) {
            setMsgTitle("老师信息不存在，请检查输入！");
        }
        // else setMsgTitle("有老师！");
        return SUCCESS;
    }
}

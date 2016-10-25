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
import cn.edu.njust.steduman.util.SessionUtil;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import org.hibernate.Hibernate;

/**
 *
 * @author Administrator
 */
public class ChangeCourseAction extends GlobalActionSupport {

    private String id, name, target, type, state;
    private int creditHour;
    private float creditValue;
    private boolean established;
    private String selectedId, teacherList, item;

    public String getItem() {
        return item;
    }

    public void setItem(String item) {
        this.item = item;
    }

    public String getTeacherList() {
        return teacherList;
    }

    public void setTeacherList(String teacherList) {
        this.teacherList = teacherList;
    }

    public String getSelectedId() {
        return selectedId;
    }

    public void setSelectedId(String selectedId) {
        this.selectedId = selectedId;
    }

    public String getState() {
        return state;
    }

    public void setState(String state) {
        this.state = state;
    }

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

    public List getCourseList() {
        return HibernateUtil.getList(Course.class);
    }

    public Set getTeacherSet(String courseID) {
        return ((Course) HibernateUtil.get(Course.class, courseID)).getTeachers();
    }

    public String delete() {
        Course course = (Course) HibernateUtil.get(Course.class, selectedId);
        HibernateUtil.delete(course);
        return SUCCESS;
    }

    public List getTeachers() {
        List<Teacher> teachers = HibernateUtil.getList(Teacher.class);
        Iterator<Teacher> iterator = teachers.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            Teacher nextteacher = (Teacher) iterator.next();
            res.add(nextteacher.getName() + " " + nextteacher.getId());
        }
        return res;
    }

    public String addItem() {
        if (item.length() == 0) {
            setMsgTitle("添加教师信息");
            setMsgInformation("教师名称不能为空！");
        } else {
            teacherList += item + "~";
            item = "";
        }
        return SUCCESS;
    }

    public String clearItem() {
        teacherList = "";
        return SUCCESS;
    }

    public String update() {
        if (teacherList.length() == 0 || name.length() == 0 || name == null || id == null || id.length() == 0 || id.trim().equals("")
                || teacherList == null || name.trim().equals("")) {
            setMsgTitle("输入不完整");
            setMsgInformation("您必须填写完整信息！");
            return SUCCESS;
        }
        Course course = (Course) HibernateUtil.get(Course.class, id);
        if (course == null) {
            course = new Course();
            setMsgTitle("课程不存在");
            setMsgInformation("请检查课程编号！");
            return SUCCESS;
        }

        String[] teachers = teacherList.split("~");
        course.setCreditHour(creditHour);
        course.setCreditValue(creditValue);
        course.setEstablished(state.equals("已开设"));
        course.setId(id);
        course.setName(name);
        course.setTarget(target);
        course.setType(type);
        course.getTeachers().clear();
        for (String iteacher : teachers) {
            String temp[] = iteacher.split(" ");
            Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, temp[1]);
            course.getTeachers().add(teacher);
        }


        HibernateUtil.update(course);
        setMsgTitle("成功提交信息");
        setMsgInformation("已经成功更新到数据库！");
        return SUCCESS;
    }
}

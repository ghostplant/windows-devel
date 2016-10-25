/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.research;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Project;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.database.TeacherOfProject;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.SessionUtil;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import javax.management.Query;
import org.apache.poi.hssf.util.HSSFColor.TURQUOISE;

/**
 *
 * @author Administrator
 */
public class ApplyProjectAction extends GlobalActionSupport {

    private int id, startYear, endYear;
    private float cost;
    private String name, number, source, type, status;
    private String teacher;
    private Map personMap;

    public Map getPersonMap() {
        return personMap;
    }

    public void setPersonMap(Map personMap) {
        this.personMap = personMap;
    }

    public String getRank() {
        return rank;
    }

    public void setRank(String rank) {
        this.rank = rank;
    }

    public String getTeacher() {
        return teacher;
    }

    public void setTeacher(String teacher) {
        this.teacher = teacher;
    }
    private String rank;
    private int int_rank;

    public int getInt_rank() {
        return int_rank;
    }

    public void setInt_rank(int int_rank) {
        this.int_rank = int_rank;
    }
    private Set teacherOfProjects = new HashSet();

    public Set getTeacherOfProjects() {
        return teacherOfProjects;
    }

    public void setTeacherOfProjects(Set teacherOfProjects) {
        this.teacherOfProjects = teacherOfProjects;
    }

    public float getCost() {
        return cost;
    }

    public void setCost(float cost) {
        this.cost = cost;
    }

    public int getEndYear() {
        return endYear;
    }

    public void setEndYear(int endYear) {
        this.endYear = endYear;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getNumber() {
        return number;
    }

    public void setNumber(String number) {
        this.number = number;
    }

    public String getSource() {
        return source;
    }

    public void setSource(String source) {
        this.source = source;
    }

    public int getStartYear() {
        return startYear;
    }

    public void setStartYear(int startYear) {
        this.startYear = startYear;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public boolean hasContent(String a) {
        if (a == null || a.trim().equals("")) {
            // setMsgTitle("not null");
            return false;
        }
        return true;
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

    public List getStartYears() {
        List<Integer> res = new ArrayList<Integer>();
        for (int i = 1900; i <= 2050; i++) {
            res.add(i);
        }
        return res;
    }

    public List getEndYears() {
        List<Integer> res = new ArrayList<Integer>();
        for (int i = 1900; i <= 2050; i++) {
            res.add(i);
        }
        return res;
    }

    public String addPerson() {
        personMap = (TreeMap) SessionUtil.getSession("Project_personMap");
        if (personMap == null) {
            personMap = new TreeMap();
        }
        int_rank = Integer.parseInt(rank);
        //System.out.println("ADD PERSON");
        personMap.put(int_rank, teacher);

        // System.out.println(teacher + " " + rank + " " + personMap.size());
        SessionUtil.setSession("Project_personMap", personMap);
        return SUCCESS;
    }

    public String submit() {
        Project project = new Project();
        TeacherOfProject teach = null;
        personMap = (TreeMap) SessionUtil.getSession("Project_personMap");
        if (personMap == null) {
            personMap = new TreeMap();
            int_rank = Integer.parseInt(rank);
            personMap.put(int_rank, teacher);
        } else {
            int_rank = Integer.parseInt(rank);
            personMap.put(int_rank, teacher);
        }
        project.setCost(cost);
        project.setEndYear(endYear);
        project.setName(name);
        project.setNumber(number);
        project.setSource(source);
        project.setStartYear(startYear);
        project.setStatus(status);
        project.setType(type);
        HibernateUtil.save(project);
        id = project.getId();
        //org.hibernate.Query query = HibernateUtil.getSession().createQuery("from Person where name=:name");
        Iterator iterator = personMap.keySet().iterator();
        while (iterator.hasNext()) {
            teach = new TeacherOfProject();
            teach.setProjectId(id);
            int_rank = (Integer) iterator.next();
            teach.setRank(int_rank);
            //  query.setString("name", (String) personMap.get(int_rank));
            String temp[] = ((String) personMap.get(int_rank)).split(" ");
            teach.setTeacherId(temp[1]);
            project.getTeacherOfProjects().add(teach);
            HibernateUtil.update(project);

        }

        setMsgTitle("成功提交信息");
        setMsgInformation(project.getName() + "已经成功申请项目并添加到数据库！");
        return SUCCESS;
    }
}
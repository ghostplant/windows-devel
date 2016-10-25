/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.research;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Course;
import cn.edu.njust.steduman.database.Project;
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
public class QueryProjectAction extends GlobalActionSupport {

    private String projectName, teacherName;
     private Project project;
    private Teacher teacher;

    public Project getProject() {
        return project;
    }

    public void setProject(Project project) {
        this.project = project;
    }

    public String getProjectName() {
        return projectName;
    }

    public void setProjectName(String projectName) {
        this.projectName = projectName;
    }

    public Teacher getTeacher() {
        return teacher;
    }

    public void setTeacher(Teacher teacher) {
        this.teacher = teacher;
    }

    public String getTeacherName() {
        return teacherName;
    }

    public void setTeacherName(String teacherName) {
        this.teacherName = teacherName;
    }
   
    public List getProjects(){
        List<Project> projects=HibernateUtil.getList(Project.class);
         Iterator<Project> iterator = projects.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            Project nextproject = (Project) iterator.next();
            res.add(nextproject.getId() + " " + nextproject.getName());
        }
        return res;
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
   
   public Set getTeacherOfProjectSet(int projectID){
       Set teacherofproject=((Project)HibernateUtil.get(Project.class, projectID)).getTeacherOfProjects();
       Iterator iterator=teacherofproject.iterator();
       while(iterator.hasNext()){
           TeacherOfProject teacherOfProject=(TeacherOfProject)iterator.next();
           if(String.valueOf(teacherOfProject.getProjectId())==null||String.valueOf(teacherOfProject.getRank())==null||teacherOfProject.getTeacherId()==null){
              iterator.remove();
           }
       }
      
       return teacherofproject;
   }
   public String getTeacherName(String teacherID){
       return ((Teacher)HibernateUtil.get(Teacher.class, teacherID)).getName();
   }
   public Set getProjectOfTeacherSet(String teacherID){
       HibernateUtil.close();
       HibernateUtil.init();
       Set projectofteacher=((Teacher)HibernateUtil.get(Teacher.class, teacherID)).getTeacherOfProjects();
       Iterator iterator=projectofteacher.iterator();
       while(iterator.hasNext()){
           TeacherOfProject teacherOfProject=(TeacherOfProject)iterator.next();
           if(String.valueOf(teacherOfProject.getProjectId())==null||String.valueOf(teacherOfProject.getRank())==null||teacherOfProject.getTeacherId()==null){
              iterator.remove();
           }
       }
       return projectofteacher;
//
//       return ((Teacher)HibernateUtil.get(Teacher.class, teacherID)).getTeacherOfProjects();
   }
   public Project getPro(int projectID){
       return (Project)HibernateUtil.get(Project.class, projectID);
   }
//   public Project getProjectViaId(int projectOfTeacherID){
//       Project projectViaId=(Project)HibernateUtil.get(Project.class,((TeacherOfProject)HibernateUtil.get(TeacherOfProject.class, projectOfTeacherID)).getProjectId());
//       return projectViaId;
//   }
   public String queryByProject(){
        String[] temp = projectName.split(" ");
        project = (Project) HibernateUtil.get(Project.class, Integer.valueOf(temp[0]));
        if (project == null) {
            setMsgTitle("课程不存在，请检查课程信息！");
        }
        
       return SUCCESS;
   }
   public String queryByTeacher(){
       String[] temp = teacherName.split(" ");
        teacher = (Teacher) HibernateUtil.get(Teacher.class,temp[0]);
        if (teacher == null) {
            setMsgTitle("课程不存在，请检查课程信息！");
          //  setMsgTitle(teacher.getName());
        }
//        Set set= ((Teacher)HibernateUtil.get(Teacher.class, temp[0])).getTeacherOfProjects();
//       if(set!=null){
//           setMsgTitle("Set不为空！");
//           setMsgInformation(set.toString());
//           
//           //System.out.print("成功啦！！！！");
//           //setMsgTitle("成功啦！！！！");
//       }
//       else{
//           setMsgTitle("Set空的！");
//           //System.out.print("失败啦！！！！");
//       }
       return SUCCESS;
   }
//    public String submitByYear() {
//        org.hibernate.Query query = HibernateUtil.getSession().createQuery("from Project where startYear>=:startYear AND endYear<:endYear");
//        query.setString("startYear", startYear);
//        query.setString("endYear", endYear);
//        Iterator<Project> iterator = query.list().iterator();
//        if (!iterator.hasNext()) {
//            setMsgTitle("没有符合条件的结果");
//            return SUCCESS;
//        }
//        projectSet.clear();
//        while (iterator.hasNext()) {
//            projectSet.add(iterator.next());
//        }
//        return SUCCESS;
//
//    }
}

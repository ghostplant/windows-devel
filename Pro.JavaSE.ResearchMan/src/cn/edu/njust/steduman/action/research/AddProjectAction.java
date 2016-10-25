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
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.SessionUtil;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.InputStream;
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
public class AddProjectAction extends GlobalActionSupport {

    private int id, startYear, endYear;
    private float cost;
    private String name, number, source, type, status;
    private Set teacherOfProjects = new HashSet();
    private String teacherList, itemOfTeacher;
    private int rank;
    private int selectedId;

    public int getSelectedId() {
        return selectedId;
    }

    public void setSelectedId(int selectedId) {
        this.selectedId = selectedId;
    }

    public String getItemOfTeacher() {
        return itemOfTeacher;
    }

    public void setItemOfTeacher(String itemOfTeacher) {
        this.itemOfTeacher = itemOfTeacher;
    }

    public int getRank() {
        return rank;
    }

    public void setRank(int rank) {
        this.rank = rank;
    }

    public String getTeacherList() {
        return teacherList;
    }

    public void setTeacherList(String teacherList) {
        this.teacherList = teacherList;
    }

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

    public String addItem() {
        if (itemOfTeacher.length() == 0) {
            setMsgTitle("添加教师信息");
            setMsgInformation("教师名称不能为空！");
        } else {
            teacherList += itemOfTeacher + "~" + rank + "@";
            itemOfTeacher = "";
        }
        return SUCCESS;
    }

    public String clearItem() {
        teacherList = "";
        return SUCCESS;
    }

    public List getProjectList() {
        return HibernateUtil.getList(Project.class);
    }

    public Set getTeacherOfProjectSet(int projectID) {
        return ((Project) HibernateUtil.get(Project.class, projectID)).getTeacherOfProjects();
    }

    public String getTeacherId(int teaproID) {
        return ((TeacherOfProject) HibernateUtil.get(TeacherOfProject.class, teaproID)).getTeacherId();
    }

//    public String getTeacherName(int teaproID) {
//        return ((Teacher) HibernateUtil.get(Teacher.class, (String) ((TeacherOfProject) HibernateUtil.get(TeacherOfProject.class, teaproID)).getTeacherId())).getName();
//    }

    public String getTeacherName(String teacherID){
        return ((Teacher)HibernateUtil.get(Teacher.class, teacherID)).getName();
    }
    public String delete() {
        HibernateUtil.close();
        HibernateUtil.init();
        Project project = (Project) HibernateUtil.get(Project.class, selectedId);
        HibernateUtil.delete(project);
       // HibernateUtil.update(Project.class);
        setMsgTitle("成功删除信息！");
        return SUCCESS;
    }

    public void destroy() {
        id = startYear = endYear = rank = 0;
        cost = 0;
        name = null;
        number = null;
        source = null;
        type = null;
        status = null;
        teacherList = null;
        itemOfTeacher = null;
    }

    public String insert() {

        if (teacherList.length() == 0 || name.length() == 0||teacherList==null||name==null||name.trim().equals("")){
            setMsgTitle("添加项目信息");
            setMsgInformation("您必须填写完整信息！");
            return SUCCESS;
        }
        Project project = new Project();
        TeacherOfProject teach = null;
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
        String[] teachers = teacherList.split("@");
        for (String iteacherAndRank : teachers) {
            teach = new TeacherOfProject();
            String teacher[] = iteacherAndRank.split("~");
            String temp[] = teacher[0].split(" ");
            //Teacher teacher = (Teacher)HibernateUtil.get(Teacher.class, temp[1]);
            int Rank = Integer.valueOf(teacher[1]);
            teach.setProjectId(id);
            teach.setRank(Rank);
            teach.setTeacherId(temp[1]);
            project.getTeacherOfProjects().add(teach);
        }
        HibernateUtil.update(project);
        destroy();
        setMsgTitle("成功提交信息");
        setMsgInformation(project.getName() + "已经成功添加到数据库！");
        return SUCCESS;
    }
     public String export() {
        WebdiskUtil.clearTempFile();  // 第一步，清理
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("project.xls")); // 第二步，载入
            Iterator<Project> iterator=getProjectList().iterator();
            boolean first = true;
            int lineNumber=2;
            int index=0;
            while (iterator.hasNext()) {
                
                if (first) {
                    first = false;
                    lineNumber = 2;
                } 
                Project _project=iterator.next();
                Iterator<TeacherOfProject> it=_project.getTeacherOfProjects().iterator();
                while(it.hasNext()){
                    TeacherOfProject _teacherOfProject=it.next();
                    workbook.setCenterCellValue(0, lineNumber, 0, String.valueOf(++index));
                    workbook.setCenterCellValue(0, lineNumber, 1, _project.getName()+"("+String.valueOf(_project.getId()+")"));
                    workbook.setCenterCellValue(0, lineNumber, 2, _project.getSource());
                    workbook.setCenterCellValue(0, lineNumber, 3,String.valueOf(_project.getStartYear())+"-"+String.valueOf(_project.getEndYear()));
                    workbook.setCenterCellValue(0, lineNumber, 4,getTeacherName(_teacherOfProject.getTeacherId())+"("+String.valueOf(_teacherOfProject.getRank())+")");
                    workbook.setCenterCellValue(0, lineNumber, 5, String.valueOf(_project.getCost()));
                  
                    lineNumber = workbook.createNewLineWithBorder(0);
                }
            }
            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("project.xls")); // 第三步，保存
            workbook.close(); // 第四步，释放资源与返回INPUT
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception { // 第五步，定义下载入口流；第六步修改struts2配置文件
        return WebdiskUtil.getSaveDocumentStream("project.xls");
    }
    
}
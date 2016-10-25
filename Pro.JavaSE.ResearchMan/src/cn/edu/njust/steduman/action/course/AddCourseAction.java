/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.course;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Course;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Teacher;
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
import org.hibernate.Hibernate;
import org.hibernate.Session;

/**
 *
 * @author Administrator
 */
public class AddCourseAction extends GlobalActionSupport {

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
        HibernateUtil.close();
        HibernateUtil.init();
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

    public void destroy() {
        id = "";
        name = null;
        target = null;
        type = null;
        state = null;
        selectedId = null;
        teacherList = null;
        item = null;
        creditHour = 0;
        creditValue = 0;
    }

    public String insert() {

        if (teacherList.length() == 0 || name.length() == 0||id==null||id.length()==0||teacherList==null||name==null||name.trim().equals("")) {
            setMsgTitle("添加课程信息");
            setMsgInformation("您必须填写完整信息！");
        } else {
            String[] teachers = teacherList.split("~");
            Course course = new Course();
            course.setId(id);
            course.setName(name);
            course.setCreditHour(creditHour);
            course.setCreditValue(creditValue);
            course.setEstablished(state.equals("已开设"));
            course.setTarget(target);
            course.setType(type);

            for (String iteacher : teachers) {
                String temp[] = iteacher.split(" ");
                Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, temp[1]);
                course.getTeachers().add(teacher);
                //teacher.getCourses().add(course);
            }
            HibernateUtil.save(course);
            
            destroy();
            setMsgTitle("成功添加信息！");
        }
        return SUCCESS;
    }
    
    public String export() {
        WebdiskUtil.clearTempFile();  // 第一步，清理
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("course.xls")); // 第二步，载入
            Iterator<Course> iterator=getCourseList().iterator();
            boolean first = true;
            int lineNumber=3;
            while (iterator.hasNext()) {
                
                if (first) {
                    first = false;
                    lineNumber = 3;
                } 
                Course _Course=iterator.next();
                Iterator<Teacher> it=_Course.getTeachers().iterator();
                while(it.hasNext()){
                    Teacher _Teacher=it.next();
                    workbook.setCenterCellValue(0, lineNumber, 0, _Course.getName());
                    workbook.setCenterCellValue(0, lineNumber, 1, _Teacher.getName());
                    workbook.setCenterCellValue(0, lineNumber, 2,_Teacher.getExpertTitle());
                    workbook.setCenterCellValue(0, lineNumber, 3,_Teacher.getDepartment());
                    workbook.setCenterCellValue(0, lineNumber, 4, String.valueOf(_Course.getCreditHour()));
                    workbook.setCenterCellValue(0, lineNumber, 5, ((_Course.isEstablished()==true)?("已开设"):("拟开设")));
                    lineNumber = workbook.createNewLineWithBorder(0);
                }
            }
            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("course.xls")); // 第三步，保存
            workbook.close(); // 第四步，释放资源与返回INPUT
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception { // 第五步，定义下载入口流；第六步修改struts2配置文件
        return WebdiskUtil.getSaveDocumentStream("course.xls");
    }
}

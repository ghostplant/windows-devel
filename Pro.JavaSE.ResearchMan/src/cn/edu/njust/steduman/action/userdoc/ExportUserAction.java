package cn.edu.njust.steduman.action.userdoc;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Course;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Project;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.database.TeacherOfProject;
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import org.hibernate.Query;

/**
 *
 * @author 编程是一门艺术
 */
public class ExportUserAction extends GlobalActionSupport {

    private String selectedUser;

    public String getSelectedUser() {
        return selectedUser;
    }

    public void setSelectedUser(String selectedUser) {
        this.selectedUser = selectedUser;
    }

    public List getUsers() {
        List<Teacher> t = HibernateUtil.getList(Teacher.class);
        Iterator<Teacher> it = t.iterator();
        List<String> ret = new ArrayList<String>();
        Teacher teacher;
        while (it.hasNext()) {
            teacher = it.next();
            if (teacher.isFreeze() == false) {
                ret.add(teacher.getId() + ":" + teacher.getName());
            }
        }
        return ret;
    }

    public int castToYear(Date date) {
        SimpleDateFormat formater = new SimpleDateFormat("yyyy-MM-dd");
        String ans = formater.format(date);
        String[] temp = ans.split("-");
        return Integer.valueOf(temp[0]);
    }

    public int castToMonth(Date date) {
        SimpleDateFormat formater = new SimpleDateFormat("yyyy-MM-dd");
        String ans = formater.format(date);
        String[] temp = ans.split("-");
        return Integer.valueOf(temp[1]);
    }
    Calendar ca = Calendar.getInstance();

    public Calendar getCa() {
        return ca;
    }

    public void setCa(Calendar ca) {
        this.ca = ca;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }
    private int year = ca.get(Calendar.YEAR);

    public String getStuDocNum(int enyear, String teacherID) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as stu where teacherId=:teacherID and entranceYear=:Year and degree='博士'");
        query.setString("teacherID", teacherID);
        query.setString("Year", String.valueOf(enyear));

        String ans = query.uniqueResult().toString();
        return ans;

    }

    public String getStuMasNum(int enyear, String teacherID) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as stu where teacherId=:teacherID and entranceYear=:Year and degree='硕士'");
        query.setString("teacherID", teacherID);
        query.setString("Year", String.valueOf(enyear));

        String ans = query.uniqueResult().toString();
        return ans;

    }

    public String getGraDocNum(int grayear, String teacherID) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as stu where teacherId=:teacherID and graduationYear=:Year and degree='博士'");
        query.setString("teacherID", teacherID);
        query.setString("Year", String.valueOf(grayear));

        String ans = query.uniqueResult().toString();
        return ans;

    }

    public String getGraMasNum(int grayear, String teacherID) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as stu where teacherId=:teacherID and graduationYear=:Year and degree='硕士'");
        query.setString("teacherID", teacherID);
        query.setString("Year", String.valueOf(grayear));

        String ans = query.uniqueResult().toString();
        return ans;

    }

    public String export() {
        String[] teacherName = selectedUser.split(":");
        if (teacherName[0] == null || teacherName[0].length() == 0) {
            setMsgTitle("输入有误");
            setMsgInformation("请检查输入");
            return SUCCESS;
        }
        String teacherID = teacherName[0];
        Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, teacherID);
        Person person = (Person) HibernateUtil.get(Person.class, teacherID);
        Set projectSet = teacher.getTeacherOfProjects();
        Set courseSet = teacher.getCourses();
        if (person == null || teacher == null) {
            setMsgTitle("教师信息不存在");
            setMsgInformation("请检查数据库");
            return SUCCESS;
        }
        WebdiskUtil.clearTempFile();  // 第一步，清理
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("user.xls")); // 第二步，载入
            workbook.setCenterCellValue(0, 1, 1, teacher.getName());
            workbook.setCenterCellValue(0, 1, 3, (person.isMale() == true) ? "男" : "女");
            workbook.setCenterCellValue(0, 1, 6, String.valueOf(castToYear(teacher.getBirthDate()) + "." + String.valueOf(castToMonth(teacher.getBirthDate()))));
            workbook.setCenterCellValue(0, 1, 12, teacher.getExpertTitle());
            workbook.setCenterCellValue(0, 2, 4, teacher.getFinalDegree() + "  " + teacher.getFinalSchool() + "  " + teacher.getFinalMajor() + "  " + String.valueOf(castToYear(teacher.getGraduationDate()) + "." + String.valueOf(castToMonth(teacher.getGraduationDate()))));


            if (projectSet != null) {
                int cnt = 11;
                Iterator iterator = projectSet.iterator();
                while (iterator.hasNext() && cnt <= 13) {
                    TeacherOfProject nexTeacherOfProject = (TeacherOfProject) iterator.next();
                    Project project = (Project) HibernateUtil.get(Project.class, nexTeacherOfProject.getProjectId());
                    workbook.setCenterCellValue(0, cnt, 1, project.getName());
                    workbook.setCenterCellValue(0, cnt, 6, project.getSource());
                    workbook.setCenterCellValue(0, cnt, 8, String.valueOf(project.getStartYear()) + "-" + String.valueOf(project.getEndYear()));
                    workbook.setCenterCellValue(0, cnt, 9, String.valueOf(project.getCost()));
                    workbook.setCenterCellValue(0, cnt, 11, teacher.getWorkingSection());
                    cnt++;

                    System.out.println(project.getName());
                    System.out.println(project.getSource());
                    System.out.println(teacher.getWorkingSection());
                }
            }

            workbook.setCenterCellValue(0, 16, 1, String.valueOf(year - 4));
            workbook.setCenterCellValue(0, 16, 2, getStuDocNum(year - 4, teacher.getId()));
            workbook.setCenterCellValue(0, 16, 6, getStuMasNum(year - 4, teacher.getId()));
            workbook.setCenterCellValue(0, 16, 7, getGraDocNum(year - 4, teacher.getId()));
            workbook.setCenterCellValue(0, 16, 10, getGraMasNum(year - 4, teacher.getId()));

            workbook.setCenterCellValue(0, 17, 1, String.valueOf(year - 3));
            workbook.setCenterCellValue(0, 17, 2, getStuDocNum(year - 3, teacher.getId()));
            workbook.setCenterCellValue(0, 17, 6, getStuMasNum(year - 3, teacher.getId()));
            workbook.setCenterCellValue(0, 17, 7, getGraDocNum(year - 3, teacher.getId()));
            workbook.setCenterCellValue(0, 17, 10, getGraMasNum(year - 3, teacher.getId()));

            workbook.setCenterCellValue(0, 18, 1, String.valueOf(year - 2));
            workbook.setCenterCellValue(0, 18, 2, getStuDocNum(year - 2, teacher.getId()));
            workbook.setCenterCellValue(0, 18, 6, getStuMasNum(year - 2, teacher.getId()));
            workbook.setCenterCellValue(0, 18, 7, getGraDocNum(year - 2, teacher.getId()));
            workbook.setCenterCellValue(0, 18, 10, getGraMasNum(year - 2, teacher.getId()));

            workbook.setCenterCellValue(0, 19, 1, String.valueOf(year - 1));
            workbook.setCenterCellValue(0, 19, 2, getStuDocNum(year - 1, teacher.getId()));
            workbook.setCenterCellValue(0, 19, 6, getStuMasNum(year - 1, teacher.getId()));
            workbook.setCenterCellValue(0, 19, 7, getGraDocNum(year - 1, teacher.getId()));
            workbook.setCenterCellValue(0, 19, 10, getGraMasNum(year - 1, teacher.getId()));

            workbook.setCenterCellValue(0, 20, 1, String.valueOf(year));
            workbook.setCenterCellValue(0, 20, 2, getStuDocNum(year, teacher.getId()));
            workbook.setCenterCellValue(0, 20, 6, getStuMasNum(year, teacher.getId()));
            workbook.setCenterCellValue(0, 20, 7, getGraDocNum(year, teacher.getId()));
            workbook.setCenterCellValue(0, 20, 10, getGraMasNum(year, teacher.getId()));

            if (courseSet != null) {
                Iterator it = courseSet.iterator();
                int lineNumber = 22;
                while (it.hasNext() && lineNumber <= 24) {
                    Course course = (Course) it.next();
                    workbook.setCenterCellValue(0, lineNumber, 5, course.getName());
                    workbook.setCenterCellValue(0, lineNumber, 10, String.valueOf(course.getCreditHour()));
                    workbook.setCenterCellValue(0, lineNumber, 12, course.getTarget());
                    lineNumber++;
                }
            }

            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("user.xls")); // 第三步，保存
            workbook.close(); // 第四步，释放资源与返回INPUT
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，或者用户信息不完整，不能导出！");
        }
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception { // 第五步，定义下载入口流；第六步修改struts2配置文件
        return WebdiskUtil.getSaveDocumentStream("user.xls");
    }
}

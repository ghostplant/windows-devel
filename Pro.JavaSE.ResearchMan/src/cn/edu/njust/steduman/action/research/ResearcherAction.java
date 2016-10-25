/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.research;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Iterator;
import org.hibernate.Hibernate;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import org.hibernate.Query;

/**
 *
 * @author Administrator
 */
public class ResearcherAction extends GlobalActionSupport {

    private int doctorNumber, masterNumber;
    private int a11, a12, a13, a14, a15;
    private int a21, a22, a23, a24, a25;
    private int a31, a32, a33, a34, a35;
    Calendar ca = Calendar.getInstance();
    private int year = ca.get(Calendar.YEAR);//获取年份

    public int getA11() {
        return a11;
    }

    public void setA11(int a11) {
        this.a11 = a11;
    }

    public int getA12() {
        return a12;
    }

    public void setA12(int a12) {
        this.a12 = a12;
    }

    public int getA13() {
        return a13;
    }

    public void setA13(int a13) {
        this.a13 = a13;
    }

    public int getA14() {
        return a14;
    }

    public void setA14(int a14) {
        this.a14 = a14;
    }

    public int getA15() {
        return a15;
    }

    public void setA15(int a15) {
        this.a15 = a15;
    }

    public int getA21() {
        return a21;
    }

    public void setA21(int a21) {
        this.a21 = a21;
    }

    public int getA22() {
        return a22;
    }

    public void setA22(int a22) {
        this.a22 = a22;
    }

    public int getA23() {
        return a23;
    }

    public void setA23(int a23) {
        this.a23 = a23;
    }

    public int getA24() {
        return a24;
    }

    public void setA24(int a24) {
        this.a24 = a24;
    }

    public int getA25() {
        return a25;
    }

    public void setA25(int a25) {
        this.a25 = a25;
    }

    public int getA31() {
        return a31;
    }

    public void setA31(int a31) {
        this.a31 = a31;
    }

    public int getA32() {
        return a32;
    }

    public void setA32(int a32) {
        this.a32 = a32;
    }

    public int getA33() {
        return a33;
    }

    public void setA33(int a33) {
        this.a33 = a33;
    }

    public int getA34() {
        return a34;
    }

    public void setA34(int a34) {
        this.a34 = a34;
    }

    public int getA35() {
        return a35;
    }

    public void setA35(int a35) {
        this.a35 = a35;
    }

    public Calendar getCa() {
        return ca;
    }

    public void setCa(Calendar ca) {
        this.ca = ca;
    }

    public int getDoctorNumber() {
        return doctorNumber;
    }

    public void setDoctorNumber(int doctorNumber) {
        this.doctorNumber = doctorNumber;
    }

    public int getMasterNumber() {
        return masterNumber;
    }

    public void setMasterNumber(int masterNumber) {
        this.masterNumber = masterNumber;
    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
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
    private Set majors = new HashSet();

    public Set getMajors() {
        return majors;
    }

    public void setMajors(Set majors) {
        this.majors = majors;
    }

    public Set getResearcherSet(String majorName) {
        Set R = new HashSet();
        Query query = HibernateUtil.getSession().createQuery("from Teacher as tea where major=:majorname");
        query.setString("majorname", majorName);
        List list = query.list();
        Iterator iterator = list.iterator();
        while (iterator.hasNext()) {
            Teacher teacher = (Teacher) iterator.next();
            if (teacher == null) {
                continue;
            }
            R.add(teacher);
        }
        return R;
    }

    public String getBirthDate(Teacher teacher) {
        String temp = "";
        temp += castToYear(teacher.getBirthDate()) + "." + castToMonth(teacher.getBirthDate());
        return temp;
    }

    public String getGraduationDate(Teacher teacher) {
        String temp = "";
        temp += castToYear(teacher.getGraduationDate()) + "." + castToMonth(teacher.getGraduationDate());
        return temp;
    }

    public String getGraDocNum(String teacherID) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as stu where teacherId=:teacherID and graduationYear between:start and:end and degree='博士'");
        query.setString("teacherID", teacherID);
        query.setString("start", String.valueOf(year - 5));
        query.setString("end", String.valueOf(year));
        String ans = query.uniqueResult().toString();
        return ans;
    }

    public String getGraMasNum(String teacherID) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as stu where teacherId=:teacherID and graduationYear between:start and:end and degree='硕士'");
        query.setString("teacherID", teacherID);
        query.setString("start", String.valueOf(year - 5));
        query.setString("end", String.valueOf(year));
        String ans = query.uniqueResult().toString();
        return ans;
    }

    public String getStuMasNum(String teacherID) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as stu where teacherId=:teacherID and graduationYear >:end and degree='硕士'");
        query.setString("teacherID", teacherID);
        //   query.setString("start",String.valueOf(year-5));
        query.setString("end", String.valueOf(year));
        String ans = query.uniqueResult().toString();
        return ans;
    }

    public String getStuDocNum(String teacherID) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as stu where teacherId=:teacherID and graduationYear >:end and degree='博士'");
        query.setString("teacherID", teacherID);
        // query.setString("start",String.valueOf(year-5));
        query.setString("end", String.valueOf(year));
        String ans = query.uniqueResult().toString();
        return ans;
    }

    @Override
    public String init() {
        super.init();
        Iterator iterator = HibernateUtil.getList(Teacher.class).iterator();
        while (iterator.hasNext()) {
            Teacher teacher = (Teacher) iterator.next();
            // System.out.print(teacher.getName());
            if (teacher == null) {
                continue;
            }
            Date date = teacher.getBirthDate();
            if (date == null) {
                continue;
            }
            if (teacher.getFinalDegree() == null || teacher.getFinalDegree().length() == 0) {
                continue;
            }

//            majors.
            if (teacher.getFinalDegree().equals("博士")) {
                doctorNumber++;
            } else if (teacher.getFinalDegree().equals("硕士")) {
                masterNumber++;
            }

            int birthYear = castToYear(date);
            int old = year - birthYear;
            if (teacher.getExpertTitle().equals("教授")) {
                if (old <= 35) {
                    a11++;
                } else if (old > 35 && old <= 45) {
                    a12++;
                } else if (old > 45 && old <= 55) {
                    a13++;
                } else if (old > 55 && old <= 60) {
                    a14++;
                } else if (old > 61) {
                    a15++;
                }
            } else if (teacher.getExpertTitle().equals("副教授")) {
                if (old <= 35) {
                    a21++;
                } else if (old > 35 && old <= 45) {
                    a22++;
                } else if (old > 45 && old <= 55) {
                    a23++;
                } else if (old > 55 && old <= 60) {
                    a24++;
                } else if (old > 61) {
                    a25++;
                }
            } else if (teacher.getExpertTitle().equals("讲师")) {
                if (old <= 35) {
                    a31++;
                } else if (old > 35 && old <= 45) {
                    a32++;
                } else if (old > 45 && old <= 55) {
                    a33++;
                } else if (old > 55 && old <= 60) {
                    a34++;
                } else if (old > 61) {
                    a35++;
                }
            }
        }
        majors = new HashSet();

        Iterator it = HibernateUtil.getList(Teacher.class).iterator();
        while (it.hasNext()) {
            Teacher teacher = (Teacher) it.next();
            if (teacher == null || teacher.getMajor() == null || teacher.getMajor().length() == 0) {
                continue;
            }
            majors.add(teacher.getMajor());
        }
        return SUCCESS;
    }

    public String export() {
        Iterator iter = HibernateUtil.getList(Teacher.class).iterator();
        while (iter.hasNext()) {
            Teacher teacher = (Teacher) iter.next();
          
            if (teacher == null) {
                continue;
            }
            Date date = teacher.getBirthDate();
            if (date == null) {
                continue;
            }
            if (teacher.getFinalDegree() == null || teacher.getFinalDegree().length() == 0) {
                continue;
            }

            if (teacher.getFinalDegree().equals("博士")) {
                doctorNumber++;
            } else if (teacher.getFinalDegree().equals("硕士")) {
                masterNumber++;
            }

            int birthYear = castToYear(date);
            int old = year - birthYear;
            if (teacher.getExpertTitle().equals("教授")) {
                if (old <= 35) {
                    a11++;
                } else if (old > 35 && old <= 45) {
                    a12++;
                } else if (old > 45 && old <= 55) {
                    a13++;
                } else if (old > 55 && old <= 60) {
                    a14++;
                } else if (old > 61) {
                    a15++;
                }
            } else if (teacher.getExpertTitle().equals("副教授")) {
                if (old <= 35) {
                    a21++;
                } else if (old > 35 && old <= 45) {
                    a22++;
                } else if (old > 45 && old <= 55) {
                    a23++;
                } else if (old > 55 && old <= 60) {
                    a24++;
                } else if (old > 61) {
                    a25++;
                }
            } else if (teacher.getExpertTitle().equals("讲师")) {
                if (old <= 35) {
                    a31++;
                } else if (old > 35 && old <= 45) {
                    a32++;
                } else if (old > 45 && old <= 55) {
                    a33++;
                } else if (old > 55 && old <= 60) {
                    a34++;
                } else if (old > 61) {
                    a35++;
                }
            }
        }

        WebdiskUtil.clearTempFile();  // 第一步，清理
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("researcher.xls")); // 第二步，载入

            workbook.setCenterCellValue(0, 2, 1, String.valueOf(a11 + a12 + a13 + a14 + a15));
            workbook.setCenterCellValue(0, 2, 2, String.valueOf(a11));
            workbook.setCenterCellValue(0, 2, 3, String.valueOf(a12));
            workbook.setCenterCellValue(0, 2, 4, String.valueOf(a13));
            workbook.setCenterCellValue(0, 2, 5, String.valueOf(a14));
            workbook.setCenterCellValue(0, 2, 6, String.valueOf(a15));

            workbook.setCenterCellValue(0, 3, 1, String.valueOf(a21 + a22 + a23 + a24 + a25));
            workbook.setCenterCellValue(0, 3, 2, String.valueOf(a21));
            workbook.setCenterCellValue(0, 3, 3, String.valueOf(a22));
            workbook.setCenterCellValue(0, 3, 4, String.valueOf(a23));
            workbook.setCenterCellValue(0, 3, 5, String.valueOf(a24));
            workbook.setCenterCellValue(0, 3, 6, String.valueOf(a25));

            workbook.setCenterCellValue(0, 4, 1, String.valueOf(a31 + a32 + a33 + a34 + a35));
            workbook.setCenterCellValue(0, 4, 2, String.valueOf(a31));
            workbook.setCenterCellValue(0, 4, 3, String.valueOf(a32));
            workbook.setCenterCellValue(0, 4, 4, String.valueOf(a33));
            workbook.setCenterCellValue(0, 4, 5, String.valueOf(a34));
            workbook.setCenterCellValue(0, 4, 6, String.valueOf(a35));

            workbook.setCenterCellValue(0, 2, 7, String.valueOf(doctorNumber));
            workbook.setCenterCellValue(0, 2, 8, String.valueOf(masterNumber));


            Iterator major = HibernateUtil.getList(Teacher.class).iterator();
            while (major.hasNext()) {
                Teacher teacher = (Teacher) major.next();
                if (teacher == null || teacher.getMajor() == null || teacher.getMajor().length() == 0) {
                    continue;
                }
                majors.add(teacher.getMajor());
            }
            Iterator iterator = majors.iterator();
            boolean first = true;
            int lineNumber = 8;
            while (iterator.hasNext()) {

                if (first) {
                    first = false;
                    lineNumber = 8;
                }
                String majorString = (String) iterator.next();
                if (majorString == null) {
                    continue;
                }
                workbook.setCenterCellValue(0, lineNumber, 0, majorString);
            
                Query query = HibernateUtil.getSession().createQuery("from Teacher as tea where major=:majorname");
                query.setString("majorname", majorString);
                List list = query.list();
                Iterator it=list.iterator();


                while (it.hasNext()) {
                    Teacher _Teacher = (Teacher) it.next();
                    if (_Teacher == null) {
                        continue;
                    }
                    workbook.setCenterCellValue(0, lineNumber, 1, _Teacher.getName());


                  
                    workbook.setCenterCellValue(0, lineNumber, 2, getBirthDate(_Teacher));
                    workbook.setCenterCellValue(0, lineNumber, 3, getGraduationDate(_Teacher));

                    workbook.setCenterCellValue(0, lineNumber, 4, _Teacher.getExpertTitle());
                    
                    workbook.setCenterCellValue(0, lineNumber, 5, getGraDocNum(_Teacher.getId()));
                    workbook.setCenterCellValue(0, lineNumber, 6, getStuDocNum(_Teacher.getId()));
                    workbook.setCenterCellValue(0, lineNumber, 7, getGraMasNum(_Teacher.getId()));
                    workbook.setCenterCellValue(0, lineNumber, 8, getStuMasNum(_Teacher.getId()));
                  lineNumber = workbook.createNewLineWithBorder(0);
                }
            }
            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("researcher.xls")); // 第三步，保存
            workbook.close(); // 第四步，释放资源与返回INPUT
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception { // 第五步，定义下载入口流；第六步修改struts2配置文件
        return WebdiskUtil.getSaveDocumentStream("researcher.xls");
    }
}
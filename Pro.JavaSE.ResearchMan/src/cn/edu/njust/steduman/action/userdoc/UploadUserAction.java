/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.userdoc;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Student;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.MD5Util;
import cn.edu.njust.steduman.util.WebdiskUtil;
import cn.edu.njust.steduman.util.ZIPUtil;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashSet;
import java.util.Set;
import javax.servlet.ServletContext;
import org.apache.commons.io.FileUtils;
import org.apache.struts2.ServletActionContext;
import org.apache.struts2.util.ServletContextAware;

/**
 *
 * @author Ghostplant
 */
public class UploadUserAction extends GlobalActionSupport implements ServletContextAware {

    private File upload;

    public File getUpload() {
        return upload;
    }

    public void setUpload(File upload) {
        this.upload = upload;
    }
    private Set results;

    public Set getResults() {
        return results;
    }

    public void setResults(Set results) {
        this.results = results;
    }

    public ServletContext getContext() {
        return context;
    }

    public void setContext(ServletContext context) {
        this.context = context;
    }
    private int successCount, failedCount, errorCount;

    public int getErrorCount() {
        return errorCount;
    }

    public void setErrorCount(int errorCount) {
        this.errorCount = errorCount;
    }

    public int getFailedCount() {
        return failedCount;
    }

    public void setFailedCount(int failedCount) {
        this.failedCount = failedCount;
    }

    public int getSuccessCount() {
        return successCount;
    }

    public void setSuccessCount(int successCount) {
        this.successCount = successCount;
    }
    private boolean finished = false;

    public boolean isFinished() {
        return finished;
    }

    public void setFinished(boolean finished) {
        this.finished = finished;
    }

    public InputStream getInputStream() throws Exception {
        return ServletActionContext.getServletContext().getResourceAsStream("/webdisk/template/register.xls");
    }

    public String template() {
        return "download";
    }

    @Override
    public String execute() {
        if (upload == null) {
            setMsgTitle("上传文件信息");
            setMsgInformation("请您选则要上传的zip压缩包！");
        } else {
            if (upload.length() > 5 * 1024 * 1024) {
                setMsgTitle("上传文件信息");
                setMsgInformation("上传文件不允许超过5M！");
                return SUCCESS;
            }
            File dirPath = new File(context.getRealPath("/webdisk/common"));
            File target = new File(dirPath, "zipdoc.zip");
            results = new HashSet();
            successCount = 0;
            failedCount = 0;
            errorCount = 0;
            try {
                FileUtils.copyFile(upload, target);
                ZIPUtil.unzip(target, dirPath);
                String[] documents = dirPath.list();
                for (String document : documents) {
                    String result = document + "：";
                    boolean formatAccepted = true;
                    String url = dirPath.getPath() + "/" + document;
                    ExcelUtil workbook = null;
                    String userid = null, username = null, gender = null, department = null, password = null;
                    try {
                        workbook = new ExcelUtil(url);
                        userid = workbook.getCellValue(0, 1, 1);
                        username = workbook.getCellValue(0, 2, 1);
                        gender = workbook.getCellValue(0, 3, 1);
                        department = workbook.getCellValue(0, 4, 1);
                        password = workbook.getCellValue(0, 5, 1);
                    } catch (Exception ex) {
                        formatAccepted = false;
                    } finally {
                        try {
                            workbook.close();
                        } catch (Exception ex) {
                        }
                    }
                    new File(url).delete();
                    if (!formatAccepted || gender == null || userid == null || userid.length() == 0 || username == null || username.length() == 0) {
                        result += "文档格式不正确；";
                        errorCount++;
                    } else {
                        boolean male = gender.equals("男");
                        if (userid.length() <= 6 && (password == null || password.length() == 0)) {
                            result += "'" + username + "'(" + userid + ")添加失败：教师密码不能为空；";
                            failedCount++;
                        } else if (HibernateUtil.get(Person.class, userid) != null) {
                            result += "'" + username + "'(" + userid + ")添加失败：该用户ID已在数据库中存在；";
                            failedCount++;
                        } else {
                            Person person = new Person();
                            person.setId(userid);
                            person.setMale(male);
                            person.setName(username);
                            if (userid.length() <= 6) {
                                person.setTeacher(true);
                                Teacher teacher = new Teacher();
                                teacher.setId(userid);
                                teacher.setPasswordHashCode(MD5Util.md5(password));
                                teacher.setFreeze(false);
                                teacher.setDepartment(department);
                                HibernateUtil.save(teacher);
                            } else {
                                person.setTeacher(false);
                                Student student = new Student();
                                student.setId(userid);
                                student.setDormNo(department);
                                HibernateUtil.save(student);
                            }
                            HibernateUtil.save(person);
                            result += (person.isTeacher() ? "教师" : "学生") + "'" + username + "'(" + userid + ")成功添加至数据库中；";
                            successCount++;
                        }
                    }
                    results.add(result);
                }
            } catch (IOException ex) {
                if (ex.getMessage().equals("Negative seek offset")) {
                    setMsgTitle("上传文件信息");
                    setMsgInformation("对不起，您上传的不是 zip 格式的文件！");
                } else {
                    setMsgTitle("上传文件信息" + ex.getMessage());
                    setMsgInformation("上传文件失败，请确认本地文件存在，文件不能正确访问！");
                }
            } finally {
                target.delete();
            }
            finished = true;
        }
        return SUCCESS;
    }
    private ServletContext context;

    @Override
    public void setServletContext(ServletContext sc) {
        this.context = sc;
    }
}

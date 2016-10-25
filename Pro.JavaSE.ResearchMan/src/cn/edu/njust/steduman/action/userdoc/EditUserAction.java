package cn.edu.njust.steduman.action.userdoc;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.SessionUtil;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 *
 * @author 编程是一门艺术
 */
public class EditUserAction extends GlobalActionSupport {

    private String username, gender, mobilePhone, department,
            officeAddress, officePhone, major, contactAddress,
            expertTitle, finalDegree, finalSchool,
            finalMajor, workingSection;
    private Date birthady, graduationDate;

    @Override
    public String init() {
        super.init();
        if (getMenuItem() != null) {
            //person
            Person person = (Person) HibernateUtil.get(Person.class, getCurrentUserId());
            username = person.getName();
            mobilePhone = person.getMobilePhone();
            if (person.isMale() == true) {
                gender = "男";
            } else {
                gender = "女";
            }
            //teacher
            Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, getCurrentUserId());
            department = teacher.getDepartment();
            officeAddress = teacher.getOfficeAddress();
            officePhone = teacher.getOfficePhone();
            major = teacher.getMajor();
            contactAddress = teacher.getContactAddress();
            expertTitle = teacher.getExpertTitle();
            finalDegree = teacher.getFinalDegree();
            finalSchool = teacher.getFinalSchool();
            finalMajor = teacher.getFinalMajor();
            workingSection = teacher.getWorkingSection();
            birthady = teacher.getBirthDate();
            graduationDate = teacher.getGraduationDate();

        }
        return SUCCESS;
    }

    public String getBirthady() {
        SimpleDateFormat formater = new SimpleDateFormat("yyyy-MM-dd");
        return formater.format(birthady);
    }

    public void setBirthady(Date birthady) {
        this.birthady = birthady;
    }

    public String getContactAddress() {
        return contactAddress;
    }

    public void setContactAddress(String contactAddress) {
        this.contactAddress = contactAddress;
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }

    public String getExpertTitle() {
        return expertTitle;
    }

    public void setExpertTitle(String expertTitle) {
        this.expertTitle = expertTitle;
    }

    public String getFinalDegree() {
        return finalDegree;
    }

    public void setFinalDegree(String finalDegree) {
        this.finalDegree = finalDegree;
    }

    public String getFinalMajor() {
        return finalMajor;
    }

    public void setFinalMajor(String finalMajor) {
        this.finalMajor = finalMajor;
    }

    public String getFinalSchool() {
        return finalSchool;
    }

    public void setFinalSchool(String finalSchool) {
        this.finalSchool = finalSchool;
    }

    public String getGender() {
        return gender;
    }

    public void setGender(String gender) {
        this.gender = gender;
    }

    public String getGraduationDate() {
        SimpleDateFormat formater = new SimpleDateFormat("yyyy-MM-dd");
        return formater.format(graduationDate);
    }

    public void setGraduationDate(Date graduationDate) {
        this.graduationDate = graduationDate;
    }

    public String getMajor() {
        return major;
    }

    public void setMajor(String major) {
        this.major = major;
    }

    public String getMobilePhone() {
        return mobilePhone;
    }

    public void setMobilePhone(String mobilePhone) {
        this.mobilePhone = mobilePhone;
    }

    public String getOfficeAddress() {
        return officeAddress;
    }

    public void setOfficeAddress(String officeAddress) {
        this.officeAddress = officeAddress;
    }

    public String getOfficePhone() {
        return officePhone;
    }

    public void setOfficePhone(String officePhone) {
        this.officePhone = officePhone;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getWorkingSection() {
        return workingSection;
    }

    public void setWorkingSection(String workingSection) {
        this.workingSection = workingSection;
    }

    public String getCurrentUserId() {
        return ((Teacher) SessionUtil.getSession("user")).getId();
    }

    public String change() {
        //validate
        if (null == username || "".equals(username)) {
            setMsgTitle("没有输入用户名");
            return SUCCESS;
        }

        Person person = (Person) HibernateUtil.get(Person.class, getCurrentUserId());
        person.setName(username);
        person.setMale(gender.equals("男"));
        person.setMobilePhone(mobilePhone);

        Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, getCurrentUserId());
        teacher.setDepartment(department);
        teacher.setOfficeAddress(officeAddress);
        teacher.setOfficePhone(officePhone);
        teacher.setMajor(major);
        teacher.setContactAddress(contactAddress);
        teacher.setBirthDate(birthady);
        teacher.setExpertTitle(expertTitle);
        teacher.setFinalDegree(finalDegree);
        teacher.setFinalSchool(finalSchool);
        teacher.setFinalMajor(finalMajor);
        teacher.setGraduationDate(graduationDate);
        teacher.setWorkingSection(workingSection);

        HibernateUtil.update(teacher);
        HibernateUtil.update(person);

        setMsgTitle("已修改用户" + person.getName());
        return SUCCESS;
    }
}

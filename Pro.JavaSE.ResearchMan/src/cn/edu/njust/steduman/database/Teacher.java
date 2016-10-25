/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.Date;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;

/**
 *
 * @author Ghostplant
 */
public class Teacher {

    private String name, id, department, passwordHashCode, officeAddress, officePhone, major, contactAddress, expertTitle, finalDegree, finalSchool, finalMajor, workingSection;

    public void setName(String name) {
        this.name = name;
    }
    private Set roles = new HashSet(), courses = new HashSet(), teacherOfAchievements = new HashSet(), teacherOfPatents = new HashSet(), teacherOfProjects = new HashSet(),
            sendedMessages = new HashSet(), deliveredMessages = new HashSet();

    public Set getDeliveredMessages() {
        return deliveredMessages;
    }

    public void setDeliveredMessages(Set deliveredMessages) {
        this.deliveredMessages = deliveredMessages;
    }

    public Set getSendedMessages() {
        return sendedMessages;
    }

    public void setSendedMessages(Set sendedMessages) {
        this.sendedMessages = sendedMessages;
    }

    public Set getCourses() {
        return courses;
    }

    public void setCourses(Set courses) {
        this.courses = courses;
    }

    public Set getTeacherOfProjects() {
        return teacherOfProjects;
    }

    public void setTeacherOfProjects(Set teacherOfProjects) {
        this.teacherOfProjects = teacherOfProjects;
    }

    public Set getTeacherOfPatents() {
        return teacherOfPatents;
    }

    public void setTeacherOfPatents(Set teacherOfPatents) {
        this.teacherOfPatents = teacherOfPatents;
    }

    public Set getTeacherOfAchievements() {
        return teacherOfAchievements;
    }

    public void setTeacherOfAchievements(Set teacherOfAchievements) {
        this.teacherOfAchievements = teacherOfAchievements;
    }

    public Set getRoles() {
        return roles;
    }

    public void setRoles(Set roles) {
        this.roles = roles;
    }

    public String getDepartment() {
        return department;
    }

    public void setDepartment(String department) {
        this.department = department;
    }
    private java.util.Date birthDate, graduationDate;
    private boolean freeze;

    public Date getBirthDate() {
        return birthDate;
    }

    public void setBirthDate(Date birthDate) {
        this.birthDate = birthDate;
    }

    public String getContactAddress() {
        return contactAddress;
    }

    public void setContactAddress(String contactAddress) {
        this.contactAddress = contactAddress;
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

    public boolean isFreeze() {
        return freeze;
    }

    public void setFreeze(boolean freeze) {
        this.freeze = freeze;
    }

    public Date getGraduationDate() {
        return graduationDate;
    }

    public void setGraduationDate(Date graduationDate) {
        this.graduationDate = graduationDate;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getMajor() {
        return major;
    }

    public void setMajor(String major) {
        this.major = major;
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

    public String getPasswordHashCode() {
        return passwordHashCode;
    }

    public void setPasswordHashCode(String passwordHashCode) {
        this.passwordHashCode = passwordHashCode;
    }

    public String getWorkingSection() {
        return workingSection;
    }

    public void setWorkingSection(String workingSection) {
        this.workingSection = workingSection;
    }

    public String getName() {
        return ((Person) HibernateUtil.get(Person.class, id)).getName();
    }

    public boolean hasAccess(String mod, String func) {
        String authority = mod + "#" + func;
        Set<Role> myroles = getRoles();
        Iterator<Role> rit = myroles.iterator();
        while (rit.hasNext()) {
            Set<Authority> authorities = rit.next().getAuthorities();
            Iterator<Authority> ait = authorities.iterator();
            while (ait.hasNext()) {
                if (ait.next().getFunction().equals(authority)) {
                    return true;
                }
            }
        }
        return false;
    }
}

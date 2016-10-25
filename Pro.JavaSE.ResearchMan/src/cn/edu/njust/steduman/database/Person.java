/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

import java.util.HashSet;
import java.util.Set;

/**
 *
 * @author Ghostplant
 */
public class Person {

    private String id, name, mobilePhone;
    private boolean male, teacher;
    private Set emails=new HashSet(), experiences=new HashSet(), personOfEssays=new HashSet();

    public Set getPersonOfEssays() {
        return personOfEssays;
    }

    public void setPersonOfEssays(Set personOfEssays) {
        this.personOfEssays = personOfEssays;
    }

    public Set getEmails() {
        return emails;
    }

    public void setEmails(Set emails) {
        this.emails = emails;
    }

    public Set getExperiences() {
        return experiences;
    }

    public void setExperiences(Set experiences) {
        this.experiences = experiences;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public boolean isMale() {
        return male;
    }

    public void setMale(boolean male) {
        this.male = male;
    }

    public String getMobilePhone() {
        return mobilePhone;
    }

    public void setMobilePhone(String mobilePhone) {
        this.mobilePhone = mobilePhone;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public boolean isTeacher() {
        return teacher;
    }

    public void setTeacher(boolean teacher) {
        this.teacher = teacher;
    }
}

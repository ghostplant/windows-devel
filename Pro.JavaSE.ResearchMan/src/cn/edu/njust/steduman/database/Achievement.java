/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

import java.util.Date;
import java.util.HashSet;
import java.util.Set;

/**
 *
 * @author Ghostplant
 */
public class Achievement {

    private int id, finishCount;
    private String name, award, level, certificationId, type;
    private java.util.Date date;
    private Set teacherOfAchievements=new HashSet();

    public Set getTeacherOfAchievements() {
        return teacherOfAchievements;
    }

    public void setTeacherOfAchievements(Set teacherOfAchievements) {
        this.teacherOfAchievements = teacherOfAchievements;
    }

    public String getAward() {
        return award;
    }

    public void setAward(String award) {
        this.award = award;
    }

    public String getCertificationId() {
        return certificationId;
    }

    public void setCertificationId(String certificationId) {
        this.certificationId = certificationId;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public int getFinishCount() {
        return finishCount;
    }

    public void setFinishCount(int finishCount) {
        this.finishCount = finishCount;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getLevel() {
        return level;
    }

    public void setLevel(String level) {
        this.level = level;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }
}

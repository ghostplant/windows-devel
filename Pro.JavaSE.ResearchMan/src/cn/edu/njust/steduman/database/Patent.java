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
public class Patent {

    private int id, finishCount;
    private java.util.Date date;
    private String registryNo, type, name;
    private Set teacherOfPatents=new HashSet();

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

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getRegistryNo() {
        return registryNo;
    }

    public void setRegistryNo(String registryNo) {
        this.registryNo = registryNo;
    }

    public Set getTeacherOfPatents() {
        return teacherOfPatents;
    }

    public void setTeacherOfPatents(Set teacherOfPatents) {
        this.teacherOfPatents = teacherOfPatents;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }
}

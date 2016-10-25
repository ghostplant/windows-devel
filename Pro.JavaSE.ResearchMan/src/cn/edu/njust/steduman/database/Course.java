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
public class Course {

    private String id, name, target, type;
    private int creditHour;
    private float creditValue;
    private boolean established;
    private Set teachers=new HashSet();

    public Set getTeachers() {
        return teachers;
    }

    public void setTeachers(Set teachers) {
        this.teachers = teachers;
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
}

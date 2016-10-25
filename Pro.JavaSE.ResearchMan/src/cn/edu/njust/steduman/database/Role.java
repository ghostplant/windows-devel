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
public class Role {

    private int id;
    private String name;
    private Set authorities=new HashSet(), teachers=new HashSet();

    public Set getTeachers() {
        return teachers;
    }

    public void setTeachers(Set teachers) {
        this.teachers = teachers;
    }

    public Set getAuthorities() {
        return authorities;
    }

    public void setAuthorities(Set authorities) {
        this.authorities = authorities;
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
}

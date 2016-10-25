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
public class Authority {

    private int id;
    private String name, function;

    public String getFunction() {
        return function;
    }

    public void setFunction(String function) {
        this.function = function;
    }
    private Set roles = new HashSet();

    public Set getRoles() {
        return roles;
    }

    public void setRoles(Set roles) {
        this.roles = roles;
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

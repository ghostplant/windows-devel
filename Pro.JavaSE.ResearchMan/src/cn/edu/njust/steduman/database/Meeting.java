/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

import java.util.Date;

/**
 *
 * @author Ghostplant
 */
public class Meeting {

    private int id, totalPeople, foreignPeople;
    private java.util.Date holdStartDate, holdEndDate;
    private String name;

    public int getForeignPeople() {
        return foreignPeople;
    }

    public void setForeignPeople(int foreignPeople) {
        this.foreignPeople = foreignPeople;
    }

    public Date getHoldEndDate() {
        return holdEndDate;
    }

    public void setHoldEndDate(Date holdEndDate) {
        this.holdEndDate = holdEndDate;
    }

    public Date getHoldStartDate() {
        return holdStartDate;
    }

    public void setHoldStartDate(Date holdStartDate) {
        this.holdStartDate = holdStartDate;
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

    public int getTotalPeople() {
        return totalPeople;
    }

    public void setTotalPeople(int totalPeople) {
        this.totalPeople = totalPeople;
    }
}

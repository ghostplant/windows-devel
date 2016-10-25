/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

/**
 *
 * @author Ghostplant
 */
public class Cooperation {

    private int id, startYear, endYear;
    private String name, target;

    public String convert(int year) {
        if (year == 0) {
            return "至今";
        } else {
            return String.valueOf(year);
        }
    }

    public int getEndYear() {
        return endYear;
    }

    public void setEndYear(int endYear) {
        this.endYear = endYear;
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

    public int getStartYear() {
        return startYear;
    }

    public void setStartYear(int startYear) {
        this.startYear = startYear;
    }

    public String getTarget() {
        return target;
    }

    public void setTarget(String target) {
        this.target = target;
    }
}

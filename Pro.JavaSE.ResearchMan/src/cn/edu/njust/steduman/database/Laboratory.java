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
public class Laboratory {

    private int id, personnelCount, deviceCount;
    private float deviceCost;
    private String name;
    private Set devices=new HashSet();

    public Set getDevices() {
        return devices;
    }

    public void setDevices(Set devices) {
        this.devices = devices;
    }

    public float getDeviceCost() {
        return deviceCost;
    }

    public void setDeviceCost(float deviceCost) {
        this.deviceCost = deviceCost;
    }

    public int getDeviceCount() {
        return deviceCount;
    }

    public void setDeviceCount(int deviceCount) {
        this.deviceCount = deviceCount;
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

    public int getPersonnelCount() {
        return personnelCount;
    }

    public void setPersonnelCount(int personnelCount) {
        this.personnelCount = personnelCount;
    }
}

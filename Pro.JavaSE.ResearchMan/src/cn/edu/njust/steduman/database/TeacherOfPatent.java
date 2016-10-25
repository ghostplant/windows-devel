/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

/**
 *
 * @author Ghostplant
 */
public class TeacherOfPatent {

    private int id, patentId, rank;
    private String teacherId;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getPatentId() {
        return patentId;
    }

    public void setPatentId(int patentId) {
        this.patentId = patentId;
    }

    public int getRank() {
        return rank;
    }

    public void setRank(int rank) {
        this.rank = rank;
    }

    public String getTeacherId() {
        return teacherId;
    }

    public void setTeacherId(String teacherId) {
        this.teacherId = teacherId;
    }
}

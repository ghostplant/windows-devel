/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.database;

/**
 *
 * @author Ghostplant
 */
public class PersonOfEssay {

    private int id, essayId, rank;
    private String personId;

    public int getEssayId() {
        return essayId;
    }

    public void setEssayId(int essayId) {
        this.essayId = essayId;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getPersonId() {
        return personId;
    }

    public void setPersonId(String personId) {
        this.personId = personId;
    }

    public int getRank() {
        return rank;
    }

    public void setRank(int rank) {
        this.rank = rank;
    }
}

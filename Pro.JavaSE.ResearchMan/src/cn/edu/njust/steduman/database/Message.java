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
public class Message {

    private int id;
    private String message, srcTeacherId, dstTeacherId;
    private java.util.Date date;
    private boolean readed;

    public boolean isReaded() {
        return readed;
    }

    public void setReaded(boolean readed) {
        this.readed = readed;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public String getDstTeacherId() {
        return dstTeacherId;
    }

    public void setDstTeacherId(String dstTeacherId) {
        this.dstTeacherId = dstTeacherId;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public String getSrcTeacherId() {
        return srcTeacherId;
    }

    public void setSrcTeacherId(String srcTeacherId) {
        this.srcTeacherId = srcTeacherId;
    }
}

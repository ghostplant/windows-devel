/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package comapi;

import javax.microedition.lcdui.StringItem;

/**
 *
 * @author Ghostplant
 */
public class StringItemEx extends StringItem {

    public String userId;

    public StringItemEx(String id, String data) {
        super(id.equals("(消息)") ? "系统" : "离线", data);
        userId = id;
    }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package callback;

import comapi.CommonLib;
import comapi.StringItemEx;
import javax.microedition.lcdui.Display;
import vector.FriendPair;

/**
 *
 * @author Ghostplant
 */
public class RemoveAsyncCallback implements AsyncCallback {

    private StringItemEx itemEx;

    public RemoveAsyncCallback(StringItemEx itemEx) {
        this.itemEx = itemEx;
    }

    public void execute() {
        String fid = itemEx.userId;
        for (int i = 0; i < CommonLib.connThread.ct.friends.size(); i++) {
            if (((FriendPair) (CommonLib.connThread.ct.friends.elementAt(i))).friendId.equals(fid)) {
                try {
                    CommonLib.send("rmfrd", new String[]{fid});
                } catch (Exception e) {
                }
            }
        }
        Display.getDisplay(CommonLib.midlet).setCurrent(CommonLib.frame);
    }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package callback;

import comapi.CommonLib;

/**
 *
 * @author Ghostplant
 */
public class ExitAsyncCallback implements AsyncCallback {

    public void execute() {
        CommonLib.midlet.notifyDestroyed();
    }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.util;

import com.opensymphony.xwork2.ActionContext;

/**
 *
 * @author Ghostplant
 */
public class SessionUtil {

    public static Object getSession(String key) {
        return ActionContext.getContext().getSession().get(key);
    }

    public static void setSession(String key, Object dat) {
        ActionContext.getContext().getSession().put(key, dat);
    }
}

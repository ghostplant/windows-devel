/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action;

import cn.edu.njust.steduman.database.Message;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.SessionUtil;
import com.opensymphony.xwork2.Action;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import org.hibernate.Query;

/**
 *
 * @author Ghostplant
 */
public class GlobalActionSupport implements Action {

    private String urlMenuName, urlMenuItem, msgTitle, msgInformation;

    public String getListViewName(String str) {
        int pos = str.indexOf("@");
        return pos >= 0 ? str.substring(0, pos) : null;
    }

    public String getListViewLink(String str) {
        int pos = str.indexOf("@");
        String build = pos >= 0 ? str.substring(pos + 1, str.length()) : null;
        if (build != null && build.endsWith("*")) {
            return build.substring(0, build.length() - 1);
        } else {
            return build;
        }
    }

    public String isRecommended(String str) {
        return str.endsWith("*") ? "true" : "false";
    }

    public int getUnreadCount() {
        Query query = HibernateUtil.getSession().createQuery("from Message where dstTeacherId=:teacherId");
        query.setString("teacherId", ((Teacher) SessionUtil.getSession("user")).getId());
        List<Message> messages = query.list();
        int tot = 0;
        Iterator<Message> it = messages.iterator();
        while (it.hasNext()) {
            if (!it.next().isReaded()) {
                tot++;
            }
        }
        return tot;
    }

    public String getUrlMenuItem() {
        return urlMenuItem;
    }

    public void setUrlMenuItem(String urlMenuItem) {
        this.urlMenuItem = urlMenuItem;
    }

    public String getUrlMenuName() {
        return urlMenuName;
    }

    public void setUrlMenuName(String urlMenuName) {
        this.urlMenuName = urlMenuName;
    }

    public String getMsgInformation() {
        return msgInformation;
    }

    public void setMsgInformation(String msgInformation) {
        this.msgInformation = msgInformation;
    }

    public String getMsgTitle() {
        return msgTitle;
    }

    public void setMsgTitle(String msgTitle) {
        this.msgTitle = msgTitle;
    }

    public String init() {
        setMenuName(urlMenuName);
        Teacher teacher = (Teacher) SessionUtil.getSession("user");
        if (teacher == null || !teacher.hasAccess(urlMenuName, urlMenuItem)) {
            setMenuItem(null);
            setMsgTitle("用户权限信息");
            setMsgInformation("您没有权限执行该操作！");
        } else {
            setMenuItem(urlMenuItem);
        }
        return SUCCESS;
    }

    public String getMenuItem() {
        return (String) SessionUtil.getSession("menuItem");
    }

    public void setMenuItem(String currentItem) {
        SessionUtil.setSession("menuItem", currentItem);
    }

    public String getMenuName() {
        return (String) SessionUtil.getSession("menuName");
    }

    public void setMenuName(String menuName) {
        SessionUtil.setSession("menuName", menuName);
    }

    public List getListView() {
        return (List) SessionUtil.getSession("listView");
    }

    public void setListView(List listView) {
        SessionUtil.setSession("listView", listView);
    }

    @Override
    public String execute() throws Exception {
        return SUCCESS;
    }

    public String allowAccess(String a, String b) {
        return ((Teacher) SessionUtil.getSession("user")).hasAccess(a, b) ? "yes" : "no";
    }
}

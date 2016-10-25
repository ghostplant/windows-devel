/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.index;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.catalist.*;
import cn.edu.njust.steduman.database.Teacher;

import cn.edu.njust.steduman.util.*;

/**
 *
 * @author zyz
 */
public class IndexAction extends GlobalActionSupport {

    private String userId, password;

    public String getUserId() {
        return userId;
    }

    public void setUserId(String userid) {
        this.userId = userid;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    private boolean doFilter() {
        if (SessionUtil.getSession("user") == null) {
            setUserInformation("对不起，您还未登录或者登录已经超时，请重新登录！");
            return false;
        }
        setMenuItem(null);
        return true;
    }

    public String toUserdoc() {
        if (doFilter()) {
            setMenuName("userdoc");
            setListView(new UserdocMan().getPageList());
        }
        return SUCCESS;
    }

    public String toRoleauth() {
        if (doFilter()) {
            setMenuName("roleauth");
            setListView(new RoleauthMan().getPageList());
        }
        return SUCCESS;
    }

    public String toAchievement() {
        if (doFilter()) {
            setMenuName("achievement");
            setListView(new AchievementMan().getPageList());
        }
        return SUCCESS;
    }

    public String toResearch() {
        if (doFilter()) {
            setMenuName("research");
            setListView(new ResearchMan().getPageList());
        }
        return SUCCESS;
    }

    public String toCourse() {
        if (doFilter()) {
            setMenuName("course");
            setListView(new CourseMan().getPageList());
        }
        return SUCCESS;
    }

    public String toTalents() {
        if (doFilter()) {
            setMenuName("talents");
            setListView(new TalentsMan().getPageList());
        }
        return SUCCESS;
    }

    public String toAcademic() {
        if (doFilter()) {
            setMenuName("academic");
            setListView(new AcademicMan().getPageList());
        }
        return SUCCESS;
    }

    public String toDocument() {
        if (doFilter()) {
            setMenuName("document");
            setListView(new DocumentMan().getPageList());
        }
        return SUCCESS;
    }
    
    private String userInformation;

    public String getUserInformation() {
        return userInformation;
    }

    public void setUserInformation(String userInformation) {
        this.userInformation = userInformation;
    }

    public String login() {
        if (userId.length() == 0 || password.length() == 0) {
            setUserInformation("错误：请您填写完整的登录信息！");
        } else {
            Teacher user = (Teacher) HibernateUtil.get(Teacher.class, userId);
            if (user == null) {
                setUserInformation("错误：该用户不存在！");
            } else if (!user.getPasswordHashCode().equals(MD5Util.md5(password))) {
                setUserInformation("错误：用户密码错误！");
            } else if (user.isFreeze()) {
                setUserInformation("对不起，您的账号已经被冻结！");
            } else {
                SessionUtil.setSession("user", user);
                setMenuItem(null);
            }
        }
        return SUCCESS;
    }

    public String logoff() {
        SessionUtil.setSession("user", null);
        return SUCCESS;
    }
}

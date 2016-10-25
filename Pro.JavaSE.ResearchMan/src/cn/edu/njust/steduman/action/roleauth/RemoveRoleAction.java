/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.roleauth;

import cn.edu.njust.steduman.database.Role;
import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Authority;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

/**
 *
 * @author Ghostplant
 */
public class RemoveRoleAction extends GlobalActionSupport {

    private int roleId;

    public int getRoleId() {
        return roleId;
    }

    public void setRoleId(int roleId) {
        this.roleId = roleId;
    }

    public List getRoles() {
        return HibernateUtil.getList(Role.class);
    }

    public String remove() {
        if (getRoleId() == 0) {
            setMsgTitle("角色删除信息");
            setMsgInformation("请您选中要删除的指定角色名！");
        } else {
            HibernateUtil.close();
            HibernateUtil.init();
            Role role = (Role) HibernateUtil.get(Role.class, roleId);
            Set<Teacher> teachers = role.getTeachers();
            Iterator<Teacher> it = teachers.iterator();
            while (it.hasNext()) {
                Teacher teacher = it.next();
                teacher.getRoles().remove(role);
                HibernateUtil.update(teacher);
            }
            role.setTeachers(null);
            Set<Authority> authorities = role.getAuthorities();
            Iterator<Authority> ia = authorities.iterator();
            while (ia.hasNext()) {
                Authority authority = ia.next();
                authority.getRoles().remove(role);
                HibernateUtil.update(authority);
            }
            role.setAuthorities(null);
            HibernateUtil.delete(role);
            setMsgTitle("角色删除信息");
            setMsgInformation("角色删除成功！");
        }
        return SUCCESS;
    }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.roleauth;

import cn.edu.njust.steduman.database.Role;
import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.List;

/**
 *
 * @author Ghostplant
 */
public class QueryRoleAction extends GlobalActionSupport {

    private String roleId;

    public String getRoleId() {
        return roleId;
    }

    public void setRoleId(String roleId) {
        this.roleId = roleId;
    }

    public List getRoles() {
        return HibernateUtil.getList(Role.class);
    }

    public String queryId() {
        setMsgTitle("角色查询信息");
        if (roleId != null) {
            setMsgInformation("该角色的ID值为：" + roleId);
        } else {
            setMsgInformation("您必须选定一个要查询的角色!");
        }
        return SUCCESS;
    }
}

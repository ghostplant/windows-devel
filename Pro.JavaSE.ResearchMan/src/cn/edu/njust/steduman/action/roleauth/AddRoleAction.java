/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.roleauth;

import cn.edu.njust.steduman.database.Role;
import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.util.HibernateUtil;
import org.hibernate.Query;

/**
 *
 * @author Ghostplant
 */
public class AddRoleAction extends GlobalActionSupport {

    private String roleName;

    public String getRoleName() {
        return roleName;
    }

    public void setRoleName(String roleName) {
        this.roleName = roleName;
    }

    public String submit() {
        Query query = HibernateUtil.getSession().createQuery("from Role where name=:name");
        query.setString("name", roleName);
        if (query.list().size() > 0) {
            setMsgTitle("角色添加信息");
            setMsgInformation("角色名为'" + roleName + "'的角色已经存在，添加失败！");
        } else {
            Role role = new Role();
            role.setName(roleName);
            HibernateUtil.save(role);
            setMsgTitle("角色添加信息");
            setMsgInformation("角色已经成功添加！");
        }
        return SUCCESS;
    }

}

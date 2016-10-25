/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.roleauth;

import cn.edu.njust.steduman.database.Role;
import cn.edu.njust.steduman.database.Authority;
import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

/**
 *
 * @author Ghostplant
 */
public class RemoveAuthorityAction extends GlobalActionSupport {

    private int authorityId;

    public int getAuthorityId() {
        return authorityId;
    }

    public void setAuthorityId(int authorityId) {
        this.authorityId = authorityId;
    }

    public List getAuthorities() {
        return HibernateUtil.getList(Authority.class);
    }

    public String remove() {
        if (authorityId == 0) {
            setMsgTitle("权限删除信息");
            setMsgInformation("请您选中要删除的指定权限名！");
        } else {
            HibernateUtil.close();
            HibernateUtil.init();
            Authority authority=(Authority)HibernateUtil.get(Authority.class, authorityId);
            Set<Role> roles = authority.getRoles();
            Iterator<Role> it = roles.iterator();
            while (it.hasNext()) {
                Role role = it.next();
                role.getAuthorities().remove(authority);
                HibernateUtil.update(role);
            }
            authority.setRoles(null);
            HibernateUtil.delete(authority);
            setMsgTitle("权限删除信息");
            setMsgInformation("权限删除成功！");
        }
        return SUCCESS;
    }
}

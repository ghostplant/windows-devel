/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.roleauth;

import cn.edu.njust.steduman.database.Authority;
import cn.edu.njust.steduman.database.Role;
import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

/**
 *
 * @author Ghostplant
 */
public class AllocAuthorityAction extends GlobalActionSupport {

    private String access[];

    public String[] getAccess() {
        return access;
    }

    public void setAccess(String[] access) {
        this.access = access;
    }
    private int roleId;

    public int getRoleId() {
        return roleId;
    }

    public void setRoleId(int roleId) {
        this.roleId = roleId;
    }

    @Override
    public String init() {
        super.init();
        if (getMenuItem() != null) {
            if (getAuthorities().length == 0) {
                setMsgTitle("角色分配权限信息");
                setMsgInformation("对不起，目前没有任何权限条目，故无法进行该操作！");
                setMenuItem(null);
            } else if (getRoles().isEmpty()) {
                setMsgTitle("角色分配权限信息");
                setMsgInformation("对不起，目前没有任何角色条目，故无法进行该操作！");
                setMenuItem(null);
            }
        }
        return SUCCESS;
    }

    public List getRoles() {
        return HibernateUtil.getList(Role.class);
    }

    public String[] getAuthorities() {
        List<Authority> list = HibernateUtil.getList(Authority.class);
        Iterator<Authority> it = list.iterator();
        String arr[] = new String[list.size()];
        int i = 0;
        while (it.hasNext()) {
            Authority au = it.next();
            arr[i++] = au.getId() + ":" + au.getName();
        }
        return arr;
    }
    private boolean firstStep = true;

    public boolean isFirstStep() {
        return firstStep;
    }

    public void setFirstStep(boolean firstStep) {
        this.firstStep = firstStep;
    }
    private String selectedRoleName;
    private int selectedRoleId;

    public int getSelectedRoleId() {
        return selectedRoleId;
    }

    public void setSelectedRoleId(int selectedRoleId) {
        this.selectedRoleId = selectedRoleId;
    }

    public String getSelectedRoleName() {
        return selectedRoleName;
    }

    public void setSelectedRoleName(String selectedRoleName) {
        this.selectedRoleName = selectedRoleName;
        setTextSelectedRoleName(selectedRoleName);
    }

    public String next() {
        setSelectedRoleId(roleId);
        setSelectedRoleName(((Role) HibernateUtil.get(Role.class, roleId)).getName());
        firstStep = false;

        Role selRole = (Role) HibernateUtil.get(Role.class, roleId);
        Set<Authority> auth = selRole.getAuthorities();
        String[] value = new String[auth.size()];
        Iterator<Authority> it = auth.iterator();
        int i = 0;
        while (it.hasNext()) {
            Authority au = it.next();
            value[i++] = au.getId() + ":" + au.getName();
        }
        setAccess(value);
        return SUCCESS;
    }
    private String textSelectedRoleName;

    public String getTextSelectedRoleName() {
        return textSelectedRoleName;
    }

    public void setTextSelectedRoleName(String textSelectedRoleName) {
        this.textSelectedRoleName = textSelectedRoleName;
    }

    public String update() {
        textSelectedRoleName = selectedRoleName;
        firstStep = false;
        Role role = (Role) HibernateUtil.get(Role.class, getSelectedRoleId());
        if (role == null) {
            return SUCCESS;
        }
        Set authorities = new HashSet();
        for (String part : access) {
            int authorityId = Integer.parseInt(part.substring(0, part.indexOf(":")));
            authorities.add(HibernateUtil.get(Authority.class, authorityId));
        }
        role.setAuthorities(authorities);
        HibernateUtil.update(role);
        setMsgTitle("角色分配权限信息");
        setMsgInformation("角色'" + selectedRoleName + "'的所有权限组已经成功更新到数据库！");
        return SUCCESS;
    }
}

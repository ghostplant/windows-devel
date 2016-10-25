/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.catalist;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Ghostplant
 */
public class RoleauthMan {

    private List pageList = new ArrayList();

    public List getPageList() {
        return pageList;
    }

    public void setPageList(List pageList) {
        this.pageList = pageList;
    }

    public RoleauthMan() {
        pageList.add("新建权限@addauthority");
        pageList.add("查询权限@queryauthority*");
        pageList.add("删除权限@removeauthority");
        pageList.add("新建角色@addrole");
        pageList.add("查询角色@queryrole");
        pageList.add("删除角色@removerole");
        pageList.add("角色分配权限@allocauthority*");
        pageList.add("用户分配角色@allocrole*");
    }
}

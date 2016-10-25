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
public class UserdocMan {

    private List pageList = new ArrayList();

    public List getPageList() {
        return pageList;
    }

    public void setPageList(List pageList) {
        this.pageList = pageList;
    }

    public UserdocMan() {
        pageList.add("新建用户@adduser");
        pageList.add("删除用户@deleteuser");
        pageList.add("冻结用户@freezeuser");
        pageList.add("解冻用户@unfreezeuser");
        pageList.add("批量导入用户@uploaduser*");
        //pageList.add("导出用户@exportuser");
        pageList.add("修改用户信息@edituser");
        pageList.add("登录密码修改@changepassword");
    }
}

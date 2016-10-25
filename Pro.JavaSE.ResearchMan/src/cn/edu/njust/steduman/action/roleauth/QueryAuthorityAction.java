/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.roleauth;

import cn.edu.njust.steduman.database.Authority;
import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Ghostplant
 */
public class QueryAuthorityAction extends GlobalActionSupport {

    private String authorityId;
    private int page;

    public int getPage() {
        return page;
    }

    public void setPage(int page) {
        this.page = page;
    }

    public String getAuthorityId() {
        return authorityId;
    }

    public void setAuthorityId(String authorityId) {
        this.authorityId = authorityId;
    }

    public String currentPage() {
        List list = HibernateUtil.getList(Authority.class);
        int count = (list.size() + 14) / 15;
        if (page > count || page < 0) {
            page = 1;
            return "first";
        }
        if (page == 1) {
            return "first";
        } else if (page == count) {
            return "last";
        }
        return "";
    }

    public String next() {
        page++;
        return SUCCESS;
    }

    public String prev() {
        page--;
        return SUCCESS;
    }

    @Override
    public String init() {
        super.init();
        page = 1;
        return SUCCESS;
    }

    public List getAuthorities() {
        List list = HibernateUtil.getList(Authority.class), build = new ArrayList();
        int count = (list.size() + 14) / 15;
        if (page > count || page < 0) {
            page = 1;
        }
        Iterator it = list.iterator();
        for (int i = 1; i < page; i++) {
            for (int j = 0; j < 15; j++) {
                it.next();
            }
        }
        while (it.hasNext() && build.size() < 15) {
            build.add(it.next());
        }
        return build;
    }

    public String queryId() {
        setMsgTitle("权限查询信息");
        if (authorityId != null) {
            setMsgInformation("该权限的ID值为：" + authorityId + "; 权限映射模块：" + ((Authority) HibernateUtil.get(Authority.class, Integer.parseInt(authorityId))).getFunction());
        } else {
            setMsgInformation("您必须选定一个要查询的权限！");
        }
        return SUCCESS;
    }
}

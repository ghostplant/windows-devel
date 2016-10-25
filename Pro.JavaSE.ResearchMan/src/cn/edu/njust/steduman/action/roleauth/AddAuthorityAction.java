/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.roleauth;

import cn.edu.njust.steduman.database.Authority;
import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.util.HibernateUtil;
import org.hibernate.Query;

/**
 *
 * @author Ghostplant
 */
public class AddAuthorityAction extends GlobalActionSupport {

    private String authoriyName, authoriyFunction;

    public String getAuthoriyFunction() {
        return authoriyFunction;
    }

    public void setAuthoriyFunction(String authoriyFunction) {
        this.authoriyFunction = authoriyFunction;
    }

    public String getAuthoriyName() {
        return authoriyName;
    }

    public void setAuthoriyName(String authoriyName) {
        this.authoriyName = authoriyName;
    }

    public String submit() {
        Query query = HibernateUtil.getSession().createQuery("from Authority where name=:name");
        query.setString("name", authoriyName);
        if (query.list().size() > 0) {
            setMsgTitle("权限添加信息");
            setMsgInformation("权限名为'" + authoriyName + "'的权限已经存在，添加失败！");
        } else if (authoriyFunction.indexOf("#") < 0) {
            setMsgTitle("权限添加信息");
            setMsgInformation("权限映射模块名称必须含有分隔符'#'，添加失败！");
        } else {
            Authority authority = new Authority();
            authority.setName(authoriyName);
            authority.setFunction(authoriyFunction);
            HibernateUtil.save(authority);
            setMsgTitle("权限添加信息");
            setMsgInformation("权限已经成功添加！");
        }
        return SUCCESS;
    }
}

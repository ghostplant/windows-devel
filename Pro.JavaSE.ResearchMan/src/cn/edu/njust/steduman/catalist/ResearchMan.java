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
public class ResearchMan {

    private List pageList = new ArrayList();

    public List getPageList() {
        return pageList;
    }

    public void setPageList(List pageList) {
        this.pageList = pageList;
    }

    public ResearchMan() {
        pageList.add("添加项目@addproject");
        pageList.add("更改项目@changeproject");
        pageList.add("查询项目@queryproject*");
        pageList.add("人员结构与学术骨干@researcher*");
    }
}

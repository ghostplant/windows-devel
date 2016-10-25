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
public class TalentsMan {

    private List pageList = new ArrayList();

    public List getPageList() {
        return pageList;
    }

    public void setPageList(List pageList) {
        this.pageList = pageList;
    }

    public TalentsMan() {
        pageList.add("显示学生信息@showstuinfo");
        pageList.add("查询学生信息@querystuinfo");
        pageList.add("更改学生信息@changestuinfo");
        pageList.add("下载@download*");
    }
}

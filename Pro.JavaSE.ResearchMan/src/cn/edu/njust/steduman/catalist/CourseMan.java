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
public class CourseMan {

    private List pageList = new ArrayList();

    public List getPageList() {
        return pageList;
    }

    public void setPageList(List pageList) {
        this.pageList = pageList;
    }

    public CourseMan() {
        pageList.add("添加课程@addcourse");
        pageList.add("变更课程@changecourse");
        pageList.add("查询课程@querycourse*");
    }
}

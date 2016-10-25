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
public class AcademicMan {
    private List pageList=new ArrayList();

    public List getPageList() {
        return pageList;
    }

    public void setPageList(List pageList) {
        this.pageList = pageList;
    }

    public AcademicMan() {
        pageList.add("主要国际国内学术会议@insertacademicmeeting");
        pageList.add("学术交流活动申请@appacademicactivity");
        pageList.add("国际交流合作项目@internationcooperation");
        pageList.add("工作条件管理@workingcondition*");
        pageList.add("图书资料情况管理@libraryorder");
    }
}

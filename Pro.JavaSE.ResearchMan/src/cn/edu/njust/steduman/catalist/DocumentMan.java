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
public class DocumentMan {

    private List pageList = new ArrayList();

    public List getPageList() {
        return pageList;
    }

    public void setPageList(List pageList) {
        this.pageList = pageList;
    }

    public DocumentMan() {
        pageList.add("文档添加删除与下载@documentsmanager*");
    }
}

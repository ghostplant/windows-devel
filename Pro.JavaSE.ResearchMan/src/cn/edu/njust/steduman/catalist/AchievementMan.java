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
public class AchievementMan {

    private List pageList = new ArrayList();

    public List getPageList() {
        return pageList;
    }

    public void setPageList(List pageList) {
        this.pageList = pageList;
    }

    public AchievementMan() {
        pageList.add("教学成果录入@teachingachievement");
        pageList.add("科研成果录入@researchachievement");
        pageList.add("论文专著录入@essayachievement*");
        pageList.add("著作专利录入@patentachievement");
        pageList.add("优秀人才培养@cultivationachievement");
        pageList.add("成果查询@achievementquery");
        pageList.add("成果导出@exportachievement");

    }
}

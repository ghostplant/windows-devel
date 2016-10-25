/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.academic;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Cooperation;
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.InputStream;
import java.util.Iterator;
import java.util.List;
import org.hibernate.Hibernate;

/**
 *
 * @author Ghostplant
 */
public class InternationCooperationAction extends GlobalActionSupport {

    private String name, target;
    private int selectedId;

    public int getSelectedId() {
        return selectedId;
    }

    public void setSelectedId(int selectedId) {
        this.selectedId = selectedId;
    }

    public List getRecordList() {
        return HibernateUtil.getList(Cooperation.class);
    }

    public int getEnd() {
        return end;
    }

    public void setEnd(int end) {
        this.end = end;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getStart() {
        return start;
    }

    public void setStart(int start) {
        this.start = start;
    }

    public String getTarget() {
        return target;
    }

    public void setTarget(String target) {
        this.target = target;
    }
    private int start, end;

    public String insert() {
        if (name.length() == 0 || target.length() == 0) {
            setMsgTitle("添加项目信息");
            setMsgInformation("请您填写完整的项目信息！");
        } else if (start < 0 || end < 0) {
            setMsgTitle("添加项目信息");
            setMsgInformation("请您填写正确的项目起止年份或0！");
        } else {
            Cooperation cooperation = new Cooperation();
            cooperation.setName(name);
            cooperation.setStartYear(start);
            cooperation.setEndYear(end);
            cooperation.setTarget(target);
            HibernateUtil.save(cooperation);
        }
        return SUCCESS;
    }

    public String delete() {
        Cooperation cooperation = (Cooperation) HibernateUtil.get(Cooperation.class, selectedId);
        HibernateUtil.delete(cooperation);
        return SUCCESS;
    }

    public String export() {
        WebdiskUtil.clearTempFile();
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("cooperation.xls"));
            Iterator<Cooperation> it = getRecordList().iterator();
            boolean first = true;
            while (it.hasNext()) {
                int lineNumber;
                if (first) {
                    first = false;
                    lineNumber = 2;
                } else {
                    lineNumber = workbook.createNewLineWithBorder(0);
                }
                Cooperation cooperation = it.next();
                workbook.setCenterCellValue(0, lineNumber, 0, cooperation.getName());
                workbook.setCenterCellValue(0, lineNumber, 2, cooperation.getTarget());
                String period = cooperation.getStartYear() != 0 ? String.valueOf(cooperation.getStartYear()) : "至今";
                period += "-" + (cooperation.getEndYear() != 0 ? String.valueOf(cooperation.getEndYear()) : "至今");
                workbook.setCenterCellValue(0, lineNumber, 1, period);
            }
            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("cooperation.xls"));
            workbook.close();
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return INPUT;
    }

    public InputStream getInputStream() throws Exception {
        return WebdiskUtil.getSaveDocumentStream("cooperation.xls");
    }
}

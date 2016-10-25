/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.academic;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Meeting;
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Ghostplant
 */
public class InsertAcademicMeetingAction extends GlobalActionSupport {

    private String meetingName;
    private java.util.Date startDate, endDate;
    private int totalCount, foreignCount;

    public String castToString(Date date) {
        SimpleDateFormat formater = new SimpleDateFormat("yyyy-MM-dd");
        return formater.format(date);
    }

    public Date getEndDate() {
        return endDate;
    }

    public void setEndDate(Date endDate) {
        this.endDate = endDate;
    }

    public int getForeignCount() {
        return foreignCount;
    }

    public void setForeignCount(int foreignCount) {
        this.foreignCount = foreignCount;
    }

    public String getMeetingName() {
        return meetingName;
    }

    public void setMeetingName(String meetingName) {
        this.meetingName = meetingName;
    }

    public Date getStartDate() {
        return startDate;
    }

    public void setStartDate(Date startDate) {
        this.startDate = startDate;
    }

    public int getTotalCount() {
        return totalCount;
    }

    public void setTotalCount(int totalCount) {
        this.totalCount = totalCount;
    }

    public List getRecordList() {
        return HibernateUtil.getList(Meeting.class);
    }

    public String insert() {
        if (startDate == null || endDate == null || meetingName.length() == 0) {
            setMsgTitle("添加项目信息");
            setMsgInformation("请您填写正确的日期格式与完整的项目信息！");
        } else {
            Meeting meeting = new Meeting();
            meeting.setName(meetingName);
            meeting.setTotalPeople(totalCount);
            meeting.setForeignPeople(foreignCount);
            meeting.setHoldStartDate(startDate);
            meeting.setHoldEndDate(endDate);
            HibernateUtil.save(meeting);
            meetingName = "";
            totalCount = foreignCount = 0;
            startDate = endDate = null;
        }
        return SUCCESS;
    }
    private int selectedId;

    public int getSelectedId() {
        return selectedId;
    }

    public void setSelectedId(int selectedId) {
        this.selectedId = selectedId;
    }

    public String delete() {
        Meeting meeting = (Meeting) HibernateUtil.get(Meeting.class, selectedId);
        HibernateUtil.delete(meeting);
        return SUCCESS;
    }

    public String export() {
        WebdiskUtil.clearTempFile();
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("meeting.xls"));
            Iterator<Meeting> it = getRecordList().iterator();
            boolean first = true;
            while (it.hasNext()) {
                int lineNumber;
                if (first) {
                    first = false;
                    lineNumber = 3;
                } else {
                    lineNumber = workbook.createNewLineWithBorder(0);
                }
                Meeting meeting = it.next();
                workbook.setCenterCellValue(0, lineNumber, 0, meeting.getName());
                workbook.setCenterCellValue(0, lineNumber, 1, castToString(meeting.getHoldStartDate()) + "至" + castToString(meeting.getHoldEndDate()));
                workbook.setCenterCellValue(0, lineNumber, 2, String.valueOf(meeting.getTotalPeople()));
                workbook.setCenterCellValue(0, lineNumber, 3, String.valueOf(meeting.getForeignPeople()));
            }
            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("meeting.xls"));
            workbook.close();
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception {
        return WebdiskUtil.getSaveDocumentStream("meeting.xls");
    }
}

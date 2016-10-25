/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.academic;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Essay;
import cn.edu.njust.steduman.database.Meeting;
import cn.edu.njust.steduman.database.MeetingReport;
import cn.edu.njust.steduman.database.Person;
import cn.edu.njust.steduman.database.Teacher;
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
public class AppAcademicActivityAction extends GlobalActionSupport {
    
    private String teacherId;
    private int essayId, destinationId;
    
    public int getDestinationId() {
        return destinationId;
    }
    
    public void setDestinationId(int destinationId) {
        this.destinationId = destinationId;
    }
    
    public int getEssayId() {
        return essayId;
    }
    
    public void setEssayId(int essayId) {
        this.essayId = essayId;
    }
    
    public String getTeacherId() {
        return teacherId;
    }
    
    public void setTeacherId(String teacherId) {
        this.teacherId = teacherId;
    }
    private java.util.Date reportDate;
    
    public Date getReportDate() {
        return reportDate;
    }
    
    public void setReportDate(Date reportDate) {
        this.reportDate = reportDate;
    }
    
    public List getTeacherList() {
        return HibernateUtil.getList(Teacher.class);
    }
    
    public List getMeetingList() {
        return HibernateUtil.getList(Meeting.class);
    }
    
    public List getEssayList() {
        return HibernateUtil.getList(Essay.class);
    }
    
    public List getRecordList() {
        return HibernateUtil.getList(MeetingReport.class);
    }
    
    public String getEssayName(int essayId) {
        return ((Essay) HibernateUtil.get(Essay.class, essayId)).getTitle();
    }
    
    public String getTeacherName(String teacherId) {
        return ((Person) HibernateUtil.get(Person.class, teacherId)).getName();
    }
    
    public String getMeetingName(int meetingId) {
        return ((Meeting) HibernateUtil.get(Meeting.class, meetingId)).getName();
    }
    
    public String castToString(Date date) {
        SimpleDateFormat formater = new SimpleDateFormat("yyyy-MM-dd");
        return formater.format(date);
    }
    
    public String insert() {
        if (teacherId == null || essayId == 0 || destinationId == 0 || reportDate == null) {
            setMsgTitle("添加记录信息");
            setMsgInformation("请您选择正确的日期格式，以及填写完整的项目信息！");
        } else {
            MeetingReport mr = new MeetingReport();
            mr.setEssayId(essayId);
            mr.setMeetingId(destinationId);
            mr.setReportDate(reportDate);
            mr.setTeacherId(teacherId);
            HibernateUtil.save(mr);
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
        MeetingReport mr = (MeetingReport) HibernateUtil.get(MeetingReport.class, selectedId);
        HibernateUtil.delete(mr);
        return SUCCESS;
    }
    
    public String export() {
        WebdiskUtil.clearTempFile();
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("report.xls"));
            Iterator<MeetingReport> it = getRecordList().iterator();
            boolean first = true;
            while (it.hasNext()) {
                int lineNumber;
                if (first) {
                    first = false;
                    lineNumber = 3;
                } else {
                    lineNumber = workbook.createNewLineWithBorder(0);
                }
                MeetingReport meetingReport = it.next();
                workbook.setCenterCellValue(0, lineNumber, 0, ((Teacher) HibernateUtil.get(Teacher.class, meetingReport.getTeacherId())).getName());
                workbook.setCenterCellValue(0, lineNumber, 1, ((Meeting) HibernateUtil.get(Meeting.class, meetingReport.getMeetingId())).getName());
                workbook.setCenterCellValue(0, lineNumber, 2, castToString(meetingReport.getReportDate()));
                workbook.setCenterCellValue(0, lineNumber, 3, ((Essay) HibernateUtil.get(Essay.class, meetingReport.getEssayId())).getTitle());
            }
            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("report.xls"));
            workbook.close();
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return SUCCESS;
    }
    
    public InputStream getInputStream() throws Exception {
        return WebdiskUtil.getSaveDocumentStream("report.xls");
    }
}

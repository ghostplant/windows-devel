/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.talents;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.InputStream;
import java.util.Calendar;
import java.util.Iterator;
import java.util.List;
import org.hibernate.Query;

/**
 *
 * @author Administrator
 */
public class DownloadAction extends GlobalActionSupport {

    private int totalenrollmaster, totalgraduatemaster, totalenrolldoctor, totalgraduatedoctor;
    private int enfivemas, enforemas, enthreemas, entwomas, enonemas;
    private int enfivedoc, enforedoc, enthreedoc, entwodoc, enonedoc;
    private int grafivemas, graforemas, grathreemas, gratwomas, graonemas;
    private int grafivedoc, graforedoc, grathreedoc, gratwodoc, graonedoc;
    Calendar ca = Calendar.getInstance();
    private int year = ca.get(Calendar.YEAR);//获取年份

    public String getAmount1(int Year) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as student where entranceYear=:Year and degree='硕士'");
        query.setString("Year", String.valueOf(Year));

        String anString = query.uniqueResult().toString();
        totalenrollmaster += Integer.valueOf(anString);
        return anString;

    }

    public String getAmount2(int Year) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as student where graduationYear=:Year and degree='硕士'");
        query.setString("Year", String.valueOf(Year));

        String anString = query.uniqueResult().toString();
        totalgraduatemaster += Integer.valueOf(anString);
        return anString;

    }

    public String getAmount3(int Year) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as student where entranceYear=:Year and degree='博士'");
        query.setString("Year", String.valueOf(Year));

        String anString = query.uniqueResult().toString();
        totalenrolldoctor += Integer.valueOf(anString);
        return anString;

    }

    public String getAmount4(int Year) {
        Query query = HibernateUtil.getSession().createQuery("select count(*) from Student as student where graduationYear=:Year and degree='博士'");
        query.setString("Year", String.valueOf(Year));

        String anString = query.uniqueResult().toString();
        totalgraduatedoctor += Integer.valueOf(anString);
        return anString;

    }

    public int getYear() {
        return year;
    }

    public void setYear(int year) {
        this.year = year;
    }

    public int getEnfivedoc() {
        return enfivedoc;
    }

    public void setEnfivedoc(int enfivedoc) {
        this.enfivedoc = enfivedoc;
    }

    public int getEnfivemas() {
        return enfivemas;
    }

    public void setEnfivemas(int enfivemas) {
        this.enfivemas = enfivemas;
    }

    public int getEnforedoc() {
        return enforedoc;
    }

    public void setEnforedoc(int enforedoc) {
        this.enforedoc = enforedoc;
    }

    public int getEnforemas() {
        return enforemas;
    }

    public void setEnforemas(int enforemas) {
        this.enforemas = enforemas;
    }

    public int getEnonedoc() {
        return enonedoc;
    }

    public void setEnonedoc(int enonedoc) {
        this.enonedoc = enonedoc;
    }

    public int getEnonemas() {
        return enonemas;
    }

    public void setEnonemas(int enonemas) {
        this.enonemas = enonemas;
    }

    public int getEnthreedoc() {
        return enthreedoc;
    }

    public void setEnthreedoc(int enthreedoc) {
        this.enthreedoc = enthreedoc;
    }

    public int getEnthreemas() {
        return enthreemas;
    }

    public void setEnthreemas(int enthreemas) {
        this.enthreemas = enthreemas;
    }

    public int getEntwodoc() {
        return entwodoc;
    }

    public void setEntwodoc(int entwodoc) {
        this.entwodoc = entwodoc;
    }

    public int getEntwomas() {
        return entwomas;
    }

    public void setEntwomas(int entwomas) {
        this.entwomas = entwomas;
    }

    public int getGrafivedoc() {
        return grafivedoc;
    }

    public void setGrafivedoc(int grafivedoc) {
        this.grafivedoc = grafivedoc;
    }

    public int getGrafivemas() {
        return grafivemas;
    }

    public void setGrafivemas(int grafivemas) {
        this.grafivemas = grafivemas;
    }

    public int getGraforedoc() {
        return graforedoc;
    }

    public void setGraforedoc(int graforedoc) {
        this.graforedoc = graforedoc;
    }

    public int getGraforemas() {
        return graforemas;
    }

    public void setGraforemas(int graforemas) {
        this.graforemas = graforemas;
    }

    public int getGraonedoc() {
        return graonedoc;
    }

    public void setGraonedoc(int graonedoc) {
        this.graonedoc = graonedoc;
    }

    public int getGraonemas() {
        return graonemas;
    }

    public void setGraonemas(int graonemas) {
        this.graonemas = graonemas;
    }

    public int getGrathreedoc() {
        return grathreedoc;
    }

    public void setGrathreedoc(int grathreedoc) {
        this.grathreedoc = grathreedoc;
    }

    public int getGrathreemas() {
        return grathreemas;
    }

    public void setGrathreemas(int grathreemas) {
        this.grathreemas = grathreemas;
    }

    public int getGratwodoc() {
        return gratwodoc;
    }

    public void setGratwodoc(int gratwodoc) {
        this.gratwodoc = gratwodoc;
    }

    public int getGratwomas() {
        return gratwomas;
    }

    public void setGratwomas(int gratwomas) {
        this.gratwomas = gratwomas;
    }

    public int getTotalenrolldoctor() {
        return totalenrolldoctor;
    }

    public void setTotalenrolldoctor(int totalenrolldoctor) {
        this.totalenrolldoctor = totalenrolldoctor;
    }

    public int getTotalenrollmaster() {
        return totalenrollmaster;
    }

    public void setTotalenrollmaster(int totalenrollmaster) {
        this.totalenrollmaster = totalenrollmaster;
    }

    public int getTotalgraduatedoctor() {
        return totalgraduatedoctor;
    }

    public void setTotalgraduatedoctor(int totalgraduatedoctor) {
        this.totalgraduatedoctor = totalgraduatedoctor;
    }

    public int getTotalgraduatemaster() {
        return totalgraduatemaster;
    }

    public void setTotalgraduatemaster(int totalgraduatemaster) {
        this.totalgraduatemaster = totalgraduatemaster;
    }

    public String export() {
        WebdiskUtil.clearTempFile();  // 第一步，清理
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("enrollandgraduate.xls")); // 第二步，载入
            workbook.setCenterCellValue(0, 1, 2, String.valueOf(year - 4));
            workbook.setCenterCellValue(0, 1, 3, String.valueOf(year - 3));
            workbook.setCenterCellValue(0, 1, 4, String.valueOf(year - 2));
            workbook.setCenterCellValue(0, 1, 5, String.valueOf(year - 1));
            workbook.setCenterCellValue(0, 1, 6, String.valueOf(year));


            workbook.setCenterCellValue(0, 2, 2, String.valueOf(getAmount1(year - 4)));
            workbook.setCenterCellValue(0, 2, 3, String.valueOf(getAmount1(year - 3)));
            workbook.setCenterCellValue(0, 2, 4, String.valueOf(getAmount1(year - 2)));
            workbook.setCenterCellValue(0, 2, 5, String.valueOf(getAmount1(year - 1)));
            workbook.setCenterCellValue(0, 2, 6, String.valueOf(getAmount1(year)));
            workbook.setCenterCellValue(0, 2, 7, String.valueOf(totalenrollmaster));


            workbook.setCenterCellValue(0, 3, 2, String.valueOf(getAmount2(year - 4)));
            workbook.setCenterCellValue(0, 3, 3, String.valueOf(getAmount2(year - 3)));
            workbook.setCenterCellValue(0, 3, 4, String.valueOf(getAmount2(year - 2)));
            workbook.setCenterCellValue(0, 3, 5, String.valueOf(getAmount2(year - 1)));
            workbook.setCenterCellValue(0, 3, 6, String.valueOf(getAmount2(year)));
            workbook.setCenterCellValue(0, 3, 7, String.valueOf(totalgraduatemaster));


            workbook.setCenterCellValue(0, 4, 2, String.valueOf(getAmount3(year - 4)));
            workbook.setCenterCellValue(0, 4, 3, String.valueOf(getAmount3(year - 3)));
            workbook.setCenterCellValue(0, 4, 4, String.valueOf(getAmount3(year - 2)));
            workbook.setCenterCellValue(0, 4, 5, String.valueOf(getAmount3(year - 1)));
            workbook.setCenterCellValue(0, 4, 6, String.valueOf(getAmount3(year)));
            workbook.setCenterCellValue(0, 4, 7, String.valueOf(totalenrolldoctor));


            workbook.setCenterCellValue(0, 5, 2, String.valueOf(getAmount4(year - 4)));
            workbook.setCenterCellValue(0, 5, 3, String.valueOf(getAmount4(year - 3)));
            workbook.setCenterCellValue(0, 5, 4, String.valueOf(getAmount4(year - 2)));
            workbook.setCenterCellValue(0, 5, 5, String.valueOf(getAmount4(year - 1)));
            workbook.setCenterCellValue(0, 5, 6, String.valueOf(getAmount4(year)));
            workbook.setCenterCellValue(0, 5, 7, String.valueOf(totalgraduatedoctor));

            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("enrollandgraduate.xls")); // 第三步，保存
            workbook.close(); // 第四步，释放资源与返回INPUT
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception { // 第五步，定义下载入口流；第六步修改struts2配置文件
        return WebdiskUtil.getSaveDocumentStream("enrollandgraduate.xls");
    }
}

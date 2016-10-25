/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.academic;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Device;
import cn.edu.njust.steduman.database.Laboratory;
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.InputStream;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import org.hibernate.Query;

/**
 *
 * @author Ghostplant
 */
public class WorkingConditionAction extends GlobalActionSupport {

    private String labName, deviceList, item;

    public String clearItem() {
        deviceList = "";
        return SUCCESS;
    }

    public List getRecordList() {
        return HibernateUtil.getList(Laboratory.class);
    }

    public Set getDeviceSet(int laboratoryId) {
        return ((Laboratory) HibernateUtil.get(Laboratory.class, laboratoryId)).getDevices();
    }

    public String addItem() {
        if (item.length() == 0) {
            setMsgTitle("添加设备信息");
            setMsgInformation("设备名称不能为空！");
        } else {
            deviceList += item + "~";
            item = "";
        }
        return SUCCESS;
    }

    public String getItem() { //一条记录
        return item;
    }

    public void setItem(String item) {
        this.item = item;
    }

    public String getDeviceList() {  // 保存hidden
        return deviceList;
    }

    public void setDeviceList(String deviceList) {
        this.deviceList = deviceList;
    }
    private int totPersonnel, totDevice;

    public String getLabName() {
        return labName;
    }

    public void setLabName(String labName) {
        this.labName = labName;
    }

    public float getTotCost() {
        return totCost;
    }

    public void setTotCost(float totCost) {
        this.totCost = totCost;
    }

    public int getTotDevice() {
        return totDevice;
    }

    public void setTotDevice(int totDevice) {
        this.totDevice = totDevice;
    }

    public int getTotPersonnel() {
        return totPersonnel;
    }

    public void setTotPersonnel(int totPersonnel) {
        this.totPersonnel = totPersonnel;
    }
    private float totCost;
    private int selectedId;

    public int getSelectedId() {
        return selectedId;
    }

    public void setSelectedId(int selectedId) {
        this.selectedId = selectedId;
    }

    public String getSumPersonnel() {
        Query query = HibernateUtil.getSession().createQuery("select sum(lab.personnelCount) from Laboratory as lab");
        Object obj = query.uniqueResult();
        return (obj != null) ? query.uniqueResult().toString() : "----";
    }

    public String getSumDevice() {
        Query query = HibernateUtil.getSession().createQuery("select sum(lab.deviceCount) from Laboratory as lab");
        Object obj = query.uniqueResult();
        return (obj != null) ? query.uniqueResult().toString() : "----";
    }

    public String getSumCost() {
        Query query = HibernateUtil.getSession().createQuery("select sum(lab.deviceCost) from Laboratory as lab");
        Object obj = query.uniqueResult();
        return (obj != null) ? query.uniqueResult().toString() : "----";
    }

    public String insert() {
        if (deviceList.length() == 0 || labName.length() == 0) {
            setMsgTitle("添加项目信息");
            setMsgInformation("您必须填写实验室名称，而且至少一种设备！");
        } else {
            String[] devices = deviceList.split("~");
            Laboratory laboratory = new Laboratory();
            laboratory.setName(labName);
            laboratory.setDeviceCost(totCost);
            laboratory.setPersonnelCount(totPersonnel);
            laboratory.setDeviceCount(totDevice);
            for (String idevice : devices) {
                Device device = new Device();
                device.setName(idevice);
                laboratory.getDevices().add(device);
            }
            HibernateUtil.save(laboratory);

            totCost = totPersonnel = totDevice = 0;
            labName = "";
            clearItem();
        }
        return SUCCESS;
    }

    public String delete() {
        Laboratory laboratory = (Laboratory) HibernateUtil.get(Laboratory.class, selectedId);
        HibernateUtil.delete(laboratory);
        return SUCCESS;
    }

    public String export() {
        WebdiskUtil.clearTempFile();
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("workingcondition.xls"));
            Iterator<Laboratory> it = getRecordList().iterator();
            boolean first = true;
            while (it.hasNext()) {
                int lineNumber;
                if (first) {
                    first = false;
                    lineNumber = 5;
                } else {
                    lineNumber = workbook.createNewLineWithBorder(0);
                }
                Laboratory laboratory = it.next();
                workbook.setCenterCellValue(0, lineNumber, 0, laboratory.getName());
                workbook.setCenterCellValue(0, lineNumber, 2, String.valueOf(laboratory.getPersonnelCount()));
                workbook.setCenterCellValue(0, lineNumber, 3, String.valueOf(laboratory.getDeviceCount()));
                workbook.setCenterCellValue(0, lineNumber, 4, String.valueOf(laboratory.getDeviceCost()));
                Iterator<Device> itdev = laboratory.getDevices().iterator();
                workbook.setCenterCellValue(0, lineNumber, 1, itdev.next().getName());
                while (itdev.hasNext()) {
                    int nLine = workbook.createNewLineWithoutBorder(0);
                    workbook.setCenterCellValue(0, nLine, 1, itdev.next().getName());
                }
            }
            workbook.setCenterCellValue(0, 4, 2, getSumPersonnel());
            workbook.setCenterCellValue(0, 4, 3, getSumDevice());
            workbook.setCenterCellValue(0, 4, 4, getSumCost());
            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("workingcondition.xls"));
            workbook.close();
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return INPUT;
    }

    public InputStream getInputStream() throws Exception {
        return WebdiskUtil.getSaveDocumentStream("workingcondition.xls");
    }
}

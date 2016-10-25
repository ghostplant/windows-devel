/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.document;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import org.apache.commons.io.FileUtils;
import org.apache.struts2.ServletActionContext;

/**
 *
 * @author Ghostplant
 */
public class DocumentsManagerAction extends GlobalActionSupport {

    private String selectedFile;

    public String getSelectedFile() {
        return selectedFile;
    }

    public void setSelectedFile(String selectedFile) {
        this.selectedFile = selectedFile;
    }

    public File[] getRecordList() {
        String url = WebdiskUtil.getCurrentUserWebdisk();
        File file = new File(WebdiskUtil.getApplicationPath(url));
        if (!file.exists()) {
            file.mkdir();
        }
        return file.listFiles();
    }

    public String delete() {
        String url = WebdiskUtil.getCurrentUserWebdisk();
        new File(WebdiskUtil.getApplicationPath(url) + "/" + selectedFile).delete();
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception {
        return ServletActionContext.getServletContext().getResourceAsStream(WebdiskUtil.getCurrentUserWebdisk() + "/" + selectedFile);
    }

    public String encode(String str) {
        String build = "";
        for (int i = 0; i < str.length(); i++) {
            build += Integer.toHexString(str.charAt(i)) + "-";
        }
        return build;
    }

    public String decode(String str) {
        String[] parts = str.split("-");
        String build = "";
        for (int i = 0; i < parts.length; i++) {
            build += (char) Integer.parseInt(parts[i], 16);
        }
        return build;
    }

    public String getContentDisposition() throws UnsupportedEncodingException {
        return "attachment;filename=" + java.net.URLEncoder.encode(decode(selectedFile), "UTF-8");
    }

    public String download() {
        return INPUT;
    }
    private File uploadFile;

    public File getUploadFile() {
        return uploadFile;
    }

    public void setUploadFile(File uploadFile) {
        this.uploadFile = uploadFile;
    }
    private String uploadFileFileName;

    public String getUploadFileFileName() {
        return uploadFileFileName;
    }

    public void setUploadFileFileName(String uploadFileFileName) {
        this.uploadFileFileName = uploadFileFileName;
    }

    public String upload() throws IOException {
        if (uploadFile == null) {
            setMsgTitle("上传文件信息");
            setMsgInformation("上传文件找不到或拒绝访问！");
        } else if (new FileInputStream(uploadFile).available() > 3 * (2 << 20)) {
            setMsgTitle("上传文件信息");
            setMsgInformation("单文件上传文件大小不能超过3M！");
        } else {
            String url = WebdiskUtil.getCurrentUserWebdisk();
            try {
                File file = new File(WebdiskUtil.getApplicationPath(url + "/" + encode(uploadFileFileName)));
                if (file.exists()) {
                    setMsgTitle("上传文件信息");
                    setMsgInformation("文件名为'" + uploadFileFileName + "'的文件已存在，不能上传！");
                }
                FileUtils.copyFile(uploadFile, file);
            } catch (IOException ex) {
                setMsgTitle("上传文件信息");
                setMsgInformation("上传文件异常中断！");
            }
        }
        return SUCCESS;
    }
}

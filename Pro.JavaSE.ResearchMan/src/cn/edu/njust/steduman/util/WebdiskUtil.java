/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.util;

import cn.edu.njust.steduman.database.Teacher;
import java.io.File;
import java.io.InputStream;
import java.util.Random;
import org.apache.struts2.ServletActionContext;

/**
 *
 * @author Ghostplant
 */
public class WebdiskUtil {

    public static void clearTempFile() {
        String localPath = getTemporaryPath();
        File dirPath = new File(localPath);
        String[] files = dirPath.list();
        for (String file : files) {
            new File(localPath + "/" + file).delete();
        }
    }

    public static String getRootPath() {
        return ServletActionContext.getServletContext().getRealPath("/");
    }

    public static String getApplicationPath(String pathBase) {
        return ServletActionContext.getServletContext().getRealPath(pathBase);
    }

    public static String getTemporaryPath() {
        return ServletActionContext.getServletContext().getRealPath("/webdisk/common");
    }

    public static String getTemplateFile(String excelFile) {
        return getApplicationPath("/webdisk/template/" + excelFile);
    }

    public static String getSaveDocumentFile(String excelFile) {
        return getApplicationPath("/webdisk/common/" + excelFile);
    }

    public static String getCurrentUserWebdisk() {
        return "/webdisk/user@" + ((Teacher) SessionUtil.getSession("user")).getId();
    }

    public static InputStream getSaveDocumentStream(String excelFile) {
        return ServletActionContext.getServletContext().getResourceAsStream("/webdisk/common/" + excelFile);
    }
}

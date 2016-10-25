/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.util;

/**
 *
 * @author Ghostplant
 */
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;

import org.apache.tools.zip.ZipEntry;
import org.apache.tools.zip.ZipFile;

public class ZIPUtil {

    static void createDir(String path) {
        File dir = new File(path);
        if (dir.exists() == false) {
            dir.mkdir();
        }
    }

    static String getSuffixName(String name) {
        return name.substring(0, name.lastIndexOf("."));
    }

    public static void unzip(File file, File unzipFileDir)
            throws IOException {
        // 创建文件对象
        // 创建zip文件对象
        ZipFile zipFile = new ZipFile(file);
        // 创建本zip文件解压目录
        if (unzipFileDir.exists()) {
            unzipFileDir.delete();
        }
        unzipFileDir.mkdir();
        // 得到zip文件条目枚举对象
        Enumeration zipEnum = zipFile.getEntries();
        // 定义输入输出流对象
        InputStream input = null;
        OutputStream output = null;
        // 定义对象
        ZipEntry entry = null;
        // 循环读取条目
        while (zipEnum.hasMoreElements()) {
            // 得到当前条目
            entry = (ZipEntry) zipEnum.nextElement();
            String entryName = entry.getName();
            // 用/分隔条目名称
            String names[] = entryName.split("\\/");
            int length = names.length;
            String path = unzipFileDir.getAbsolutePath();
            for (int v = 0; v < length; v++) {
                if (v < length - 1) { // 最后一个目录之前的目录
                    path += "/" + names[v] + "/";
                    createDir(path);
                } else { // 最后一个
                    if (entryName.endsWith("/")) // 为目录,则创建文件夹
                    {
                        createDir(unzipFileDir.getAbsolutePath() + "/" + entryName);
                    } else { // 为文件,则输出到文件
                        input = zipFile.getInputStream(entry);
                        output = new FileOutputStream(new File(unzipFileDir.getAbsolutePath()
                                + "/" + entryName));
                        byte[] buffer = new byte[1024 * 8];
                        int readLen = 0;
                        while ((readLen = input.read(buffer, 0, 1024 * 8)) != -1) {
                            output.write(buffer, 0, readLen);
                        }
                        // 关闭流
                        input.close();
                        output.flush();
                        output.close();
                    }
                }
            }
        }
        zipFile.close();
    }
}

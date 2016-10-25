/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.util;

import java.io.*;
import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFCellStyle;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.usermodel.Font;

/**
 *
 * @author Ghostplant
 */
public class ExcelUtil {

    private HSSFWorkbook workbook;
    private FileInputStream fis;
    private POIFSFileSystem pfs;

    public ExcelUtil(String template) throws IOException {
        fis = new FileInputStream(template);
        pfs = new POIFSFileSystem(fis);
        workbook = new HSSFWorkbook(pfs);
    }

    public HSSFWorkbook getWorkbook() {
        return workbook;
    }

    public boolean existCell(int sheetIndex, int rowIndex, int cellIndex) {
        if (sheetIndex >= workbook.getNumberOfSheets()) {
            return false;
        }
        HSSFSheet sheet = workbook.getSheetAt(sheetIndex);
        if (rowIndex > sheet.getLastRowNum()) {
            return false;
        }
        HSSFRow row = sheet.getRow(rowIndex);
        if (cellIndex >= row.getLastCellNum()) {
            return false;
        }
        return true;
    }

    public String getCellValue(int sheetIndex, int rowIndex, int cellIndex) {
        if (this.existCell(sheetIndex, rowIndex, cellIndex)) {
            try {
                return workbook.getSheetAt(sheetIndex).getRow(rowIndex).getCell(cellIndex).getStringCellValue();
            } catch (Exception ex) {
                double ret = workbook.getSheetAt(sheetIndex).getRow(rowIndex).getCell(cellIndex).getNumericCellValue();
                if (ret == (int) ret) {
                    return String.valueOf((int) ret);
                } else {
                    return String.valueOf(ret);
                }
            }
        } else {
            return null;
        }
    }

    public void setCenterCellValue(int sheetIndex, int rowIndex, int cellIndex, String value) {
        Font font = workbook.createFont();
        font.setFontHeightInPoints((short) 10);
        font.setFontName("宋体");
        if (this.existCell(sheetIndex, rowIndex, cellIndex)) {
            HSSFCell cell = workbook.getSheetAt(sheetIndex).getRow(rowIndex).getCell(cellIndex);
            HSSFCellStyle cellStyle = cell.getCellStyle();
            cellStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);
            cellStyle.setFont(font);
            cell.setCellStyle(cellStyle);
            cell.setCellValue(value);
        }
    }
    
    
    public void setLeftCellValue(int sheetIndex, int rowIndex, int cellIndex, String value) {
        Font font = workbook.createFont();
        font.setFontHeightInPoints((short) 10);
        font.setFontName("宋体");
        if (this.existCell(sheetIndex, rowIndex, cellIndex)) {
            HSSFCell cell = workbook.getSheetAt(sheetIndex).getRow(rowIndex).getCell(cellIndex);
            HSSFCellStyle cellStyle = cell.getCellStyle();
            cellStyle.setAlignment(HSSFCellStyle.ALIGN_LEFT);
            cellStyle.setFont(font);
            cell.setCellStyle(cellStyle);
            cell.setCellValue(value);
        }
    }

    public void saveDocument(String document) throws FileNotFoundException, IOException {
        workbook.write(new FileOutputStream(document));
    }

    public void close() throws IOException {
        fis.close();
    }

    public int createNewLineWithBorder(int sheetIndex) {
        int rowNo, prevNo;
        HSSFSheet sheet = workbook.getSheetAt(sheetIndex);
        int width = sheet.getRow(0).getLastCellNum() - 1;
        prevNo = sheet.getLastRowNum();
        rowNo = prevNo + 1;
        HSSFRow row = sheet.createRow(rowNo), prevRow = sheet.getRow(prevNo);
        for (int i = 0; i <= width; i++) {
            HSSFCell cell = row.createCell(i);
            HSSFCellStyle cellStyle = prevRow.getCell(i).getCellStyle();
            HSSFCellStyle nextStyle=workbook.createCellStyle();
            nextStyle.setBorderTop(HSSFCellStyle.BORDER_MEDIUM);
            nextStyle.setBorderLeft(cellStyle.getBorderLeft());
            nextStyle.setBorderRight(cellStyle.getBorderRight());
            nextStyle.setBorderBottom(HSSFCellStyle.BORDER_MEDIUM);
            cell.setCellStyle(nextStyle);
        }
        return rowNo;
    }

    public int createNewLineWithoutBorder(int sheetIndex) {
        int rowNo, prevNo;
        HSSFSheet sheet = workbook.getSheetAt(sheetIndex);
        int width = sheet.getRow(0).getLastCellNum() - 1;
        prevNo = sheet.getLastRowNum();
        rowNo = prevNo + 1;
        HSSFRow row = sheet.createRow(rowNo), prevRow = sheet.getRow(prevNo);
        for (int i = 0; i <= width; i++) {
            HSSFCell prevCell = prevRow.getCell(i);
            HSSFCellStyle cellStyle = prevCell.getCellStyle();
            cellStyle.setBorderBottom(HSSFCellStyle.BORDER_NONE);
            prevCell.setCellStyle(cellStyle);
            HSSFCell curCell = row.createCell(i);
            HSSFCellStyle nextStyle=workbook.createCellStyle();
            nextStyle.setBorderLeft(cellStyle.getBorderLeft());
            nextStyle.setBorderRight(cellStyle.getBorderRight());
            nextStyle.setBorderBottom(HSSFCellStyle.BORDER_MEDIUM);
            curCell.setCellStyle(nextStyle);
        }
        return rowNo;
    }
}

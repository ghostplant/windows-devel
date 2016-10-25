/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.academic;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Library;
import cn.edu.njust.steduman.database.Subscribe;
import cn.edu.njust.steduman.util.ExcelUtil;
import cn.edu.njust.steduman.util.HibernateUtil;
import cn.edu.njust.steduman.util.WebdiskUtil;
import java.io.InputStream;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Ghostplant
 */
public class LibraryOrderAction extends GlobalActionSupport {

    public float getChsEBookAmount() {
        return chsEBookAmount;
    }

    public void setChsEBookAmount(float chsEBookAmount) {
        this.chsEBookAmount = chsEBookAmount;
    }

    public float getChsEPeriodicalAmount() {
        return chsEPeriodicalAmount;
    }

    public void setChsEPeriodicalAmount(float chsEPeriodicalAmount) {
        this.chsEPeriodicalAmount = chsEPeriodicalAmount;
    }

    public float getChsPeriodicalAmount() {
        return chsPeriodicalAmount;
    }

    public void setChsPeriodicalAmount(float chsPeriodicalAmount) {
        this.chsPeriodicalAmount = chsPeriodicalAmount;
    }

    public float getChsStoreAmout() {
        return chsStoreAmout;
    }

    public void setChsStoreAmout(float chsStoreAmout) {
        this.chsStoreAmout = chsStoreAmout;
    }

    public int getDatabaseTypeAmount() {
        return databaseTypeAmount;
    }

    public void setDatabaseTypeAmount(int databaseTypeAmount) {
        this.databaseTypeAmount = databaseTypeAmount;
    }

    public float getFgnEBookAmount() {
        return fgnEBookAmount;
    }

    public void setFgnEBookAmount(float fgnEBookAmount) {
        this.fgnEBookAmount = fgnEBookAmount;
    }

    public float getFgnEPeriodicalAmount() {
        return fgnEPeriodicalAmount;
    }

    public void setFgnEPeriodicalAmount(float fgnEPeriodicalAmount) {
        this.fgnEPeriodicalAmount = fgnEPeriodicalAmount;
    }

    public float getFgnPeriodicalAmount() {
        return fgnPeriodicalAmount;
    }

    public void setFgnPeriodicalAmount(float fgnPeriodicalAmount) {
        this.fgnPeriodicalAmount = fgnPeriodicalAmount;
    }

    public float getFgnStoreAmount() {
        return fgnStoreAmount;
    }

    public void setFgnStoreAmount(float fgnStoreAmount) {
        this.fgnStoreAmount = fgnStoreAmount;
    }

    public float getTotalAmount() {
        return fgnStoreAmount + chsStoreAmout;
    }
    private int databaseTypeAmount;
    private float fgnStoreAmount, chsStoreAmout, fgnPeriodicalAmount, chsPeriodicalAmount, fgnEBookAmount, chsEBookAmount, fgnEPeriodicalAmount, chsEPeriodicalAmount;

    @Override
    public String init() {
        super.init();
        List<Library> libraries = HibernateUtil.getList(Library.class);
        Library library = null;
        if (libraries.isEmpty()) {
            Library crtLibrary = new Library();
            HibernateUtil.save(crtLibrary);
            library = crtLibrary;
        } else {
            library = libraries.iterator().next();
        }
        databaseTypeAmount = library.getDatabaseTypeAmount();
        fgnStoreAmount = library.getFgnStoreAmount();
        chsStoreAmout = library.getChsStoreAmout();
        fgnPeriodicalAmount = library.getFgnPeriodicalAmount();
        chsPeriodicalAmount = library.getChsPeriodicalAmount();
        fgnEBookAmount = library.getFgnEBookAmount();
        chsEBookAmount = library.getChsEBookAmount();
        fgnEPeriodicalAmount = library.getFgnEPeriodicalAmount();
        chsEPeriodicalAmount = library.getChsEPeriodicalAmount();
        return SUCCESS;
    }

    public String update() {
        List<Library> libraries = HibernateUtil.getList(Library.class);
        Library library = null;
        if (libraries.isEmpty()) {
            Library crtLibrary = new Library();
            HibernateUtil.save(crtLibrary);
            library = crtLibrary;
        } else {
            library = libraries.iterator().next();
        }
        library.setChsEBookAmount(chsEBookAmount);
        library.setChsEPeriodicalAmount(chsEPeriodicalAmount);
        library.setChsPeriodicalAmount(chsPeriodicalAmount);
        library.setChsStoreAmout(chsStoreAmout);
        library.setDatabaseTypeAmount(databaseTypeAmount);
        library.setFgnEBookAmount(fgnEBookAmount);
        library.setFgnEPeriodicalAmount(fgnEPeriodicalAmount);
        library.setFgnPeriodicalAmount(fgnPeriodicalAmount);
        library.setFgnStoreAmount(fgnStoreAmount);
        HibernateUtil.update(library);
        setMsgTitle("更新统计信息");
        setMsgTitle("新数据已经成功更新到数据库！");
        return SUCCESS;
    }
    private String type, bookName;
    private int orderYear;

    public String getBookName() {
        return bookName;
    }

    public void setBookName(String bookName) {
        this.bookName = bookName;
    }

    public int getOrderYear() {
        return orderYear;
    }

    public void setOrderYear(int orderYear) {
        this.orderYear = orderYear;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String insert() {
        if (bookName.length() == 0) {
            setMsgTitle("添加项目信息");
            setMsgInformation("您必须输入资源名称！");
        } else {
            Subscribe subscribe = new Subscribe();
            subscribe.setName(bookName);
            subscribe.setType(type);
            subscribe.setSubscribeYear(orderYear);
            HibernateUtil.save(subscribe);
            bookName = type = "";
            orderYear = 0;
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

    public List getForeignList() {
        return HibernateUtil.getSession().createQuery("from Subscribe where type='外文期刊'").list();
    }

    public List getNativeList() {
        return HibernateUtil.getSession().createQuery("from Subscribe where type='中文期刊'").list();
    }

    public List getElectronicList() {
        return HibernateUtil.getSession().createQuery("from Subscribe where type='数字资源'").list();
    }

    public String delete() {
        HibernateUtil.delete(HibernateUtil.get(Subscribe.class, selectedId));
        return SUCCESS;
    }

    public String export() {
        WebdiskUtil.clearTempFile();  // 第一步，清理
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile("library.xls")); // 第二步，载入
            workbook.setCenterCellValue(0, 1, 1, String.valueOf(getTotalAmount()));
            workbook.setCenterCellValue(0, 1, 3, String.valueOf(getChsStoreAmout()));
            workbook.setCenterCellValue(0, 1, 5, String.valueOf(getFgnStoreAmount()));
            workbook.setCenterCellValue(0, 1, 7, String.valueOf(getChsPeriodicalAmount()));
            workbook.setCenterCellValue(0, 1, 9, String.valueOf(getFgnPeriodicalAmount()));
            workbook.setCenterCellValue(0, 3, 1, String.valueOf(getDatabaseTypeAmount()));
            workbook.setCenterCellValue(0, 3, 3, String.valueOf(getChsEBookAmount()));
            workbook.setCenterCellValue(0, 3, 5, String.valueOf(getFgnEBookAmount()));
            workbook.setCenterCellValue(0, 3, 7, String.valueOf(getChsEPeriodicalAmount()));
            workbook.setCenterCellValue(0, 3, 9, String.valueOf(getFgnEPeriodicalAmount()));
            Iterator<Subscribe> it;
            it = getForeignList().iterator();
            String lineStr = "";
            while (it.hasNext()) {
                Subscribe subscribe = it.next();
                lineStr += subscribe.getName() + "(" + subscribe.getSubscribeYear() + " 起)  ；  ";
            }
            workbook.setLeftCellValue(0, 7, 0, lineStr);
            it = getNativeList().iterator();
            lineStr = "";
            while (it.hasNext()) {
                Subscribe subscribe = it.next();
                lineStr += subscribe.getName() + "(" + subscribe.getSubscribeYear() + " 起)  ；  ";
            }
            workbook.setLeftCellValue(0, 22, 0, lineStr);
            it = getElectronicList().iterator();
            lineStr = "";
            while (it.hasNext()) {
                Subscribe subscribe = it.next();
                lineStr += subscribe.getName() + "(" + subscribe.getSubscribeYear() + " 起)  ；  ";
            }
            workbook.setLeftCellValue(0, 33, 0, lineStr);
            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile("library.xls")); // 第三步，保存
            workbook.close(); // 第四步，释放资源与返回INPUT
            return INPUT;
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！" + ex.getMessage());
        }
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception { // 第五步，定义下载入口流；第六步修改struts2配置文件
        return WebdiskUtil.getSaveDocumentStream("library.xls");
    }
}

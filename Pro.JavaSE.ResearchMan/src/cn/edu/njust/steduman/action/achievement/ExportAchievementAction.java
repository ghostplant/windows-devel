/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.achievement;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.*;
import cn.edu.njust.steduman.util.*;
import java.io.InputStream;
import java.util.*;
import java.util.regex.*;
import org.hibernate.*;
import org.hibernate.criterion.Expression;

/**
 *
 * @author zyz
 */
public class ExportAchievementAction extends GlobalActionSupport {

    private String method, key, type;
    private Date startDate, endDate;
    private List<TempTuple> selectList, exportList;

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }

    public Date getEndDate() {
        return endDate;
    }

    public void setEndDate(Date endDate) {
        this.endDate = endDate;
    }

    public Date getStartDate() {
        return startDate;
    }

    public void setStartDate(Date startDate) {
        this.startDate = startDate;
    }

    public List<TempTuple> getExportList() {
        return exportList;
    }

    public void setExportList(List<TempTuple> exportList) {
        this.exportList = exportList;
    }

    public List<TempTuple> getSelectList() {
        return selectList;
    }

    public void setSelectList(List<TempTuple> selectList) {
        this.selectList = selectList;
    }

    public String getMethod() {
        return method;
    }

    public void setMethod(String method) {
        this.method = method;
    }

    private class TempTuple {

        private String presonName, achievementName, detail, date, sign, key;

        public String getKey() {
            return key;
        }

        public void setKey(String key) {
            this.key = key;
        }

        public String getAchievementName() {
            return achievementName;
        }

        public void setAchievementName(String achievementName) {
            this.achievementName = achievementName;
        }

        public String getDate() {
            return date;
        }

        public void setDate(String date) {
            this.date = date;
        }

        public String getDetail() {
            return detail;
        }

        public void setDetail(String detail) {
            this.detail = detail;
        }

        public String getPresonName() {
            return presonName;
        }

        public void setPresonName(String presonName) {
            this.presonName = presonName;
        }

        public String getSign() {
            return sign;
        }

        public void setSign(String sign) {
            this.sign = sign;
        }
    }

    boolean hasContent(String s) {
        if (s == null || s.trim().equals("")) {
            return false;
        }
        return true;
    }

    public String add() {

        selectList = (List<TempTuple>) SessionUtil.getSession("Export_selectList");
        exportList = (List<TempTuple>) SessionUtil.getSession("Export_exportList");
        type = (String) SessionUtil.getSession("Export_type");
        if ((type.equals("研究成果") || type.equals("专著发明")) && selectList != null && selectList.size() >= 20) {
            setMsgTitle("最多添加20条");
            return SUCCESS;
        }
        if ((type.equals("教学成果") || type.equals("论文")) && selectList != null && selectList.size() >= 10) {
            setMsgTitle("最多添加10条");
            return SUCCESS;
        }

        TempTuple tempTuple = null;
        for (int i = 0; i < selectList.size(); i++) {
            if (selectList.get(i).key.equals(key)) {
                tempTuple = selectList.remove(i);
                break;
            }
        }
        if (tempTuple != null) {
            exportList.add(tempTuple);
        }
        SessionUtil.setSession("Export_selectList", selectList);
        SessionUtil.setSession("Export_exportList", exportList);
        return SUCCESS;

    }

    public String remove() {

        selectList = (List<TempTuple>) SessionUtil.getSession("Export_selectList");
        exportList = (List<TempTuple>) SessionUtil.getSession("Export_exportList");
        type = (String) SessionUtil.getSession("Export_type");

        TempTuple tempTuple = null;
        for (int i = 0; i < exportList.size(); i++) {
            if (exportList.get(i).key.equals(key)) {
                tempTuple = exportList.remove(i);
                break;
            }
        }

        if (tempTuple != null) {
            selectList.add(tempTuple);
        }

        SessionUtil.setSession("Export_selectList", selectList);
        SessionUtil.setSession("Export_exportList", exportList);
        return SUCCESS;

    }

    public String moveUp() {

        selectList = (List<TempTuple>) SessionUtil.getSession("Export_selectList");
        exportList = (List<TempTuple>) SessionUtil.getSession("Export_exportList");
        type = (String) SessionUtil.getSession("Export_type");

        TempTuple tempTuple = null;
        for (int i = 0; i < exportList.size(); i++) {
            if (exportList.get(i).key.equals(key)) {
                if (i == 0) {
                    return SUCCESS;
                }
                tempTuple = exportList.get(i);
                exportList.set(i, exportList.get(i - 1));
                exportList.set(i - 1, tempTuple);
                break;
            }
        }

        SessionUtil.setSession("Export_exportList", exportList);
        return SUCCESS;

    }

    public String moveDown() {

        selectList = (List<TempTuple>) SessionUtil.getSession("Export_selectList");
        exportList = (List<TempTuple>) SessionUtil.getSession("Export_exportList");
        type = (String) SessionUtil.getSession("Export_type");

        TempTuple tempTuple = null;
        for (int i = 0; i < exportList.size(); i++) {
            if (exportList.get(i).key.equals(key)) {
                if (i == exportList.size() - 1) {
                    return SUCCESS;
                }
                tempTuple = exportList.get(i);
                exportList.set(i, exportList.get(i + 1));
                exportList.set(i + 1, tempTuple);
                break;
            }
        }

        SessionUtil.setSession("Export_exportList", exportList);
        return SUCCESS;

    }

    public String stuEssay() {

        selectList = new ArrayList<TempTuple>();
        exportList = new ArrayList<TempTuple>();
        Criteria poeCriteria = HibernateUtil.getSession().createCriteria(PersonOfEssay.class);
        if (startDate == null || endDate == null) {
            setMsgTitle("必须选择起始时间");
            return SUCCESS;
        }
        SessionUtil.setSession("startDate", startDate);
        SessionUtil.setSession("endDate", endDate);
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(startDate);
        int startYear, endYear, meetingYear, startMonth, endMonth, meetingMonth;
        startYear = calendar.get(Calendar.YEAR);
        startMonth = calendar.get(Calendar.MONTH) + 1;
        calendar.setTime(endDate);
        endYear = calendar.get(Calendar.YEAR);
        endMonth = calendar.get(Calendar.MONTH) + 1;
        String presonName, achievementName, detail, date, sign;
        List poeList = poeCriteria.list();
        Iterator poeIterator = poeList.iterator();
        while (poeIterator.hasNext()) {
            try {
                PersonOfEssay poe = (PersonOfEssay) poeIterator.next();
                if (poe.getRank() > 2) {
                    continue;
                }
                Criteria tempEssayCriteria = HibernateUtil.getSession().createCriteria(Essay.class);
                tempEssayCriteria.add(Expression.eq("id", poe.getEssayId()));
                Essay essay = (Essay) tempEssayCriteria.list().iterator().next();
                if (!essay.getStyle().equals("论文")) {
                    continue;
                }
                Person person = (Person) HibernateUtil.get(Person.class, poe.getPersonId());
                if (person.isTeacher()) {
                    continue;
                }
                if (poe.getRank() == 2) {
                    Student student = (Student) HibernateUtil.get(Student.class, person.getId());
                    Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, student.getTeacherId());
                    Query query = HibernateUtil.getSession().createQuery("from PersonOfEssay where personId=:personId and essayId=:essayId and rank=:rank");
                    query.setString("personId", teacher.getId());
                    query.setInteger("essayId", essay.getId());
                    query.setInteger("rank", 1);
                    if (query.list().isEmpty()) {
                        continue;
                    }
                }

                presonName = person.getName();
                achievementName = essay.getTitle();
                sign = poe.getRank() + "/" + essay.getTotalAuthor();

                if (hasContent(essay.getPeriodicalName())) {
                    String pattern = "([12][90]\\d\\d)\\s*年\\s*([1][012]|[1-9])\\s*月";
                    Pattern p = Pattern.compile(pattern);
                    Matcher matcher = p.matcher(essay.getPeriodicalPublish());
                    if (matcher.find()) {
                        meetingYear = Integer.valueOf(matcher.group(1));
                        meetingMonth = Integer.valueOf(matcher.group(2));
                        date = meetingYear + "." + meetingMonth;
                        if (meetingYear < startYear || meetingYear == startYear && meetingMonth < startMonth) {
                            continue;
                        }
                        if (meetingYear > endYear || meetingYear == endYear && meetingMonth > endMonth) {
                            continue;
                        }
                    } else {
                        throw new Exception();
                    }
                    detail = essay.getPeriodicalName();
                    if (hasContent(essay.getPeriodicalIssnNo())) {
                        detail += "、ISSN：" + essay.getPeriodicalIssnNo();
                    }
                    if (hasContent(essay.getPeriodicalCnNo())) {
                        detail += "、ISSN：" + essay.getPeriodicalCnNo();
                    }
                } else {
                    MeetingReport meetingReport = (MeetingReport) HibernateUtil.get(MeetingReport.class, essay.getMeetingId());
                    calendar.setTime(meetingReport.getReportDate());
                    meetingYear = calendar.get(Calendar.YEAR);
                    meetingMonth = calendar.get(Calendar.MONTH) + 1;
                    if (meetingYear < startYear || meetingYear == startYear && meetingMonth < startMonth) {
                        continue;
                    }
                    if (meetingYear > endYear || meetingYear == endYear && meetingMonth > endMonth) {
                        continue;
                    }
                    date = meetingYear + "." + meetingMonth;
                    detail = "ISBN：" + essay.getMeetingIsbnNo();
                }
                TempTuple tempTuple = new TempTuple();
                tempTuple.setAchievementName(achievementName);
                tempTuple.setDate(date);
                tempTuple.setDetail(detail);
                tempTuple.setPresonName(presonName);
                tempTuple.setSign(sign);
                tempTuple.setKey("E" + poe.getId());
                selectList.add(tempTuple);
            } catch (Exception e) {
                setMsgTitle("ERROR");
                e.printStackTrace();
                return SUCCESS;
            }
        }
        SessionUtil.setSession("Export_selectList", selectList);
        SessionUtil.setSession("Export_exportList", exportList);
        return SUCCESS;
    }

    public String teachAchieve() {

        selectList = new ArrayList<TempTuple>();
        exportList = new ArrayList<TempTuple>();

        Criteria toaCriteria = HibernateUtil.getSession().createCriteria(TeacherOfAchievement.class);
        SessionUtil.setSession("startDate", startDate);
        SessionUtil.setSession("endDate", endDate);
        Calendar calendar = Calendar.getInstance();
        String presonName, achievementName, detail, date, sign;

        List toaList = toaCriteria.list();
        Iterator toaIterator = toaList.iterator();
        while (toaIterator.hasNext()) {
            try {
                TeacherOfAchievement toa = (TeacherOfAchievement) toaIterator.next();
                Criteria tempAchieveCriteria = HibernateUtil.getSession().createCriteria(Achievement.class);
                tempAchieveCriteria.add(Expression.ge("date", startDate));
                tempAchieveCriteria.add(Expression.le("date", endDate));
                tempAchieveCriteria.add(Expression.eq("id", toa.getAchievementId()));
                Achievement achievement = (Achievement) tempAchieveCriteria.list().iterator().next();
                Person person = (Person) HibernateUtil.get(Person.class, toa.getTeacherId());
                presonName = person.getName();
                achievementName = achievement.getName();
                calendar.setTime(achievement.getDate());
                date = String.valueOf(calendar.get(Calendar.YEAR)) + "." + String.valueOf(calendar.get(Calendar.MONTH) + 1);
                sign = toa.getRank() + "/" + achievement.getFinishCount();
                detail = achievement.getAward();
                if (hasContent(achievement.getLevel())) {
                    detail += "、" + achievement.getLevel();
                }
                if (hasContent(achievement.getCertificationId())) {
                    detail += "、证书号：" + achievement.getCertificationId();
                }
                TempTuple tempTuple = new TempTuple();
                tempTuple.setAchievementName(achievementName);
                tempTuple.setDate("");
                tempTuple.setDetail(detail + "、" + date);
                tempTuple.setPresonName(presonName + "(" + sign + ")");
                tempTuple.setSign("");
                tempTuple.setKey("A" + toa.getId());
                selectList.add(tempTuple);
            } catch (Exception e) {
                setMsgTitle("ERROR");
                e.printStackTrace();
                return SUCCESS;
            }
        }
        SessionUtil.setSession("Export_selectList", selectList);
        SessionUtil.setSession("Export_exportList", exportList);
        return SUCCESS;
    }

    public String patentAchieve() {

        selectList = new ArrayList<TempTuple>();
        exportList = new ArrayList<TempTuple>();
        Criteria topCriteria = HibernateUtil.getSession().createCriteria(TeacherOfPatent.class);
        SessionUtil.setSession("startDate", startDate);
        SessionUtil.setSession("endDate", endDate);
        String presonName, achievementName, detail, date, sign;
        List topList = topCriteria.list();
        Iterator topIterator = topList.iterator();
        while (topIterator.hasNext()) {
            try {
                TeacherOfPatent top = (TeacherOfPatent) topIterator.next();
                Criteria tempPatentCriteria = HibernateUtil.getSession().createCriteria(Patent.class);
                tempPatentCriteria.add(Expression.ge("date", startDate));
                tempPatentCriteria.add(Expression.le("date", endDate));
                tempPatentCriteria.add(Expression.eq("id", top.getPatentId()));
                Patent patent = (Patent) tempPatentCriteria.list().iterator().next();
                Person person = (Person) HibernateUtil.get(Person.class, top.getTeacherId());
                presonName = person.getName();
                achievementName = patent.getName();
                Calendar calendar = Calendar.getInstance();
                calendar.setTime(patent.getDate());
                date = String.valueOf(calendar.get(Calendar.YEAR)) + "." + String.valueOf(calendar.get(Calendar.MONTH) + 1);
                sign = top.getRank() + "/" + patent.getFinishCount();
                detail = patent.getName();
                if (patent.getType().equals("专利")) {
                    detail = "专利号：" + patent.getRegistryNo();
                } else {
                    detail = "著作权登记号：" + patent.getRegistryNo();
                }
                TempTuple tempTuple = new TempTuple();
                tempTuple.setAchievementName(achievementName);
                tempTuple.setDate(date);
                tempTuple.setDetail(detail);
                tempTuple.setPresonName(presonName + "(" + sign + ")");
                tempTuple.setSign("");
                tempTuple.setKey("P" + top.getId());
                selectList.add(tempTuple);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        SessionUtil.setSession("Export_selectList", selectList);
        SessionUtil.setSession("Export_exportList", exportList);
        return SUCCESS;
    }

    public String essay() {

        selectList = new ArrayList<TempTuple>();
        exportList = new ArrayList<TempTuple>();
        Criteria poeCriteria = HibernateUtil.getSession().createCriteria(PersonOfEssay.class);
        if (startDate == null || endDate == null) {
            setMsgTitle("必须选择起始时间");
            return SUCCESS;
        }
        SessionUtil.setSession("startDate", startDate);
        SessionUtil.setSession("endDate", endDate);
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(startDate);
        int startYear, endYear, meetingYear, startMonth, endMonth, meetingMonth;
        startYear = calendar.get(Calendar.YEAR);
        startMonth = calendar.get(Calendar.MONTH) + 1;
        calendar.setTime(endDate);
        endYear = calendar.get(Calendar.YEAR);
        endMonth = calendar.get(Calendar.MONTH) + 1;
        String presonName, achievementName, detail, date, sign;
        List poeList = poeCriteria.list();
        Iterator poeIterator = poeList.iterator();
        while (poeIterator.hasNext()) {
            try {
                PersonOfEssay poe = (PersonOfEssay) poeIterator.next();
                Criteria tempEssayCriteria = HibernateUtil.getSession().createCriteria(Essay.class);
                tempEssayCriteria.add(Expression.eq("id", poe.getEssayId()));
                Essay essay = (Essay) tempEssayCriteria.list().iterator().next();
                if (!essay.getStyle().equals("论文")) {
                    continue;
                }
                Person person = (Person) HibernateUtil.get(Person.class, poe.getPersonId());
                presonName = person.getName();
                achievementName = essay.getTitle();
                sign = poe.getRank() + "/" + essay.getTotalAuthor();

                if (hasContent(essay.getPeriodicalName())) {
                    String pattern = "([12][90]\\d\\d)\\s*年\\s*([1][012]|[1-9])\\s*月";
                    Pattern p = Pattern.compile(pattern);
                    Matcher matcher = p.matcher(essay.getPeriodicalPublish());
                    if (matcher.find()) {
                        meetingYear = Integer.valueOf(matcher.group(1));
                        meetingMonth = Integer.valueOf(matcher.group(2));
                        date = meetingYear + "." + meetingMonth;
                        if (meetingYear < startYear || meetingYear == startYear && meetingMonth < startMonth) {
                            continue;
                        }
                        if (meetingYear > endYear || meetingYear == endYear && meetingMonth > endMonth) {
                            continue;
                        }
                    } else {
                        throw new Exception();
                    }
                    detail = essay.getPeriodicalName();
                    if (hasContent(essay.getPeriodicalIssnNo())) {
                        detail += "、ISSN：" + essay.getPeriodicalIssnNo();
                    }
                    if (hasContent(essay.getPeriodicalCnNo())) {
                        detail += "、ISSN：" + essay.getPeriodicalCnNo();
                    }
                } else {
                    MeetingReport meetingReport = (MeetingReport) HibernateUtil.get(MeetingReport.class, essay.getMeetingId());
                    calendar.setTime(meetingReport.getReportDate());
                    meetingYear = calendar.get(Calendar.YEAR);
                    meetingMonth = calendar.get(Calendar.MONTH) + 1;
                    if (meetingYear < startYear || meetingYear == startYear && meetingMonth < startMonth) {
                        continue;
                    }
                    if (meetingYear > endYear || meetingYear == endYear && meetingMonth > endMonth) {
                        continue;
                    }
                    date = meetingYear + "." + meetingMonth;
                    detail = "ISBN：" + essay.getMeetingIsbnNo();
                }
                TempTuple tempTuple = new TempTuple();
                tempTuple.setAchievementName(achievementName);
                tempTuple.setDate(date);
                tempTuple.setDetail(detail);
                tempTuple.setPresonName(presonName+"("+sign+")");
                tempTuple.setSign("");
                tempTuple.setKey("E" + poe.getId());
                selectList.add(tempTuple);
            } catch (Exception e) {
                setMsgTitle("ERROR");
                e.printStackTrace();
                return SUCCESS;
            }
        }
        SessionUtil.setSession("Export_selectList", selectList);
        SessionUtil.setSession("Export_exportList", exportList);
        return SUCCESS;
    }

    public String search() {

        selectList = (List<TempTuple>) SessionUtil.getSession("Export_selectList");
        exportList = (List<TempTuple>) SessionUtil.getSession("Export_exportList");
        SessionUtil.setSession("Export_type", type);

        if (type.equals("研究生成果")) {
            return stuEssay();
        }
        if (type.equals("教学成果")) {
            return teachAchieve();
        }
        if (type.equals("专著发明")) {
            return patentAchieve();
        } else if (type.equals("论文")) {
            return essay();
        }


        SessionUtil.setSession("Export_selectList", selectList);
        SessionUtil.setSession("Export_exportList", exportList);

        return SUCCESS;
    }

    public String export() {

        selectList = (List<TempTuple>) SessionUtil.getSession("Export_selectList");
        exportList = (List<TempTuple>) SessionUtil.getSession("Export_exportList");

        startDate = (Date) SessionUtil.getSession("startDate");
        endDate = (Date) SessionUtil.getSession("endDate");
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(startDate);
        int startYear, endYear, startMonth, endMonth;
        startYear = calendar.get(Calendar.YEAR);
        startMonth = calendar.get(Calendar.MONTH) + 1;
        calendar.setTime(endDate);
        endYear = calendar.get(Calendar.YEAR);
        endMonth = calendar.get(Calendar.MONTH) + 1;
        String filename = "", title = "";
        if (type.equals("研究生成果")) {
            filename = "stuEssay.xls";
            title = "Ⅵ-4本一级学科点" + startYear + "年" + startMonth + "月至" + endYear + "年" + endMonth + "月研究生在学期间有代表性学术成果（限填20项）";
        } else if (type.equals("教学成果")) {
            filename = "teachAchieve.xls";
            title = "Ⅵ-4本一级学科点" + startYear + "年" + startMonth + "月至" + endYear + "年" + endMonth + "月获省部级以上优秀教学成果、教材奖情况（限填10项）";
        } else if (type.equals("专著发明")) {
            filename = "patent.xls";
            title = "Ⅵ-4本一级学科点" + startYear + "年" + startMonth + "月至" + endYear + "年" + endMonth + "月出版学术专著（不含教材）、获授权发明专利情况（限填20项）";
        } else if (type.equals("论文")) {
            filename = "essay.xls";
            title = "Ⅵ-4本一级学科点" + startYear + "年" + startMonth + "月至" + endYear + "年" + endMonth + "月发表论文情况（限填10篇）";
        }
        WebdiskUtil.clearTempFile();  // 第一步，清理
        try {
            ExcelUtil workbook = new ExcelUtil(WebdiskUtil.getTemplateFile(filename)); // 第二步，载入
            workbook.setCenterCellValue(0, 0, 0, title);
            if (type.equals("研究生成果")) {
                for (int i = 0; i < exportList.size(); i++) {
                    TempTuple tempTuple = exportList.get(i);
                    workbook.setCenterCellValue(0, i + 2, 1, tempTuple.presonName);
                    workbook.setCenterCellValue(0, i + 2, 2, tempTuple.achievementName);
                    workbook.setLeftCellValue(0, i + 2, 3, tempTuple.detail);
                    workbook.setCenterCellValue(0, i + 2, 4, tempTuple.date);
                }
            } else if (type.equals("教学成果")) {
                for (int i = 0; i < exportList.size(); i++) {
                    TempTuple tempTuple = exportList.get(i);
                    workbook.setCenterCellValue(0, i + 2, 1, tempTuple.achievementName);
                    workbook.setCenterCellValue(0, i + 2, 2, tempTuple.presonName);
                    workbook.setLeftCellValue(0, i + 2, 3, tempTuple.detail);
                }
            } else if (type.equals("专著发明")||type.equals("论文")) {
                for (int i = 0; i < exportList.size(); i++) {
                    TempTuple tempTuple = exportList.get(i);
                    workbook.setCenterCellValue(0, i + 2, 1, tempTuple.achievementName);
                    workbook.setCenterCellValue(0, i + 2, 2, tempTuple.presonName);
                    workbook.setCenterCellValue(0, i + 2, 3, tempTuple.date);
                    workbook.setLeftCellValue(0, i + 2, 4, tempTuple.detail);
                }
            }

            workbook.saveDocument(WebdiskUtil.getSaveDocumentFile(filename)); // 第三步，保存
            workbook.close(); // 第四步，释放资源与返回INPUT
            if (type.equals("研究生成果")) {
                return "output_a";
            } else if (type.equals("教学成果")) {
                return "output_b";
            } else if (type.equals("专著发明")) {
                return "output_c";
            } else if (type.equals("论文")) {
                return "output_d";
            }
        } catch (Exception ex) {
            setMsgTitle("文件导出信息");
            setMsgInformation("不能正确处理模板，导出失败！");
        }
        return SUCCESS;
    }

    public InputStream getInputStream() throws Exception {
        // 第五步，定义下载入口流；第六步修改struts2配置文件
        String filename = "";
        if (type.equals("研究生成果")) {
            filename = "stuEssay.xls";
        } else if (type.equals("教学成果")) {
            filename = "teachAchieve.xls";
        } else if (type.equals("专著发明")) {
            filename = "patent.xls";
        } else if (type.equals("论文")) {
            filename = "essay.xls";
        }
        return WebdiskUtil.getSaveDocumentStream(filename);
    }

    @Override
    public String init() {
        super.init();
        SessionUtil.setSession("Export_selectList", new ArrayList<TempTuple>());
        SessionUtil.setSession("Export_exportList", new ArrayList<TempTuple>());
        return SUCCESS;
    }
}

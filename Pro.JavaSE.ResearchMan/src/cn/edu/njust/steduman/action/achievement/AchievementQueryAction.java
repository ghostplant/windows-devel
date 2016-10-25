/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.achievement;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.*;
import cn.edu.njust.steduman.util.*;
import java.util.*;
import java.util.regex.*;
import org.hibernate.Criteria;
import org.hibernate.Query;
import org.hibernate.criterion.Expression;

/**
 *
 * @author zyz
 */
public class AchievementQueryAction extends GlobalActionSupport {

    private String teacherName, achievementType;
    private Date startDate, endDate;
    private List<TempTuple> tupleList;
    private String delKey;

    public String getDelKey() {
        return delKey;
    }

    public void setDelKey(String delKey) {
        this.delKey = delKey;
    }

    public String getAchievementType() {
        return achievementType;
    }

    public void setAchievementType(String achievementType) {
        this.achievementType = achievementType;
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

    public String getTeacherName() {
        return teacherName;
    }

    public void setTeacherName(String teacherName) {
        this.teacherName = teacherName;
    }

    public List<TempTuple> getTupleList() {
        return tupleList;
    }

    public void setTupleList(List<TempTuple> tupleList) {
        this.tupleList = tupleList;
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

    public List getPersons() {
        List<Person> persons = HibernateUtil.getList(Person.class);
        Iterator<Person> iterator = persons.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            res.add(iterator.next().getName());
        }
        return res;
    }

    private String getId(String name) {
        Query query = HibernateUtil.getSession().createQuery("from Person where name=:name");
        query.setString("name", name);
        return ((Person) query.list().iterator().next()).getId();
    }

    private void tecahingAndResearchQuery() {
        Criteria achiveCriteria = HibernateUtil.getSession().createCriteria(Achievement.class);
        Criteria toaCriteria = HibernateUtil.getSession().createCriteria(TeacherOfAchievement.class);
        if (hasContent(teacherName)) {
            toaCriteria.add(Expression.eq("teacherId", getId(teacherName)));
        }
        if (startDate != null) {
            achiveCriteria.add(Expression.ge("date", startDate));
        }
        if (endDate != null) {
            achiveCriteria.add(Expression.le("date", endDate));
        }
        String presonName, achievementName, detail, date, sign;
        List toaList = toaCriteria.list();
        Iterator toaIterator = toaList.iterator();
        while (toaIterator.hasNext()) {
            try {
                TeacherOfAchievement toa = (TeacherOfAchievement) toaIterator.next();
                Criteria tempAchieveCriteria = HibernateUtil.getSession().createCriteria(Achievement.class);
                if (startDate != null) {
                    tempAchieveCriteria.add(Expression.ge("date", startDate));
                }
                if (endDate != null) {
                    tempAchieveCriteria.add(Expression.le("date", endDate));
                }
                tempAchieveCriteria.add(Expression.eq("id", toa.getAchievementId()));
                Achievement achievement = (Achievement) tempAchieveCriteria.list().iterator().next();
                Person person = (Person) HibernateUtil.get(Person.class, toa.getTeacherId());
                presonName = person.getName();
                achievementName = achievement.getName();
                Calendar calendar = Calendar.getInstance();
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
                tempTuple.setDate(date);
                tempTuple.setDetail(detail);
                tempTuple.setPresonName(presonName);
                tempTuple.setSign(sign);
                tempTuple.setKey("A" + toa.getId());
                tupleList.add(tempTuple);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void patentQuery() {
        Criteria patentCriteria = HibernateUtil.getSession().createCriteria(Patent.class);
        Criteria topCriteria = HibernateUtil.getSession().createCriteria(TeacherOfPatent.class);
        if (hasContent(teacherName)) {
            topCriteria.add(Expression.eq("teacherId", getId(teacherName)));
        }
        if (startDate != null) {
            patentCriteria.add(Expression.ge("date", startDate));
        }
        if (endDate != null) {
            patentCriteria.add(Expression.le("date", endDate));
        }
        String presonName, achievementName, detail, date, sign;
        List topList = topCriteria.list();
        Iterator toaIterator = topList.iterator();
        while (toaIterator.hasNext()) {
            try {
                TeacherOfPatent top = (TeacherOfPatent) toaIterator.next();
                Criteria tempPatentCriteria = HibernateUtil.getSession().createCriteria(Patent.class);
                if (startDate != null) {
                    tempPatentCriteria.add(Expression.ge("date", startDate));
                }
                if (endDate != null) {
                    tempPatentCriteria.add(Expression.le("date", endDate));
                }
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
                tempTuple.setPresonName(presonName);
                tempTuple.setSign(sign);
                tempTuple.setKey("P" + top.getId());
                tupleList.add(tempTuple);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void essayQuery() {
        Criteria essayCriteria = HibernateUtil.getSession().createCriteria(Essay.class);
        Criteria poeCriteria = HibernateUtil.getSession().createCriteria(PersonOfEssay.class);
        if (hasContent(teacherName)) {
            poeCriteria.add(Expression.eq("personId", getId(teacherName)));
        }
        if (startDate != null) {
            essayCriteria.add(Expression.ge("date", startDate));
        }
        if (endDate != null) {
            essayCriteria.add(Expression.le("date", endDate));
        }
        String presonName, achievementName, detail, date, sign;
        List poeList = poeCriteria.list();
        Iterator poeIterator = poeList.iterator();
        while (poeIterator.hasNext()) {
            try {
                PersonOfEssay poe = (PersonOfEssay) poeIterator.next();
                Criteria tempEssayCriteria = HibernateUtil.getSession().createCriteria(Essay.class);
                if (startDate != null) {
                    tempEssayCriteria.add(Expression.ge("date", startDate));
                }
                if (endDate != null) {
                    tempEssayCriteria.add(Expression.le("date", endDate));
                }
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
                        date = matcher.group(1) + "." + matcher.group(2);
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
                    Calendar calendar = Calendar.getInstance();
                    calendar.setTime(meetingReport.getReportDate());
                    date = String.valueOf(calendar.get(Calendar.YEAR)) + "." + String.valueOf(calendar.get(Calendar.MONTH) + 1);
                    detail = "ISBN：" + essay.getMeetingIsbnNo();
                }
                TempTuple tempTuple = new TempTuple();
                tempTuple.setAchievementName(achievementName);
                tempTuple.setDate(date);
                tempTuple.setDetail(detail);
                tempTuple.setPresonName(presonName);
                tempTuple.setSign(sign);
                tempTuple.setKey("E" + poe.getId());
                tupleList.add(tempTuple);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public String query() {
        tupleList = new ArrayList<TempTuple>();
        if (!hasContent(achievementType)) {
            tecahingAndResearchQuery();
            patentQuery();
            essayQuery();
        } else if (achievementType.equals("获奖")) {
            tecahingAndResearchQuery();
        } else if (achievementType.equals("论文")) {
            essayQuery();
        } else {
            patentQuery();
        }
        return SUCCESS;
    }

    private void delAchievement(int id) {
        TeacherOfAchievement toa = (TeacherOfAchievement) HibernateUtil.get(TeacherOfAchievement.class, id);
        HibernateUtil.delete(toa);
        Query query = HibernateUtil.getSession().createQuery("from Achievement where id=:id");
        query.setInteger("id", toa.getAchievementId());
        Achievement achievement = (Achievement) query.list().iterator().next();
        query=HibernateUtil.getSession().createQuery("from TeacherOfAchievement where achievementId=:id");
        query.setInteger("id", achievement.getId());
        List templList=query.list();
        if (templList.isEmpty()) {
            HibernateUtil.delete(achievement);
        }
        query();
    }


    private void delEssay(int id) {
        PersonOfEssay poe = (PersonOfEssay) HibernateUtil.get(PersonOfEssay.class, id);
        HibernateUtil.delete(poe);
        Query query = HibernateUtil.getSession().createQuery("from Essay where id=:id");
        query.setInteger("id", poe.getEssayId());
        Essay essay = (Essay) query.list().iterator().next();
        query=HibernateUtil.getSession().createQuery("from PersonOfEssay where essayId=:id");
        query.setInteger("id", essay.getId());
        List templList=query.list();
        if (templList.isEmpty()) {
            HibernateUtil.delete(essay);
        }
        query();
    }

    private void delPatent(int id) {
        TeacherOfPatent top = (TeacherOfPatent) HibernateUtil.get(TeacherOfPatent.class, id);
        HibernateUtil.delete(top);
        Query query = HibernateUtil.getSession().createQuery("from Patent where id=:id");
        query.setInteger("id", top.getPatentId());
        Patent patent = (Patent) query.list().iterator().next();
        query=HibernateUtil.getSession().createQuery("from TeacherOfPatent where patentId=:id");
        query.setInteger("id", patent.getId());
        List templList=query.list();
        if (templList.isEmpty()) {
            HibernateUtil.delete(patent);
        }
        query();
    }

    public String delete() {
        String type = delKey.substring(0, 1);
        int id = Integer.valueOf(delKey.substring(1));
        switch (type.charAt(0)) {
            case 'A':
                delAchievement(id);
                break;
            case 'E':
                delEssay(id);
                break;
            case 'P':
                delPatent(id);
                break;
        }
        return SUCCESS;
    }


    @Override
    public String init() {
        super.init();
        return SUCCESS;
    }
}

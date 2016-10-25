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
import org.hibernate.Query;

/**
 *
 * @author zyz
 */
public class EssayAchievementAction extends GlobalActionSupport {

    private String title, style, type, firstAuthorId, memo;
    private String firstAuthorName, firstAuthorName0, job, mobilePhone, firstWorkingSection, authorName;
    private List<TempAuther> authorList;
    private List<TempFund> fundList;
    private String periodicalName, foreignPeriodical, periodicalHolder, periodicalIssnNo, periodicalCnNo, periodicalPublish, fundName, fundId;
    private String meetingName, foreignMeeting, meetingPlace, meetingHolder, meetingIssnNo;
    private String treatisePublisher, treatiseBookNo, treatiseTotalKWord, treatiseSelfKWord;
    private int meatingId, totalAuthorNo;
    private Date meetingTime;

    public Date getMeetingTime() {
        return meetingTime;
    }

    public void setMeetingTime(Date meetingTime) {
        this.meetingTime = meetingTime;
    }

    public int getTotalAuthorNo() {
        return totalAuthorNo;
    }

    public void setTotalAuthorNo(int totalAuthorNo) {
        this.totalAuthorNo = totalAuthorNo;
    }

    public String getFirstAuthorName0() {
        return firstAuthorName0;
    }

    public void setFirstAuthorName0(String firstAuthorName0) {
        this.firstAuthorName0 = firstAuthorName0;
    }

    public List<TempFund> getFundList() {
        return fundList;
    }

    public void setFundList(List<TempFund> fundList) {
        this.fundList = fundList;
    }

    public List<TempAuther> getAuthorList() {
        return authorList;
    }

    public void setAuthorList(List<TempAuther> authorList) {
        this.authorList = authorList;
    }

    public String getFundId() {
        return fundId;
    }

    public void setFundId(String fundId) {
        this.fundId = fundId;
    }

    public int getMeatingId() {
        return meatingId;
    }

    public void setMeatingId(int meatingId) {
        this.meatingId = meatingId;
    }

    public String getAuthorName() {
        return authorName;
    }

    public void setAuthorName(String authorName) {
        this.authorName = authorName;
    }

    public String getFirstAuthorId() {
        return firstAuthorId;
    }

    public void setFirstAuthorId(String firstAuthorId) {
        this.firstAuthorId = firstAuthorId;
    }

    public String getFirstAuthorName() {
        return firstAuthorName;
    }

    public void setFirstAuthorName(String firstAuthorName) {
        this.firstAuthorName = firstAuthorName;
    }

    public String getFirstWorkingSection() {
        return firstWorkingSection;
    }

    public void setFirstWorkingSection(String firstWorkingSection) {
        this.firstWorkingSection = firstWorkingSection;
    }

    public String getForeignMeeting() {
        return foreignMeeting;
    }

    public void setForeignMeeting(String foreignMeeting) {
        this.foreignMeeting = foreignMeeting;
    }

    public String getForeignPeriodical() {
        return foreignPeriodical;
    }

    public void setForeignPeriodical(String foreignPeriodical) {
        this.foreignPeriodical = foreignPeriodical;
    }

    public String getFundName() {
        return fundName;
    }

    public void setFundName(String fundName) {
        this.fundName = fundName;
    }

    public String getFundNo() {
        return fundId;
    }

    public void setFundNo(String fundId) {
        this.fundId = fundId;
    }

    public String getJob() {
        return job;
    }

    public void setJob(String job) {
        this.job = job;
    }

    public String getMeetingHolder() {
        return meetingHolder;
    }

    public void setMeetingHolder(String meetingHolder) {
        this.meetingHolder = meetingHolder;
    }

    public String getMeetingIssnNo() {
        return meetingIssnNo;
    }

    public void setMeetingIssnNo(String meetingIssnNo) {
        this.meetingIssnNo = meetingIssnNo;
    }

    public String getMeetingName() {
        return meetingName;
    }

    public void setMeetingName(String meetingName) {
        this.meetingName = meetingName;
    }

    public String getMeetingPlace() {
        return meetingPlace;
    }

    public void setMeetingPlace(String meetingPlace) {
        this.meetingPlace = meetingPlace;
    }

    public String getMemo() {
        return memo;
    }

    public void setMemo(String memo) {
        this.memo = memo;
    }

    public String getMobilePhone() {
        return mobilePhone;
    }

    public void setMobilePhone(String mobilePhone) {
        this.mobilePhone = mobilePhone;
    }

    public String getPeriodicalCnNo() {
        return periodicalCnNo;
    }

    public void setPeriodicalCnNo(String periodicalCnNo) {
        this.periodicalCnNo = periodicalCnNo;
    }

    public String getPeriodicalHolder() {
        return periodicalHolder;
    }

    public void setPeriodicalHolder(String periodicalHolder) {
        this.periodicalHolder = periodicalHolder;
    }

    public String getPeriodicalIssnNo() {
        return periodicalIssnNo;
    }

    public void setPeriodicalIssnNo(String periodicalIssnNo) {
        this.periodicalIssnNo = periodicalIssnNo;
    }

    public String getPeriodicalName() {
        return periodicalName;
    }

    public void setPeriodicalName(String periodicalName) {
        this.periodicalName = periodicalName;
    }

    public String getPeriodicalPublish() {
        return periodicalPublish;
    }

    public void setPeriodicalPublish(String periodicalPublish) {
        this.periodicalPublish = periodicalPublish;
    }

    public String getStyle() {
        return style;
    }

    public void setStyle(String style) {
        this.style = style;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getTreatiseBookNo() {
        return treatiseBookNo;
    }

    public void setTreatiseBookNo(String treatiseBookNo) {
        this.treatiseBookNo = treatiseBookNo;
    }

    public String getTreatisePublisher() {
        return treatisePublisher;
    }

    public void setTreatisePublisher(String treatisePublisher) {
        this.treatisePublisher = treatisePublisher;
    }

    public String getTreatiseSelfKWord() {
        return treatiseSelfKWord;
    }

    public void setTreatiseSelfKWord(String treatiseSelfKWord) {
        this.treatiseSelfKWord = treatiseSelfKWord;
    }

    public String getTreatiseTotalKWord() {
        return treatiseTotalKWord;
    }

    public void setTreatiseTotalKWord(String treatiseTotalKWord) {
        this.treatiseTotalKWord = treatiseTotalKWord;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    private class TempAuther {

        private String name, workDep, job;

        public String getJob() {
            return job;
        }

        public void setJob(String job) {
            this.job = job;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public String getWorkDep() {
            return workDep;
        }

        public void setWorkDep(String workDep) {
            this.workDep = workDep;
        }

        public TempAuther() {
        }

        public TempAuther(String name, String workDep, String job) {
            this();
            this.job = job;
            this.name = name;
            this.workDep = workDep;
        }
    }

    private class TempFund {

        String id, name;

        public String getId() {
            return id;
        }

        public void setId(String id) {
            this.id = id;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public TempFund() {
        }

        public TempFund(String id, String name) {
            this.id = id;
            this.name = name;
        }
    }

    public List getPersons() {
        String tempPersonName1, tempPersonName2;
        Map usedPersonMap = new HashMap();
        authorList = (List<TempAuther>) SessionUtil.getSession("Essay_autherList");
        if (authorList != null) {
            Iterator iterator = authorList.iterator();
            while (iterator.hasNext()) {
                tempPersonName2 = ((TempAuther) iterator.next()).getName();
                usedPersonMap.put(tempPersonName2, tempPersonName2);
            }
        }
        if (hasContent(firstAuthorName)) {
            usedPersonMap.put(firstAuthorName, firstAuthorName);
        }
        List<Person> persons = HibernateUtil.getList(Person.class);
        Iterator<Person> iterator = persons.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            tempPersonName1 = ((Person) iterator.next()).getName();
            if (usedPersonMap.get(tempPersonName1) != null) {
                continue;
            }
            res.add(tempPersonName1);
        }
        return res;
    }

    public List getMeetings() {
        authorList = (List<TempAuther>) SessionUtil.getSession("Essay_autherList");
        List<Meeting> meetings = HibernateUtil.getList(Meeting.class);
        Iterator<Meeting> iterator = meetings.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            res.add(iterator.next().getName());
        }
        return res;
    }

    private String getWorkingSection(String id) {
        Person p = (Person) HibernateUtil.get(Person.class, id);
        if (p.isTeacher()) {
            Teacher t = (Teacher) HibernateUtil.get(Teacher.class, id);
            return t.getWorkingSection();
        } else {
            Student s = (Student) HibernateUtil.get(Student.class, id);
            Teacher t = (Teacher) HibernateUtil.get(Teacher.class, s.getTeacherId());
            return t.getWorkingSection();
        }
    }

    boolean hasContent(String s) {
        if (s == null || s.trim().equals("")) {
            return false;
        }
        return true;
    }

    public String submitName() {
        fundList = (List<TempFund>) SessionUtil.getSession("Essay_fundList");
        authorList = (List<TempAuther>) SessionUtil.getSession("Essay_autherList");
        firstAuthorName = firstAuthorName0;
        if (!hasContent(firstAuthorName)) {
            firstAuthorId = job = mobilePhone = firstWorkingSection = "";
            return SUCCESS;
        }
        Query query = HibernateUtil.getSession().createQuery("from Person where name=:name");
        query.setString("name", firstAuthorName);
        Person firstAuther = (Person) (query.list().iterator().next());
        firstAuthorId = firstAuther.getId();
        job = firstAuther.isTeacher() ? "教师" : "研究生";
        mobilePhone = firstAuther.getMobilePhone();
        firstWorkingSection = getWorkingSection(firstAuthorId);
        return SUCCESS;
    }

    public String addAuthor() {
        fundList = (List<TempFund>) SessionUtil.getSession("Essay_fundList");
        authorList = (List<TempAuther>) SessionUtil.getSession("Essay_autherList");
        if (!hasContent(authorName)) {
            return SUCCESS;
        }
        if (authorList == null) {
            authorList = new ArrayList<TempAuther>();
        }
        Query query = HibernateUtil.getSession().createQuery("from Person where name=:name");
        query.setString("name", authorName);
        Person auther = (Person) (query.list().iterator().next());
        authorList.add(new TempAuther(auther.getName(), getWorkingSection(auther.getId()), auther.isTeacher() ? "教师" : "研究生"));
        SessionUtil.setSession("Essay_autherList", authorList);
        authorName = "";
        return SUCCESS;
    }

    public String delFund() {
        authorList = (List<TempAuther>) SessionUtil.getSession("Essay_autherList");
        fundList = null;
        SessionUtil.setSession("Essay_fundList", null);
        return SUCCESS;
    }

    public String delAuthor() {
        fundList = (List<TempFund>) SessionUtil.getSession("Essay_fundList");
        authorList = null;
        SessionUtil.setSession("Essay_autherList", null);
        return SUCCESS;
    }

    public String addFund() {
        authorList = (List<TempAuther>) SessionUtil.getSession("Essay_autherList");
        fundList = (List<TempFund>) SessionUtil.getSession("Essay_fundList");
        if (!hasContent(fundName) || !hasContent(fundId)) {
            setMsgTitle("添加资金错误");
            setMsgInformation("必须输入名称和编号");
            return SUCCESS;
        }
        if (fundList == null) {
            fundList = new ArrayList<TempFund>();
        }
        fundList.add(new TempFund(fundId, fundName));
        SessionUtil.setSession("Essay_fundList", fundList);
        fundId = fundName = "";
        return SUCCESS;
    }

    public String submit() {

        fundList = (List<TempFund>) SessionUtil.getSession("Essay_fundList");
        authorList = (List<TempAuther>) SessionUtil.getSession("Essay_autherList");

        if (!hasContent(title) || !hasContent(style) || !hasContent(firstAuthorName) || (style.equals("论文") && !hasContent(memo))) {
            setMsgTitle("录入错误");
            setMsgInformation("带*的为必填内容");
            return SUCCESS;
        }

        totalAuthorNo = 1;
        if (authorList != null) {
            totalAuthorNo += authorList.size();
        }


        Essay essay = new Essay();
        Fund fund;
        PersonOfEssay poe;

        Query query = HibernateUtil.getSession().createQuery("from Person where name=:name");
        query.setString("name", firstAuthorName);
        firstAuthorId = ((Person) query.list().iterator().next()).getId();

        essay.setFirstAuthorId(firstAuthorId);
        essay.setStyle(style);
        essay.setTitle(title);
        essay.setType(type);
        essay.setTotalAuthor(totalAuthorNo);

        if (style.equals("论著") || style.equals("教材")) {
            essay.setTreatiseBookNo(treatiseBookNo);
            essay.setTreatisePublisher(treatisePublisher);
            if (!hasContent(treatiseBookNo) || !hasContent(treatisePublisher)) {
                setMsgTitle("录入错误");
                setMsgInformation("带*的为必填内容");
                return SUCCESS;
            }
            try {
                int to;
                int self;
                if (hasContent(treatiseTotalKWord)) {
                    to = Integer.valueOf(treatiseTotalKWord);
                } else {
                    to = 0;
                }
                if (hasContent(treatiseSelfKWord)) {
                    self = Integer.valueOf(treatiseSelfKWord);
                } else {
                    self = 0;
                }
                essay.setTreatiseTotalKWord(to);
                essay.setTreatiseSelfKWord(self);
                if (to < 0 || self < 0 || self > to) {
                    throw new Exception();
                }
            } catch (Exception e) {
                setMsgTitle("录入错误");
                setMsgInformation("人数输入错误");
                return SUCCESS;
            }
        } else {
            if (hasContent(periodicalName) && !hasContent(meetingName)) {//刊物发表

                String pattern = "([12][90]\\d\\d)\\s*年\\s*([1][012]|[1-9])\\s*月";
                Pattern p = Pattern.compile(pattern);
                Matcher matcher = p.matcher(periodicalPublish);
                if (!matcher.find()) {
                    setMsgTitle("录入错误");
                    setMsgInformation("刊物发表日期应包含*年*月字样,如1992年3月");
                    return SUCCESS;
                }
                essay.setForeignPeriodical(foreignPeriodical.equals("国际刊物") ? true : false);
                essay.setPeriodicalCnNo(periodicalCnNo);
                essay.setPeriodicalHolder(periodicalHolder);
                essay.setPeriodicalIssnNo(periodicalIssnNo);
                essay.setPeriodicalName(periodicalName);
                essay.setPeriodicalPublish(periodicalPublish);
            } else if (!hasContent(periodicalName) && hasContent(meetingName)) {
                try {
                    if (hasContent(meetingName)) {
                        query = HibernateUtil.getSession().createQuery("from Meeting where name=:name");
                        query.setString("name", meetingName);
                        meatingId = ((Meeting) query.list().iterator().next()).getId();
                    }
                } catch (Exception e) {
                    setMsgTitle("录入错误");
                    setMsgInformation("会议不存在");
                    return SUCCESS;
                }
                if (meetingTime == null) {
                    setMsgTitle("录入错误");
                    setMsgInformation("请正确输入会议时间");
                    return SUCCESS;
                }
                essay.setMeetingId(meatingId);
                essay.setMeetingIsbnNo(meetingIssnNo);
            }
            else{
                setMsgTitle("录入错误");
                setMsgInformation("期刊名称和会议名称必须输入且仅输入一条");
                return SUCCESS;
            }
            essay.setMemo(memo);
        }
        HibernateUtil.save(essay);
        int essayId = essay.getId();
        if (hasContent(meetingName)) {
            MeetingReport meetingReport = new MeetingReport();
            meetingReport.setEssayId(essayId);
            meetingReport.setMeetingId(meatingId);
            meetingReport.setReportDate(meetingTime);
            meetingReport.setTeacherId(firstAuthorId);
            HibernateUtil.save(meetingReport);
        }
        int rankCount = 0;

        poe = new PersonOfEssay();
        poe.setEssayId(essayId);
        poe.setPersonId(firstAuthorId);
        poe.setRank(++rankCount);
        HibernateUtil.save(poe);

        if (authorList != null && authorList.size() > 0) {
            Iterator<TempAuther> iterator = authorList.iterator();
            while (iterator.hasNext()) {
                TempAuther tmp = iterator.next();
                query = HibernateUtil.getSession().createQuery("from Person where name=:name");
                query.setString("name", tmp.name);
                poe = new PersonOfEssay();
                poe.setEssayId(essayId);
                poe.setPersonId(((Person) query.iterate().next()).getId());
                poe.setRank(++rankCount);
                System.out.println(rankCount);
                HibernateUtil.save(poe);
            }
        }

        if (fundList != null && fundList.size() > 0) {
            Iterator<TempFund> iterator = fundList.iterator();
            while (iterator.hasNext()) {
                TempFund tmp = iterator.next();
                fund = new Fund();
                fund.setEssayId(essayId);
                fund.setFundName(tmp.name);
                fund.setFundNo(tmp.id);
                HibernateUtil.save(fund);
            }
        }
        title = style = type = memo = "";
        firstAuthorName = job = mobilePhone = firstWorkingSection = authorName = "";
        periodicalName = foreignPeriodical = periodicalHolder = periodicalIssnNo = periodicalCnNo = periodicalPublish = fundName = fundId = "";
        meetingName = foreignMeeting = meetingPlace = meetingHolder = meetingIssnNo = "";
        treatisePublisher = treatiseBookNo = treatiseTotalKWord = treatiseSelfKWord = "";
        authorList = null;
        fundList = null;
        meetingTime = null;
        SessionUtil.setSession("Essay_autherList", null);
        SessionUtil.setSession("Essay_fundList", null);
        setMsgTitle("录入成功");
        return SUCCESS;
    }

    @Override
    public String init() {
        super.init();
        SessionUtil.setSession("Essay_autherList", null);
        SessionUtil.setSession("Essay_fundList", null);
        return SUCCESS;
    }
}

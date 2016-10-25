/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.achievement;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.*;
import cn.edu.njust.steduman.util.*;
import java.util.*;
import org.hibernate.Query;

/**
 *
 * @author zyz
 */
public class ResearchAchievementAction extends GlobalActionSupport {

    private Map personMap;
    private String name, award, teacherName, level, certificationId, rank, finishCount;
    private int rank0, finishCount0, achId;
    private Date date;

    public int getAchId() {
        return achId;
    }

    public void setAchId(int achId) {
        this.achId = achId;
    }

    public String getAward() {
        return award;
    }

    public void setAward(String award) {
        this.award = award;
    }

    public String getCertificationId() {
        return certificationId;
    }

    public void setCertificationId(String certificationId) {
        this.certificationId = certificationId;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public String getFinishCount() {
        return finishCount;
    }

    public void setFinishCount(String finishCount) {
        this.finishCount = finishCount;
    }

    public int getFinishCount0() {
        return finishCount0;
    }

    public void setFinishCount0(int finishCount0) {
        this.finishCount0 = finishCount0;
    }

    public String getLevel() {
        return level;
    }

    public void setLevel(String level) {
        this.level = level;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Map getPersonMap() {
        return personMap;
    }

    public void setPersonMap(Map personMap) {
        this.personMap = personMap;
    }

    public String getRank() {
        return rank;
    }

    public void setRank(String rank) {
        this.rank = rank;
    }

    public int getRank0() {
        return rank0;
    }

    public void setRank0(int rank0) {
        this.rank0 = rank0;
    }

    public String getTeacherName() {
        return teacherName;
    }

    public void setTeacherName(String teacherName) {
        this.teacherName = teacherName;
    }

    public List getTeachers() {
        String tempTeacherName1, tempTeacherName2;
        Map usedTeacherMap = new HashMap();
        Iterator iterator;
        personMap = (TreeMap) SessionUtil.getSession("Research_personMap");
        if (personMap != null) {
            iterator = personMap.keySet().iterator();
            while (iterator.hasNext()) {
                rank0 = (Integer) iterator.next();
                tempTeacherName2 = (String) personMap.get(rank0);
                usedTeacherMap.put(tempTeacherName2, tempTeacherName2);
            }
        }
        List<Teacher> teachers = HibernateUtil.getList(Teacher.class);
        iterator = teachers.iterator();
        List<String> res = new ArrayList<String>();
        res.add("");
        while (iterator.hasNext()) {
            tempTeacherName1 = ((Teacher) iterator.next()).getName();
            if (usedTeacherMap.get(tempTeacherName1) == null) {
                res.add(tempTeacherName1);
            }
        }
        return res;
    }

    boolean hasContent(String s) {
        if (s == null || s.trim().equals("")) {
            return false;
        }
        return true;
    }

    public String addPerson() {

        personMap = (TreeMap) SessionUtil.getSession("Research_personMap");
        if (personMap == null) {
            personMap = new TreeMap();
        }
        if(!hasContent(teacherName)){
            setMsgTitle("录入错误");
            setMsgInformation("输入教师名");
            return SUCCESS;
        }
        try {
            if (!hasContent(rank)) {
                throw new Exception();
            }
            rank0 = Integer.valueOf(rank);
            finishCount0 = Integer.valueOf(finishCount);
            if (finishCount0 <= 0 || rank0 <= 0 || rank0 > finishCount0) {
                throw new Exception();
            }
        } catch (Exception e) {
            setMsgTitle("录入错误");
            setMsgInformation("排名/总人数输入错误");
            return SUCCESS;
        }
        personMap.put(rank0, teacherName);
        SessionUtil.setSession("Research_personMap", personMap);
        rank = teacherName = "";
        return SUCCESS;
    }

    public String delAll() {
        SessionUtil.setSession("Research_personMap", null);
        personMap=null;
        return SUCCESS;
    }

    public String submit() {
        personMap = (TreeMap) SessionUtil.getSession("Research_personMap");
        if (!hasContent(name) || !hasContent(finishCount) || !hasContent(award)) {
            setMsgTitle("录入错误");
            setMsgInformation("带*的为必填内容");
        } else if (date == null) {
            setMsgTitle("录入错误");
            setMsgInformation("请正确输入日期");
        } else if (personMap == null || personMap.isEmpty()) {
            setMsgTitle("录入错误");
            setMsgInformation("至少输入一个获奖人");
        }else {
            try {
                finishCount0 = Integer.valueOf(finishCount);
            } catch (Exception e) {
                setMsgTitle("录入错误");
                setMsgInformation("总人数输入错误");
                return SUCCESS;
            }

            Achievement ach = new Achievement();
            TeacherOfAchievement toa;
            ach.setAward(award);
            ach.setCertificationId(certificationId);
            ach.setDate(date);
            ach.setLevel(level);
            ach.setName(name);
            ach.setType("科研成果");
            ach.setFinishCount(finishCount0);
            HibernateUtil.save(ach);
            achId = ach.getId();

            Iterator iterator=personMap.keySet().iterator();
            while (iterator.hasNext()) {
                toa = new TeacherOfAchievement();
                rank0 = (Integer)iterator.next();
                teacherName = (String) personMap.get(rank0);
                Query query = HibernateUtil.getSession().createQuery("from Person where name=:name");
                query.setString("name", teacherName);
                toa.setRank(rank0);
                toa.setTeacherId(((Person) query.list().iterator().next()).getId());
                toa.setAchievementId(achId);
                HibernateUtil.save(toa);
            }

            name = teacherName = rank = award = level = certificationId = finishCount = "";
            date = null;
            personMap=null;
            SessionUtil.setSession("Research_personMap", null);
            setMsgTitle("录入成功");

        }
        return SUCCESS;
    }
    @Override
    public String init(){
        super.init();
        SessionUtil.setSession("Research_personMap", null);
        return SUCCESS;
    }
}

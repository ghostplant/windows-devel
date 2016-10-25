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
public class PatentAchievementAction extends GlobalActionSupport {

    private String numA, numB, name, personName, rank, finishCount;
    private Date date;
    private String registryNo, type;
    private Map personMap;

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

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getNumA() {
        return numA;
    }

    public void setNumA(String numA) {
        this.numA = numA;
    }

    public String getNumB() {
        return numB;
    }

    public void setNumB(String numB) {
        this.numB = numB;
    }

    public Map getPersonMap() {
        return personMap;
    }

    public void setPersonMap(Map personMap) {
        this.personMap = personMap;
    }

    public String getPersonName() {
        return personName;
    }

    public void setPersonName(String personName) {
        this.personName = personName;
    }

    public String getRank() {
        return rank;
    }

    public void setRank(String rank) {
        this.rank = rank;
    }

    public String getRegistryNo() {
        return registryNo;
    }

    public void setRegistryNo(String registryNo) {
        this.registryNo = registryNo;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    boolean hasContent(String s) {
        if (s == null || s.trim().equals("")) {
            return false;
        }
        return true;
    }

   public List getTeachers() {
        String tempTeacherName1, tempTeacherName2;
        Map usedTeacherMap = new HashMap();
        Iterator iterator;
        personMap = (TreeMap) SessionUtil.getSession("Patent_personMap");
        if (personMap != null) {
            iterator = personMap.keySet().iterator();
            while (iterator.hasNext()) {
                int rank0 = (Integer) iterator.next();
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

    public String addPerson() {
        personMap = (TreeMap) SessionUtil.getSession("Patent_personMap");
        if (!hasContent(personName)) {
            setMsgTitle("录入错误");
            setMsgInformation("需要输入权利人");
            return SUCCESS;
        }
        int rank0, finishCount0;
        if (personMap == null) {
            personMap = new TreeMap();
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
        personMap.put(rank0, personName);
        SessionUtil.setSession("Patent_personMap", personMap);
        rank = personName = "";
        return SUCCESS;
    }

    public String delAll() {
        SessionUtil.setSession("Patent_personMap", null);
        personMap = null;
        return SUCCESS;
    }

    @Override
    public String init() {
        super.init();
        SessionUtil.setSession("Patent_personMap", null);
        return SUCCESS;
    }

    public String submit() {
        personMap = (TreeMap) SessionUtil.getSession("Patent_personMap");
        if (hasContent(numA) && !hasContent(numB)) {
            registryNo = numA;
            type = "专利";
        } else if (!hasContent(numA) && hasContent(numB)) {
            registryNo = numB;
            type = "著作";
        } else {
            setMsgTitle("录入错误");
            setMsgInformation("专利号和著作号必须输入且仅输入一个");
            return SUCCESS;
        }
        if (!hasContent(name)) {
            setMsgTitle("录入错误");
            setMsgInformation("请输入专利/著作名");
            return SUCCESS;
        }
        if (date == null) {
            setMsgTitle("录入错误");
            setMsgInformation("请输入日期");
            return SUCCESS;
        }
        if (personMap == null || personMap.isEmpty()) {
            setMsgTitle("录入错误");
            setMsgInformation("至少有一个权利人");
            return SUCCESS;
        }
        int finishCount0 = Integer.valueOf(finishCount);

        Patent patent = new Patent();
        patent.setDate(date);
        patent.setFinishCount(finishCount0);
        patent.setName(name);
        patent.setRegistryNo(registryNo);
        patent.setType(type);
        HibernateUtil.save(patent);

        int patentId = patent.getId();
        Iterator iterator = personMap.keySet().iterator();
        while (iterator.hasNext()) {
            int rank0 = (Integer) iterator.next();
            personName = (String) personMap.get(rank0);
            TeacherOfPatent top = new TeacherOfPatent();
            top.setPatentId(patentId);
            top.setRank(rank0);
            Query query = HibernateUtil.getSession().createQuery("from Person where name=:name");
            query.setString("name", personName);
            top.setTeacherId(((Person) query.list().iterator().next()).getId());
            HibernateUtil.save(top);
        }
        numA=numB=name=personName=rank=finishCount="";
        SessionUtil.setSession("Patent_personMap", null);
        personMap=null;
        date=null;
        setMsgTitle("录入成功");
        return SUCCESS;
    }
}

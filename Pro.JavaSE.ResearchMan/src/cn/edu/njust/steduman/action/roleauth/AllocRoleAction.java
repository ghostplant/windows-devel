/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cn.edu.njust.steduman.action.roleauth;

import cn.edu.njust.steduman.action.GlobalActionSupport;
import cn.edu.njust.steduman.database.Role;
import cn.edu.njust.steduman.database.Teacher;
import cn.edu.njust.steduman.util.HibernateUtil;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

/**
 *
 * @author Ghostplant
 */
public class AllocRoleAction extends GlobalActionSupport {

    private String play[];

    public String[] getPlay() {
        return play;
    }

    public void setPlay(String[] play) {
        this.play = play;
    }
    private String teacherId;

    public String getTeacherId() {
        return teacherId;
    }

    public void setTeacherId(String teacherId) {
        this.teacherId = teacherId;
    }

    @Override
    public String init() {
        super.init();
        if (getMenuItem() != null) {
            if (getRoles().length == 0) {
                setMsgTitle("用户分配角色信息");
                setMsgInformation("对不起，目前没有任何角色条目，故无法进行该操作！");
                setMenuItem(null);
            } else if (getTeachers().isEmpty()) {
                setMsgTitle("用户分配角色信息");
                setMsgInformation("对不起，目前没有任何用户条目，故无法进行该操作！");
                setMenuItem(null);
            }
        }
        return SUCCESS;
    }

    public List getTeachers() {
        return HibernateUtil.getList(Teacher.class);
    }

    public String[] getRoles() {
        List<Role> list = HibernateUtil.getList(Role.class);
        Iterator<Role> it = list.iterator();
        String arr[] = new String[list.size()];
        int i = 0;
        while (it.hasNext()) {
            Role ro = it.next();
            arr[i++] = ro.getId() + ":" + ro.getName();
        }
        return arr;
    }
    private boolean firstStep = true;

    public boolean isFirstStep() {
        return firstStep;
    }

    public void setFirstStep(boolean firstStep) {
        this.firstStep = firstStep;
    }
    private String selectedTeacherName;

    public String getSelectedTeacherName() {
        return selectedTeacherName;
    }

    public void setSelectedTeacherName(String selectedTeacherName) {
        this.selectedTeacherName = selectedTeacherName;
        setTextSelectedTeacherName(selectedTeacherName);
    }
    private String selectedTeacherId;

    public String getSelectedTeacherId() {
        return selectedTeacherId;
    }

    public void setSelectedTeacherId(String selectedTeacherId) {
        this.selectedTeacherId = selectedTeacherId;
    }

    public String next() {
        setSelectedTeacherId(teacherId);
        setSelectedTeacherName(((Teacher) HibernateUtil.get(Teacher.class, teacherId)).getName());
        firstStep = false;

        Teacher selTeacher = (Teacher) HibernateUtil.get(Teacher.class, teacherId);
        Set<Role> role = selTeacher.getRoles();
        String[] value = new String[role.size()];
        Iterator<Role> it = role.iterator();
        int i = 0;
        while (it.hasNext()) {
            Role ro = it.next();
            value[i++] = ro.getId() + ":" + ro.getName();
        }
        setPlay(value);
        return SUCCESS;
    }
    private String textSelectedTeacherName;

    public String getTextSelectedTeacherName() {
        return textSelectedTeacherName;
    }

    public void setTextSelectedTeacherName(String textSelectedTeacherName) {
        this.textSelectedTeacherName = textSelectedTeacherName;
    }

    public String update() {
        textSelectedTeacherName = selectedTeacherName;
        firstStep = false;
        Teacher teacher = (Teacher) HibernateUtil.get(Teacher.class, getSelectedTeacherId());
        if (teacher == null) {
            return SUCCESS;
        }
        Set authorities = new HashSet();
        for (String part : play) {
            int roleId = Integer.parseInt(part.substring(0, part.indexOf(":")));
            authorities.add(HibernateUtil.get(Role.class, roleId));
        }
        teacher.setRoles(authorities);
        HibernateUtil.update(teacher);
        setMsgTitle("用户分配角色信息");
        setMsgInformation("用户'" + selectedTeacherName + "'的所有角色组已经成功更新到数据库！");
        return SUCCESS;
    }
}

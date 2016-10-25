<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="querycourse" theme="simple">
        <table border="1" cellspacing="0" cellpadding="0">
            <tr >
                <td width="690"  align="center" valign="middle" colspan="10"style="background-color: rgb(255, 255, 51)"><p><strong>课程管理系统</strong><strong>（查询课程）</strong></p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td><p align="center">课程编号 </p></td>
                <td><p align="center">课程名称</p></td>
                <td><p align="center">学分</p></td>
                <td><p align="center">学时</p></td>
                <td><p align="center">授课对象</p></td>
                <td><p align="center">类型（公共、专业等）</p></td>
                <td><p align="center">教师</p></td>
                <td><p align="center">状态</p></td>
                <td><p align="center">课程操作1</p></td>
                <td><p align="center">课程操作2</p></td>

            </tr>
            <tr style="background-color: rgb(153, 255, 255)">
                <td><p align="center">-- </p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center"><s:select list="courses" name="classname" label="按课程查询"/><s:submit value="按课程查询" method="queryByCourse"/></p></td>
                <td><p align="center"><s:select list="teachers"  name="teachername" label="按老师查询"/><s:submit value="按老师查询" method="queryByTeacher" /></p></td>
            </tr>
            <s:if test="course!=null">
                <tr style="background-color: rgb(153, 255, 255)">
                    <td width="100" valign="top"><p align="center"><s:property value="course.id"/></p></td>
                    <td width="200" valign="top"><p align="center"><s:property value="course.name"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="course.creditValue"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="course.creditHour"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="course.target"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="course.type"/></p></td>
                    <td width="200" valign="top"><p align="center">
                            <s:iterator value="getTeacherSet(course.id)" id="tea">
                                <s:property value="#tea.getId()"/> <s:property value="#tea.getName()"/> <br/>
                            </s:iterator>
                        </p>
                    </td>
                    <td width="100" valign="top"><p align="center">
                            <s:if test="course.established">
                                已开设
                            </s:if>
                            <s:else>
                                待开放
                            </s:else>
                        </p>
                    </td>
                    <td width="100"><p align="center">-----</p></td>
                    <td width="100"><p align="center">-----</p></td>
                </tr>
            </s:if>
            <s:if test="teacher!=null">
              
                <s:iterator value="getCourseSet(teacher.id)" id="cour">
                    <tr style="background-color: rgb(153, 255, 255)">
                    <td width="100" valign="top"><p align="center"><s:property value="#cour.id"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="#cour.name"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="#cour.creditValue"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="#cour.creditHour"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="#cour.target"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="#cour.type"/></p></td>
                    <td width="100" valign="top"><p align="center"><s:property value="teacher.name"/></p></td>
                    <td width="100" valign="top"><p align="center">
                            <s:if test="#cour.established">
                                已开设
                            </s:if>
                            <s:else>
                                待开放
                            </s:else>
                        </p>
                    </td>
                    <td width="100"><p align="center">-----</p></td>
                    <td width="100"><p align="center">-----</p></td>
                </tr>
                </s:iterator>
            </s:if>

        </table>
    </s:form>
</body>

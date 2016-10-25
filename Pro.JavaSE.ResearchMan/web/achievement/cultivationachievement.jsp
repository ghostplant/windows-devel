<%--
    Document   : cultivationachievement
    Created on : 2011-9-14, 21:08:08
    Author     : zyz
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<%@taglib prefix="jscalendar" uri="/jscalendar" %>
<!DOCTYPE html>
<html>

    <body>
        <s:form action="cultivationachievement" theme="simple">
            <s:div align="center">
                <h1>优秀人才培养</h1>
            </s:div>
            <table width="700" border="1" cellpadding="0" cellspacing="0" align="center">
                <tr>
                    <td width="114" valign="top"><p align="center">时间</p></td>
                    <td width="117" valign="top"><p align="center">学生</p></td>
                    <td width="117" valign="top"><p align="center">指导教师</p></td>
                    <td width="114" valign="top"><p align="center">论文题目</p></td>
                    <td width="110" valign="top"><p align="center">研究方向</p></td>
                    <td width="114" valign="top"><p align="center">所属学科</p></td>
                </tr>
                <tr>
                    <td width="114" valign="middle" align="center">
                        <jscalendar:head/>
                        <jscalendar:jscalendar name="date" format="%Y-%m-%d" showstime="true" size="6"/>
                    </td>
                    <td width="117" valign="middle" align="left">
                        学号：<s:select list="students" name="studentId"/><br/>
                        姓名：<s:label name="studentName"/>
                    </td>
                    <td width="117" valign="middle" align="left">
                        工资号：<s:label name="teacherId"/><br/>
                        姓&nbsp;&nbsp;&nbsp;&nbsp;名：<s:label name="teacherName"/>
                    </td>
                    <td width="114" valign="middle" align="center">
                        <s:textfield name="essayTitle" size="5" maxlength="150"/>
                    </td>
                    <td width="110" valign="middle" align="center">
                        <s:textfield name="major" size="5" maxlength="150"/>
                    </td>
                    <td width="114" valign="middle" align="center">
                        <s:textfield name="subject" size="5" maxlength="150"/>
                    </td>
                </tr>
            </table>
            <s:div align="center">
                <s:submit method="getInfo" value="更多信息"/>
                <s:submit method="submit" value="确认提交"/>
            </s:div>
        </s:form>
    </body>
</html>
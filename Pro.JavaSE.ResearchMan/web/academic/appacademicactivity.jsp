<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<%@taglib prefix="jsc" uri="/jscalendar" %>

<body>
    <s:form action="appacademicactivity" theme="simple">
        <table align="center" border="1" cellspacing="0" cellpadding="0">
            <tr>
                <td style="background-color: rgb(255, 255, 51)"  width="690" colspan="5"><p align="center"><strong>本一级学科点教师（或研究生）在国外讲学或在国际会议上做报告情况</strong></p></td>
            </tr>
            <tr style="background-color:papayawhip">
                <td><p align="center">讲学或报告人员姓名 </p></td>
                <td><p align="center">国外大学名称或国际会议名称 </p></td>
                <td><p align="center">讲学或报告时间 </p></td>
                <td><p align="center">讲学或报告名称 </p></td>
                <td><p align="center">记录操作 </p></td>
            </tr>
            <s:iterator value="%{recordList}" id="id">
                <tr>
                    <td valign="top"><p><s:property value="getTeacherName(#id.teacherId)"/></p>
                    </td>
                    <td valign="top"><p><s:property value="getMeetingName(#id.meetingId)"/></p>
                    </td>
                    <td valign="top"><p align="center"><s:property value="castToString(#id.reportDate)"/></p>
                    </td>
                    <td valign="top"><p><s:property value="getEssayName(#id.essayId)"/></p>
                    </td>
                    <td valign="top"><p>
                            <s:url id="url" method="delete">
                                <s:param name="selectedId" value="#id.id"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a>
                            </p>
                        </td>
                    </tr>
            </s:iterator>

            <tr>
                <td valign="top"><p><s:select list="teacherList" listValue="name" listKey="id" name="teacherId" theme="simple"/></p>
                </td>
                <td valign="top"><p><s:select list="meetingList" listValue="name" listKey="id" name="destinationId" theme="simple"/></p>
                </td>
                <td valign="top"><p>
                        <jsc:head theme="simple"/><jsc:jscalendar theme="simple" name="reportDate" format="%Y-%m-%d" showstime="true"/>
                    </p>
                </td>
                <td valign="top"><p><s:select list="essayList" listValue="title" listKey="id" name="essayId" theme="simple"/></p>
                </td>
                <td valign="top"><p><s:submit theme="simple" value="添加" method="insert"/></p>
                </td>
            </tr>
        </table>
        <br/>
        <div align="center"><s:submit theme="simple" value="导出" method="export"/></div>
    </s:form>
</body>

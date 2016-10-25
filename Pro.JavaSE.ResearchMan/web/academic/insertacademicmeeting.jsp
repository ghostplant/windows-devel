<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<%@taglib prefix="jsc" uri="/jscalendar" %>

<body>
    <s:form action="insertacademicmeeting" theme="simple">

        <table align="center" border="1" cellspacing="0" cellpadding="0">
            <tr>
                <td style="background-color: rgb(255, 255, 51)" width="690" colspan="6"><p align="center"><strong>本一级学科点举办的主要国际国内学术会议</strong></p></td>
            </tr>
            <tr style="background-color:papayawhip">
                <td rowspan="2"><p align="center">会议名称 </p></td>
                <td rowspan="2"><p align="center">举办开始时间 </p></td>
                <td rowspan="2"><p align="center">举办终止时间 </p></td>
                <td colspan="2"><p align="center">参加人员 </p></td>
                <td rowspan="2"><p align="center">项目操作 </p></td>
            </tr>
            <tr style="background-color:lavender">
                <td><p align="center">总人数 </p></td>
                <td><p align="center">国（境）外人员数 </p></td>
            </tr>

            <s:iterator value="%{recordList}" id="id">
                <tr>
                    <td><p align="center"><s:property value="#id.name"/></p></td>
                    <td><p align="center"><s:property value="castToString(#id.holdStartDate)"/></p></td>
                    <td><p align="center"><s:property value="castToString(#id.holdEndDate)"/></p></td>
                    <td><p align="center"><s:property value="#id.totalPeople"/></p></td>
                    <td><p align="center"><s:property value="#id.foreignPeople"/></p></td>
                    <td><p align="center">
                            <s:url id="url" method="delete">
                                <s:param name="selectedId" value="#id.id"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a>
                        </p></td>
                </tr>
            </s:iterator>

            <tr>
                <td><p align="center"><s:textfield theme="simple" name="meetingName" size="6"/></p></td>
                <td><p align="center"><jsc:head theme="simple"/><jsc:jscalendar theme="simple" name="startDate" format="%Y-%m-%d" showstime="true" size="6"/></p></td>
                <td><p align="center"><jsc:head theme="simple"/><jsc:jscalendar theme="simple" name="endDate" format="%Y-%m-%d" showstime="true" size="6"/></p></td>
                <td><p align="center"><s:textfield theme="simple" name="totalCount" size="4"/></p></td>
                <td><p align="center"><s:textfield theme="simple" name="foreignCount" size="4"/></p></td>
                <td><p align="center"><s:submit theme="simple" value="添加" method="insert"/></p></td>
            </tr>
        </table>
        <br/>
        <p align="center"><s:submit theme="simple" value="导出" method="export"/></p>
    </s:form>
</body>

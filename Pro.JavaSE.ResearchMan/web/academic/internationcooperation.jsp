<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="internationcooperation" theme="simple">
        <table align="center" border="1" cellspacing="0" cellpadding="0">
            <tr>
                <td style="background-color: rgb(255, 255, 51)" width="690" colspan="4"><p align="center"><strong>本一级学科点承担的国际交流合作项目 </strong></p></td>
            </tr>
            <tr style="background-color:papayawhip">
                <td ><p align="center">国际交流合作项目名称 </p></td>
                <td ><p align="center">起止时间(0默认至今) </p></td>
                <td ><p align="center">合作对象 </p></td>
                <td ><p align="center">项目操作 </p></td>
            </tr>
            <s:iterator value="%{recordList}" id="id">
                <tr>
                    <td valign="top"><p><s:property value="#id.name"/></p></td>
                    <td valign="top"><p><s:property value="convert(#id.startYear)"/>-<s:property value="convert(#id.endYear)"/></p></td>
                    <td valign="top"><p><s:property value="#id.target"/></p></td>
                    <td valign="top"><p>
                            <s:url id="url" method="delete">
                                <s:param name="selectedId" value="#id.id"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a></p></td>
                    </tr>
            </s:iterator>
            <tr>
                <td valign="top"><p><s:textfield name="name" theme="simple" size="6"/></p></td>
                <td valign="top"><p><s:textfield name="start" size="4" theme="simple"/>-<s:textfield name="end" size="4" theme="simple"/></p></td>
                <td valign="top"><p><s:textfield name="target" theme="simple" size="6"/></p></td>
                <td valign="top"><p><s:submit theme="simple" value="添加" method="insert"/></p></td>
            </tr>
        </table>
        <br/>
        <p align="center"><s:submit theme="simple" value="导出" method="export"/></p>
    </s:form>
</body>

<%-- 
    Document   : queryauthority
    Created on : 2011-9-2, 22:37:50
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="queryauthority" theme="simple" align="center">
        查询权限类别: <s:select theme="simple" list="authorities" listValue="name" value="bean.id" listKey="id" name="authorityId"/>
        <s:submit theme="simple" value="查询权限的ID" method="queryId"/><br/><br/>
        <table align="center" border="1" cellspacing="0" cellpadding="0" width="690">
            <tbody>
                <tr style="background-color: rgb(255, 255, 51)">
                    <th>注册ID</th>
                    <th>权限名称</th>
                    <th>权限映射</th>
                </tr>
                <s:iterator value="authorities" status="st" id="id">
                    <tr align="center" style="font-size: small;<s:if test="#st.odd">background-color:powderblue</s:if>">
                        <td width="100"><s:property value="#id.id"/> </td>
                        <td width="245"><s:property value="#id.name"/> </td>
                        <td width="245"><s:property value="#id.function"/></td>
                    </tr>
                </s:iterator>
            </tbody>
        </table>
        <s:hidden theme="simple" name="page"/>
        <div align="center">
            <s:submit disabled="currentPage()=='first'" theme="simple" value="上一页" method="prev"/>&nbsp;
            第 <s:property value="page"/> 页
            &nbsp;<s:submit disabled="currentPage()=='last'" theme="simple" value="下一页" method="next"/></div>
    </s:form>
</body>

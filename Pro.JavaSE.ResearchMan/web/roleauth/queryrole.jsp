<%-- 
    Document   : queryauthority
    Created on : 2011-9-2, 22:37:50
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="queryrole" theme="simple" align="center">
        查询角色类别: <s:select theme="simple" list="roles" listValue="name" value="bean.id" listKey="id" name="roleId"/>
        <s:submit theme="simple" value="查询角色的ID" method="queryId" /><br/><br/>
        <table border="1" cellspacing="0" cellpadding="0" width="600" align="center">
            <tbody>
                <tr style=" background:yellow">
                    <th>注册ID</th>
                    <th>角色名称</th>
                </tr>
                <s:iterator value="roles" status="st" id="id">
                    <tr align="center" style="font-size: small;<s:if test="#st.odd">background-color:powderblue</s:if>">
                        <td width="200"><s:property value="#id.id"/> </td>
                        <td width="390"><s:property value="#id.name"/> </td>
                    </tr>
                </s:iterator>
            </tbody>
        </table>
    </s:form>
</body>

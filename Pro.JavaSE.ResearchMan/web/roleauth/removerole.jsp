<%-- 
    Document   : removeauthority
    Created on : 2011-9-3, 19:32:43
    Author     : Ghostplant
--%>


<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="removerole">
        <table align="center">
            <thead>
                <tr>
                    <th style=" color:tomato">角色名称列表</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <s:select list="roles" listValue="name" value="bean.id" listKey="id" name="roleId" label="角色类别"/>
                        <s:submit value="删除选定角色" method="remove" />
                    </td>
                </tr>
            </tbody>
        </table>
    </s:form>
</body>
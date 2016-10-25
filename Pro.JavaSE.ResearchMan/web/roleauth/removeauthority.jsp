<%-- 
    Document   : removeauthority
    Created on : 2011-9-3, 19:32:43
    Author     : Ghostplant
--%>


<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="removeauthority">
        <table align="center">
            <thead>
                <tr>
                    <th style=" color:tomato">权限名称列表</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <s:select list="authorities" listValue="name" value="bean.id" listKey="id" name="authorityId" label="权限类别"/>
                        <s:submit value="删除选定权限" method="remove" />
                    </td>
                </tr>
            </tbody>
        </table>
    </s:form>
</body>
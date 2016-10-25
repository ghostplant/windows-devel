<%-- 
    Document   : userexport
    Created on : 2011-9-19, 22:20:04
    Author     : 编程是一门艺术
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <table  align="center" border="1" cellspacing="0" cellpadding="0" width="600">
        <tr style="background-color: rgb(255, 255, 51)" align="center" valign="center"><td><strong>请选择要导出的用户</strong></td></tr>
        <tr align="center" valign="center">
            <td>
                <s:form action="exportuser">
                    <s:select list="users" name="selectedUser"/>
                    <s:submit value="导出" method="export"/>
                </s:form>
            </td>
        </tr>
    </table>
</body>
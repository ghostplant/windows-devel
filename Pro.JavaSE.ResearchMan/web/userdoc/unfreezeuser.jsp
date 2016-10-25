<%-- 
    Document   : unfreezeuser
    Created on : 2011-9-8, 22:03:43
    Author     : 编程是一门艺术
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <table  align="center" border="1" cellspacing="0" cellpadding="0" width="600">
        <tr style="background-color: rgb(255, 255, 51)" align="center" valign="center"><td><strong>解冻用户</strong></td></tr>
        <tr align="center" valign="center">
            <td>
                <s:form action="unfreezeuser">
                    <s:select list="users" name="selectedUser"/>
                    <s:submit value="解冻该用户" method="unfreeze"/>
                </s:form>
            </td>
        </tr>
    </table>
</body>
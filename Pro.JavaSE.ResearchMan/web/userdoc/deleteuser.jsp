<%-- 
    Document   : deleteuser
    Created on : 2011-9-9, 19:30:55
    Author     : 编程是一门艺术
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<body>
    <table  align="center" border="1" cellspacing="0" cellpadding="0" width="600">
        <tr style="background-color: rgb(255, 255, 51)" align="center" valign="center"><td><strong>删除用户</strong></td></tr>
        <tr  align="center" valign="center"><td><br/>删除操作不可逆,将删除数据库里所有该用户的信息<br/>建议使用【冻结用户】功能<br/><br/></td></tr>
        <tr style="background-color: rgb(255, 255, 51)" align="center" valign="center"><td><strong>删除老师</strong></td></tr>
        <tr align="center" valign="center"><td>
                <s:form theme="simple" action="deleteuser"><br/>
                    <s:select theme="simple" list="users" name="selectedUser"/>
                    <s:submit theme="simple" value="删除该老师" method="delete"/>
                </s:form><br/>
            </td></tr>
        <tr style="background-color: rgb(255, 255, 51)" align="center" valign="center"><td>
                <strong>删除学生</strong>
            </td></tr>
        <tr align="center" valign="center"><td>
                <s:form theme="simple" action="deleteuser"><br/>
                    <s:select theme="simple" list="students" name="selectedStudents"/>
                    <s:submit theme="simple" value="删除该学生" method="deleteStudent"/>
                </s:form><br/>
            </td></tr>
    </table>
</body>

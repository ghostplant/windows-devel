<%-- 
    Document   : adduser
    Created on : 2011-9-1, 16:43:16
    Author     : Ghostplant
--%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib  prefix="s" uri="/struts-tags" %>

<body>
    <form action="adduser" validate="true">
        <table  align="center" border="1" cellspacing="0" cellpadding="0" width="600">
            <tr style="background-color: rgb(255, 255, 51)" align="center" valign="center"><td colspan="2"><strong>增加用户</strong></td></tr>
            <tr align="center" valign="center">
                <td >
                    <s:textfield name="userid" label="老师工资号或学生学号" />
                    <s:textfield name="username" label="姓名" />
                    <s:radio name="gender" value="'男'" list="{'男','女'}" label="性别"/>
                    <s:textfield name="department" label="部门" />
                    <s:password name="password" label="密码" />
                    <s:password name="confirm" label="确认密码" />
                    <s:submit value="确认注册" method="register" />
                </td>
            </tr>
        </table>
    </form>
</body>

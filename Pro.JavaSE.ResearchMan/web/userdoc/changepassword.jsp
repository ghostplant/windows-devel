<%-- 
    Document   : changepassword
    Created on : 2011-9-9, 20:04:43
    Author     : 编程是一门艺术
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib  prefix="s" uri="/struts-tags" %>

<body>

    <form action="changepassword" validate="true">
        <table align="center"  border="1" cellspacing="0" cellpadding="0" width="600">
            <thead>
                <tr style="background-color: rgb(255, 255, 51)" align="center" valign="center"><td colspan="2"><strong>修改密码</strong></td></tr>

            </thead>
            <tbody>
                <tr>
                    <td>
                        <s:password name="oldpassword" label="旧密码" />
                        <s:password name="newpassword" label="新密码" />
                        <s:password name="confirm" label="确认新密码" />
                        <s:submit value="确认修改" method="change" />
                    </td>
                </tr>
            </tbody>
        </table>
    </form>
</body>

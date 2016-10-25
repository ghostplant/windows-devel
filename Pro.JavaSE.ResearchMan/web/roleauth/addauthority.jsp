<%-- 
    Document   : authority
    Created on : 2011-9-2, 22:11:23
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="addauthority" validate="true">
        <table align="center">
            <thead>
                <tr>
                    <th style="color: tomato">新建项名称与说明</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <s:textfield name="authoriyName" label="自定义权限名称" />
                        <s:textfield name="authoriyFunction" label="权限映射功能模块" />
                        <s:submit value="确认提交" method="submit" />
                    </td>
                </tr>
            </tbody>
        </table>
    </s:form>
</body>

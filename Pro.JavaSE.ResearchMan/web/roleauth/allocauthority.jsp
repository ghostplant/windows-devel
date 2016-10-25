<%-- 
    Document   : allocauthority
    Created on : 2011-9-3, 20:16:19
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="allocauthority" theme="simple">
        <s:if test="firstStep">
            选择角色: <s:select list="roles" listKey="id" listValue="name" name="roleId" value="bean.id" theme="simple"/>
            <s:submit value="下一步" method="next" theme="simple"/>
        </s:if><s:else>
            <s:hidden name="selectedRoleId" theme="simple"/>
            <s:hidden name="selectedRoleName" theme="simple"/>
            <table  border="1" cellspacing="0" cellpadding="0" style=" font-size:  smaller">
                <tbody>
                    <tr>
                        <td>所选角色的名称</td>
                        <td><s:property value="textSelectedRoleName"/></td>
                    </tr>
                    <tr>
                        <td>权限类别</td>
                        <td>
                            <s:checkboxlist name="access" list="authorities" theme="simple"/><br/>
                            <p align="center"><s:submit value="更新权限" method="update" theme="simple"/></p>
                        </td>
                    </tr>
                </tbody>
            </table>

        </s:else>
    </s:form>
</body>
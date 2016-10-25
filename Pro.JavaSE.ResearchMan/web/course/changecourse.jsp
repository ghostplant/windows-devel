<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="changecourse" theme="simple">
        <table border="1" cellspacing="0" cellpadding="0">
            <tr>
                <td width="690"  align="center" valign="middle" colspan="9" style="background-color: rgb(255, 255, 51)"><p><strong>课程管理系统</strong><strong>（更新课程）</strong></p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td><p align="center">课程编号 </p></td>
                <td><p align="center">课程名称</p></td>
                <td><p align="center">学分</p></td>
                <td><p align="center">学时</p></td>
                <td><p align="center">授课对象</p></td>
                <td><p align="center">类型（公共、专业等）</p></td>
                <td><p align="center">教师</p></td>
                <td><p align="center">状态</p></td>
                <td><p align="center">项目操作</p></td>
            </tr>
           
            <tr style="background-color: rgb(153, 255, 255)">
                <td valign="top"><p align="center"><s:textfield theme="simple" name="id" size="1" /></p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="name" size="2"/></p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="creditValue" size="1" /></p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="creditHour" size="1" /></p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="target" size="1" /></p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="type" size="1" /></p></td>
                <td valign="top"><p align="center">
                        <s:hidden name="teacherList"/>
                        <s:generator val="teacherList" separator="~">
                            <s:iterator>
                                <s:property/><br/>
                            </s:iterator>
                        </s:generator>
                        <s:select list="teachers" name="item" /><br/>
                        <s:submit value="列入" theme="simple" method="addItem"/>
                        <s:submit value="清空" theme="simple" method="clearItem"/>
                    </p>
                </td>
                <td valign="top"><p align="center"><s:radio name="state" value="'已开设'" list="{'已开设','待开放'}"/></p></td>
                <td valign="top"><p align="center"><s:submit theme="simple" value="更新" method="update"/></p></td>
            </tr>
        </table>
    </s:form>
</body>

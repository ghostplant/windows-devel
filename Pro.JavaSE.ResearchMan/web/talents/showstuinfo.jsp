<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="changestuinfo" theme="simple">
        <table border="1" cellspacing="0" cellpadding="0">
            <tr>
                <td width="690"  align="center" valign="middle" colspan="12" style="background-color: rgb(255, 255, 51)"><p><strong>显示学生信息</strong></p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td><p align="center">学号（学号为主键） </p></td>
                <td><p align="center">姓名</p></td>
                <td><p align="center">宿舍地址</p></td>
                <td><p align="center">电话</p></td>
                <td><p align="center">家庭地址</p></td>
                <td><p align="center">民族</p></td>
                <td><p align="center">获得奖项</p></td>
                <td><p align="center">学历</p></td>
                <td><p align="center">导师</p></td>
                <td><p align="center">入学年份</p></td>
                <td><p align="center">毕业年份</p></td>
                <td><p align="center">删除</p></td>
            </tr>

            <s:iterator value="%{studentList}" id="stu">
                <tr style="background-color: rgb(153, 255, 255)">
                    <td valign="top"><p align="center"><s:property value="#stu.id"/></p></td>
                    <td valign="top"><p align="center"><s:property value="getStudentName(#stu.id)"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#stu.dormNo"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#stu.dormPhone"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#stu.position"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#stu.nation"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#stu.awards"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#stu.degree"/></p></td>
                    <td valign="top"><p align="center"><s:property value="getTeacherName(#stu.teacherId)"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#stu.entranceYear"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#stu.graduationYear"/></p></td>
                    <td valign="top"><p align="center">
                            <s:url id="url" method="delete">
                                <s:param name="selectedId" value="#stu.id"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a>
                        </p>
                    </td>
                </tr>
            </s:iterator>


        </table>
    </s:form>
</body>

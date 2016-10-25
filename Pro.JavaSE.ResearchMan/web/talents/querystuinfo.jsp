<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="querystuinfo" theme="simple">
        <table border="1" cellspacing="0" cellpadding="0">
            <tr>
                <td width="690"  align="center" valign="middle" colspan="12" style="background-color: rgb(255, 255, 51)"><p><strong>查询学生信息</strong></p></td>
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
                <td><p align="center">查询</p></td>
            </tr>

            <tr style="background-color: rgb(153, 255, 255)">
                <td><p align="center">-- </p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center">--</p></td>
                <td><p align="center"><s:textfield name="ID" size="8"/><s:submit value="按学号查询" method="search"/></p></td>
</tr>

<tr style="background-color: rgb(153, 255, 255)">
    <td valign="top"><p align="center"><s:property value="student.id"/></p></td>
    <td valign="top"><p align="center"><s:property value="getStudentName(student.id)"/></p></td>
    <td valign="top"><p align="center"><s:property value="student.dormNo"/></p></td>
    <td valign="top"><p align="center"><s:property value="student.dormPhone"/></p></td>
    <td valign="top"><p align="center"><s:property value="student.position"/></p></td>
    <td valign="top"><p align="center"><s:property value="student.nation"/></p></td>
    <td valign="top"><p align="center"><s:property value="student.awards"/></p></td>
    <td valign="top"><p align="center"><s:property value="student.degree"/></p></td>
    <td valign="top"><p align="center"><s:property value="getTeacherName(student.teacherId)"/></p></td>
    <td valign="top"><p align="center"><s:property value="student.entranceYear"/></p></td>
    <td valign="top"><p align="center"><s:property value="student.graduationYear"/></p></td>
    <td valign="top"><p align="center">--</p></td>
</tr>



</table>
</s:form>
</body>

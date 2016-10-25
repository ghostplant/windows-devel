<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="changestuinfo" theme="simple">
        <table border="1" cellspacing="0" cellpadding="0" align="center">
            <tr>
                <td width="690"  align="center" valign="middle" colspan="2" style="background-color: rgb(255, 255, 51)"><p><strong>更改学生信息</strong></p></td>
            </tr>
            <tr>
                <td width="200" style=" background-color: skyblue"><p align="center">学号(主键)</p></td>
                <td valign="top"><p align="center"><s:select list="studentList" listKey="id" listValue="id" theme="simple" name="id" /><s:submit theme="simple" method="show" value="列出" /></p></td>
            </tr>
            <tr>
                <td width="200" style=" background-color: skyblue"><p align="center">姓名</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="studentName" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">宿舍地址</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="dormNo" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">电话</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="dormPhone" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">职务</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="position" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">民族</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="nation" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">获得奖项</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="awards" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">学历</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="degree" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">导师</p></td>
                <td valign="top"><p align="center"><s:select list="teachers" listKey="id" listValue="name" name="selectedTeacherId"  /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">入学年份</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="entranceYear" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">毕业年份</p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="graduationYear" /></p></td>
            </tr><tr>
                <td width="200" style=" background-color: skyblue"><p align="center">更新</p></td>
                <td valign="top"><p align="center"><s:submit theme="simple" value="更新" method="update" /></p></td>
            </tr>

        </table>
    </s:form>
</body>

<%-- 
    Document   : edituser
    Created on : 2011-9-9, 21:11:05
    Author     : 编程是一门艺术
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="jsc" uri="/jscalendar" %>
<%@taglib  prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="edituser" validate="true" theme="simple">
        <table align="center" width="600"  border="1" cellspacing="0" cellpadding="0">
            <tbody  align="center">
                <tr>
                    <td style="background-color: rgb(255, 255, 51)" colspan="2" align="center" height="50">
                        <strong>修改用户信息</strong></td>
                </tr>
                <tr>
                    <td width="200" style=" background-color: skyblue">
                        教师工资号
                    </td>
                    <td>
                        <s:label name="#session.user.id" theme="simple"/>
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        姓名
                    </td>
                    <td align="center">
                        <s:textfield name="username"/>
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        性别
                    </td>
                    <td align="center">
                        <s:radio name="gender" value="'男'" list="{'男','女'}"/>
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        手机号码
                    </td>
                    <td align="center">
                        <s:textfield name="mobilePhone"/>
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        部门
                    </td>
                    <td align="center">
                        <s:textfield name="department" />
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        办公地址
                    </td>
                    <td align="center">
                        <s:textfield name="officeAddress" />
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        办公电话
                    </td>
                    <td align="center">
                        <s:textfield name="officePhone" />
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        研究方向
                    </td>
                    <td align="center">
                        <s:textfield name="major" />
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        通信地址
                    </td>
                    <td align="center">
                        <s:textfield name="contactAddress" />
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        生日
                    </td>
                    <td align="center">
                        <jsc:head theme="simple"/><jsc:jscalendar theme="simple" name="birthady" format="%Y-%m-%d" showstime="true"/>
                    </td>
                </tr>
                <%-- <s:textfield name="birthady"  label="生日，格式(1900-01-01)" /> --%>
                <tr>
                    <td style=" background-color: skyblue">
                        专业技术职务及专家称谓
                    </td>
                    <td align="center">
                        <s:textfield name="expertTitle"/>
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        最高学位
                    </td>
                    <td align="center">
                        <s:textfield name="finalDegree" />
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        最后毕业学校
                    </td>
                    <td align="center">
                        <s:textfield name="finalSchool"/>
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        最后毕业专业
                    </td>
                    <td align="center">
                        <s:textfield name="finalMajor" />
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue">
                        最后毕业时间
                    </td>
                    <td align="center">
                        <jsc:head theme="simple"/><jsc:jscalendar theme="simple" name="graduationDate" format="%Y-%m-%d" showstime="true"/>
                    </td>
                </tr>
                <%-- <s:textfield name="graduationDate" label="最后毕业时间(1900-01-01)" /> --%>

                <tr>
                    <td style=" background-color: skyblue">
                        工作单位
                    </td>
                    <td align="center">
                        <s:textfield name="workingSection"/>
                    </td>
                </tr>
                <tr>
                    <td style=" background-color: skyblue" colspan="2">
                        <s:submit value="确认修改" method="change" />
                    </td>
                </tr>
            </tbody>
        </table>
    </s:form>
</body>

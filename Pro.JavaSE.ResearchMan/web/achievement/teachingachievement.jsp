<%--
    Document   : teachingachievement
    Created on : 2011-9-7, 21:20:11
    Author     : zyz
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<%@taglib prefix="jscalendar" uri="/jscalendar" %>

<html>
    <body>
        <s:form action="teachingachievement" theme="simple">
            <s:div align="center">
                <h1>教学成果录入</h1>
            </s:div>
            <table border="1" cellspacing="0" cellpadding="0" width="790" align="center">
                <tr>
                    <td width="90" valign="center"><p align="center">*成果名称</p></td>
                    <td width="90" valign="center"><p align="center">*完成人数</p></td>
                    <td width="276" valign="center"><p align="center">*获奖者</p></td>
                    <td width="90" valign="center"><p align="center">*获奖名称</p></td>
                    <td width="50" valign="center"><p align="center">等级</p></td>
                    <td width="90" valign="center"><p align="center">证书号 </p></td>
                    <td width="90" valign="center"><p align="center">取得时间 </p></td>
                </tr>
                <tr>
                    <td width="90" valign="center" align="center">
                        <s:textfield name="name" size="4" maxlength="150"/>
                    </td>
                    <td width="90" valign="center" align="center">
                        <s:textfield name="finishCount" size="4" maxlength="150"/>
                    </td>
                    <td width="276" valign="center">
                        <table>
                            <tr>
                                <td>
                                    <table>
                                        <tr>
                                            <td width="100" align="center">
                                                获奖者
                                            </td>
                                            <td width="100" align="center">
                                                排名
                                            </td>
                                            <td width="76" align="center">
                                                <s:submit method="delAll" value="重置"/>
                                            </td>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    <s:iterator value="personMap.keySet()" id="key" status="st">
                                        <table>
                                            <tr>
                                                <td width="100" align="center">
                                                    <s:property value="personMap.get(#key)"/>
                                                </td>
                                                <td width="100" align="center">
                                                    <s:property value="#key"/>
                                                </td>
                                                <td width="76" align="center">
                                                </td>
                                            </tr>
                                        </table>
                                    </s:iterator>
                                </td>
                            </tr>
                            <tr>
                                <td>
                                    <table>
                                        <tr>
                                            <td width="100" align="center">
                                                <s:select list="teachers" name="teacherName"/>
                                            </td>
                                            <td width="100" align="center">
                                                <s:textfield name="rank" size="4" maxlength="150"/>
                                            </td>
                                            <td width="76" align="center">
                                                <s:submit value="添加" method="addPerson"/>
                                            </td>
                                        </tr>
                                    </table>
                                </td>
                            </tr>
                        </table>
                    </td>
                    <td width="90" valign="center" align="center">
                        <s:textfield name="award" size="4" maxlength="150"/>
                    </td>
                    <td width="50" valign="center" align="center">
                         <s:select list="{'','特等奖','一等奖','二等奖','三等奖','鼓励奖','其他'}" name="level"/>
                    </td>
                    <td width="90" valign="center" align="center">
                        <s:textfield name="certificationId" size="4" maxlength="150"/>
                    </td>
                    <td width="90" valign="center" align="center">
                        <jscalendar:head/>
                        <jscalendar:jscalendar name="date" format="%Y-%m-%d" showstime="true" size="6"/>
                    </td>
                </tr>
            </table>
            <s:div align="center">
                <s:submit label="提交" method="submit" value="提交"/>
            </s:div>
        </s:form>
    </body>
</html>
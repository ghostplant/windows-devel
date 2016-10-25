<%--
    Document   : patentachievement
    Created on : 2011-9-14, 17:47:14
    Author     : zyz
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<%@taglib prefix="jscalendar" uri="/jscalendar" %>
<!DOCTYPE html>
<html>

    <body>
        <s:form action="patentachievement" theme="simple">
            <s:div align="center">
                <h1>专利著作录入</h1>
            </s:div>
            <table border="1" cellspacing="0" cellpadding="0" width="700" align="center">
                <tr>
                    <td width="90" valign="center"><p align="center">专利号 </p></td>
                    <td width="90" valign="center"><p align="center">著作权登记号</p></td>
                    <td width="90" valign="center"><p align="center">名称 </p></td>
                    <td width="50" valign="center"><p align="center">完成人数</p></td>
                    <td width="276" valign="center"><p align="center">本单位所有权利人</p></td>
                    <td width="90" valign="center"><p align="center">取得时间 </p></td>
                </tr>
                <tr>
                    <td width="90" valign="center" align="center">
                        <s:textfield name="numA" size="4" maxlength="150"/>
                    </td>
                    <td width="90" valign="center" align="center">
                        <s:textfield name="numB" size="4" maxlength="150"/>
                    </td>
                    <td width="90" valign="center" align="center">
                        <s:textfield name="name" size="4" maxlength="150"/>
                    </td>
                    <td width="50" valign="center" align="center">
                        <s:textfield name="finishCount" size="1" maxlength="9"/>
                    </td>
                    <td width="276" valign="center">
                        <table>
                            <tr>
                                <td>
                                    <table>
                                        <tr>
                                            <td width="100" align="center">
                                                权利人
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
                                                <s:select list="teachers" name="personName"/>
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
                        <jscalendar:head/>
                        <jscalendar:jscalendar name="date" format="%Y-%m-%d" showstime="true" size="6"/>
                    </td>
                </tr>
            </table>
            <s:div align="center">
                <s:submit method="submit" value="提交"/>
            </s:div>
        </s:form>
    </body>
</html>
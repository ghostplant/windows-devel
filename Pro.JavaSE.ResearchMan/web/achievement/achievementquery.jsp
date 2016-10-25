<%--
    Document   : achievementquery
    Created on : 2011-9-15, 20:48:51
    Author     : zyz
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<%@taglib prefix="jscalendar" uri="/jscalendar" %>
<!DOCTYPE html>
<html>

    <body>
        <s:form action="achievementquery" theme="simple">
            <div align="center" align="center">
                <h1>成果查询</h1>
            </div>
            成果所有者：<s:select list="persons" name="teacherName"/><br/>
            成果类型：<s:select list="{'','获奖','论文','发明专利'}" name="achievementType"/><br/>
            时间：从<jscalendar:head/>
            <jscalendar:jscalendar name="startDate" format="%Y-%m-%d" showstime="true" size="6"/>
            到<jscalendar:head/>
            <jscalendar:jscalendar name="endDate" format="%Y-%m-%d" showstime="true" size="6"/><br/>
            <s:submit method="query" value="查找"/>
            <table align="center">
                <tr>
                    <td width="50">
                        姓名
                    </td>
                    <td width="300">
                        成果名称（获奖、论文、专著、发明专利等）
                    </td>
                    <td width="300">
                        获奖名称、等级及证书号，刊物名称及ISSN、<br/>检索号，出版单位及ISBN,专利授权号
                    </td>
                    <td width="50">
                        时间
                    </td>
                    <td width="50">
                        署名<br/>情况
                    </td>
                    <td width="50"></td>
                </tr>
            </table>
            <s:iterator value="tupleList" id="tuple">
                <table>
                    <tr>
                        <td width="50">
                            <s:property value="presonName"/>
                        </td>
                        <td width="300">
                            <s:property value="achievementName"/>
                        </td>
                        <td width="300">
                            <s:property value="detail"/>
                        </td>
                        <td width="50">
                            <s:property value="date"/>
                        </td>
                        <td width="50">
                            <s:property value="sign"/>
                        </td>
                        <td width="50">
                            <s:url id="url" method="delete">
                                <s:param name="delKey" value="#tuple.key"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a>
                            </td>
                        </tr>
                    </table>
            </s:iterator>
        </s:form>
    </body>
</html>